# this make file is for compile all source files in this directory

.SUFFIXES: .c

CC = gcc
CFLAGS = -g -Wall -lpthread -D_REENTRANT

SRCS = listener.c talker.c hello_server.c hello_client.c low_read.c fd_seri.c tcp_client.c \
       endian_conv.c inet_addr.c inet_aton.c inet_ntoa.c echo_server.c echo_client.c \
       echo_client2.c op_client.c op_server.c uecho_server.c uecho_client.c bound_host1.c \
       bound_host2.c uecho_con_client.c file_server.c file_client.c gethostbyname.c \
       gethostbyaddr.c sock_type.c get_buf.c set_buf.c reuseadr_eserver.c fork.c \
       zombie.c wait.c waitpid.c signal.c sigaction.c remove_zombie.c echo_mpserv.c \
       echo_mpclient.c pipe1.c pipe2.c pipe3.c echo_storeserv.c select.c echo_selectserv.c \
       oob_send.c oob_recv.c peek_send.c peek_recv.c writev.c readv.c news_sender.c \
       news_receiver.c news_sender_brd.c news_receiver_brd.c syscpy.c stdcpy.c \
       desto.c todes.c echo_stdserv.c echo_stdclnt.c sep_serv.c sep_clnt.c dup.c \
       sep_serv2.c echo_epollserv.c echo_EPLTserv.c echo_EPETserv.c thread1.c \
       thread2.c thread3.c thread4.c mutex.c semaphore.c chat_server.c chat_clnt.c

TARGET = $(SRCS:.c=)

all: $(TARGET)

clean:
	rm -rf $(TARGET) core
