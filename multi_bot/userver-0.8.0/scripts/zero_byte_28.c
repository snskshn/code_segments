/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
    See AUTHORS file for list of contributors to the project.
  
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA
*/


/*
 * Title   : genspec99sesslogs.c
 * Author  : Tim Brecht
 * Date    : Fri Apr 25 19:29:49 EDT 2003
 *
 * Generate a session log file that can be used with httperf
 * to generate a load that conforms with the static portion 
 * of the specweb99 workload.
 *
 * With the right options we should also generate a log file
 * that can be used to prime the cache by requesting each and
 * every file in the set of possible files once.
 */

/*
 * Size information for SpecWeb99 file set
 *             Class   ClassSize  Cumulative
 *     102      0.00                     102
 *     204      0.00                     306
 *     307      0.00                     613
 *     409      0.00                    1022
 *     512      0.00                    1534
 *     614      0.00                    2148
 *     716      0.00                    2864
 *     819      0.00                    3683
 *     921      0.00        4604        4604
 *    1024      1.00                    5628
 *    2048      1.00                    7676
 *    3072      1.00                   10748
 *    4096      1.00                   14844
 *    5120      1.00                   19964
 *    6144      1.00                   26108
 *    7168      1.00                   33276
 *    8192      1.00                   41468
 *    9216      1.00       46080       50684
 *   10240      2.00                   60924
 *   20480      2.00                   81404
 *   30720      2.00                  112124
 *   40960      2.00                  153084
 *   51200      2.00                  204284
 *   61440      2.00                  265724
 *   71680      2.00                  337404
 *   81920      2.00                  419324
 *   92160      2.00      460800      511484
 *  102400      3.00                  613884
 *  204800      3.00                  818684
 *  307200      3.00                 1125884
 *  409600      3.00                 1535484
 *  512000      3.00                 2047484
 *  614400      3.00                 2661884
 *  716800      3.00                 3378684
 *  819200      3.00                 4197884
 *  921600      3.00     4608000     5119484
 *
 * 5119484               5119484
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <assert.h>

/* #define LOCAL_DEBUG */

#ifdef LOCAL_DEBUG
#define ldbg(...) \
  do {\
    printf(__VA_ARGS__); \
    fflush(stdout); \
    fflush(stderr); \
  } while (0);
#else
#define ldbg(x, ...)
#endif

#define LOGFILE_NAME             "httpspec99.log"
#define PRIME_LOGFILE_NAME       "httpspec99-prime.log"
#define PRIME_CLEAR_LOGFILE_NAME "httpspec99-prime-clear.log"
#define STATSFILE                "/tmp/statsfile"
#define TEMPFILE                 "/tmp/spec.log"
#define REQUESTDIR               "specweb99/file_set/dir"

#define MAX_FILE_CHARS (200)
#define MAX_CLASS      (4)
#define MAX_FILE       (8)
#define MIN_SESSION    (1)
#define MAX_SESSION    (15)
#define FILES_PER_DIR  ((MAX_FILE+1) * (MAX_CLASS+1))

#define BYTES_PER_DIR  (5119484)
#define MBS_PER_DIR    (4.88232040405273437500)

void    get_file_class_dir(int *file, int *class, int *dir, int num_dirs);
void    get_file_class_dir_prime(int *file, int *class, int *dir, int num_dirs);
double  get_think_time();
int     get_session_len();
int     distribution_add_and_check(int file, int class, int dir, double epsilon);
int     session_add_and_check(int session_len, double epsilon );
void    usage();
int     tolerance_met(double *dist, double *pdf, int index, double epsilon);
void    writeDist(FILE * sf, double *dist, double *pdf, int size, double epsilon);


/* Globals */
static char *progname;
static int  num_dirs = -1;
static int  max_size = -1;
static int  use_think_times = 0;
static int  prime_cache = 0;
static double tolerance = 0.1;

/* CDF (discrete distribution) for various kinds of files in the SpecWeb fileset. 
   See SpecWeb user guide for details*/
static double fileCDF[MAX_FILE+1] = {0.039, 0.098, 0.186, 0.363, 0.716, 0.834, 0.905, 0.955, 1.0};

/* PDF (discrete distribution) for various kinds of files in the SpecWeb fileset */
static double filePDF[MAX_FILE+1] = {0.039, 0.059, 0.088, 0.177, 0.353, 0.118, 0.071, 0.05, 0.044};

/* CDF (discrete distribution) for file classes in the SpecWeb fileset.*/
static double classCDF[MAX_CLASS+1] = {0.252, 0.612, 0.7128, 0.72 ,1.0};

/* CDF (discrete distribution) for the file classes in the SpecWeb fileset*/
static double classPDF[MAX_CLASS+1] = {0.252, 0.36, 0.1008, 0.0072, 0.28};

static double sessionPDF[MAX_SESSION+1] = {0.0, 0.3, 0.0, 0.0, 0.0, 0.0636, 0.0636, 0.0636, 0.0636, 
                                           0.0636, 0.0636, 0.0636, 0.0636, 0.0636, 0.0636, 0.0636};

/* These will eventually point to arrays that will hold the CDF and PDF respectively. 
   The size of the table depends on the max_dir parameter, so we can't allocate arrays a priori */
static double * dirCDF;
static double * dirPDF;

/* Track how many files of each type are actually generated in our workload.*/
static double fileDist[MAX_FILE+1];

/* Track how many files of each class are actually generated in our workload.*/
static double classDist[MAX_CLASS+1];

/* Track how many sessions of each length are actually generated in the workload */
static double sessionDist[MAX_SESSION+1];

/* Track how many times each directory appears in the workload */
static double * directoryDist;


/* Total number of files generated in the workload */
static double totalFiles = 0.0;

/* Total number of sessions in the workload*/
static int session_count = 0;

/* Seed for the random number generator*/
unsigned int random_seed = 0;

void usage()
{
  printf("Usage: %s [-d #] [-t] [-p] [-s #] [-hH?] \n", progname);
  printf("-c     : add special userver url to clear stats\n");
  printf("-d #   : number of directories in workload (numbered from zero)\n");
  printf("-p     : produce a log that can be used to prime the cache\n");
  printf("-s #   : minimum number of sessions\n");
  printf("-S #   : maximum number of sessions\n");
  printf("-m #   : limit the size of the file set to # MB (just below)\n");
  printf("-t #.# : run until distribution tolerances are within a factor of #.#\n");
  printf("-T     : use think times\n"); 
  printf("-h     : help\n"); 
  printf("-H     : help\n"); 
  printf("-?     : help\n"); 
}



/*
  Initialize the tracking arrays for each file type and file class.
  These arrays are used to track how many files of each type and class are generated
  Requires the num_dirs global to have been set!
*/
void init() {
  int i;
  double tableSum = 0.0;
  double cumulative = 0.0;

  for( i = 0; i <= MAX_FILE; i++ ) {
    fileDist[i] = 0.0;
  }
  
  for( i = 0; i <= MAX_CLASS; i++ ) {
    classDist[i] = 0.0;
  }
  
  for( i = 0; i <= MAX_SESSION; i++ ) {
    sessionDist[i] = 0.0;
  }

  if( num_dirs > 0 ) {
    /* Allocate an array to track the directory distribution */
    directoryDist = (double*) malloc( sizeof(double) * num_dirs );
    /* Generate the discrete zipf distribution for the directory access pattern */
    dirCDF = (double*) malloc( sizeof(double) * num_dirs );
    dirPDF = (double*) malloc( sizeof(double) * num_dirs );
    dirPDF[0] = 0.0;

    for( i = 0; i < num_dirs; i++ ) {
      directoryDist[i] = 0.0;
      dirPDF[i] = ((double) 1.0) / ((double) i+1);
      tableSum += dirPDF[i];
    }

    for( i = 0; i < num_dirs; i++ ) {
      dirPDF[i] = (dirPDF[i] / tableSum);
      cumulative += dirPDF[i];
      dirCDF[i] = cumulative;
    }
    /* for */
  }
  /* if */
}

void writeStatsFile( FILE * sf, int argc, char * argv[] ) 
{
  int i;
  
  fprintf(sf, "# Command Line:\n#    ");
  for( i = 0; i < argc; i++ ) {
    fprintf(sf, "%s ", argv[i] );
  }
  fprintf(sf, "\n#\n");
   
  fprintf(sf, "# Random Seed = %d\n", random_seed );
  fprintf(sf, "# Number of sessions produced = %d\n", session_count);
  fprintf(sf, "# Avg number of file requests per session = %6.2lf\n", 
          (double) totalFiles / (double) session_count);

  fprintf(sf, "# Total number of file requests = %8.4f\n#\n", totalFiles );
  
  fprintf(sf, "# File Distributions\n");
  writeDist(sf, fileDist, filePDF, MAX_FILE+1, tolerance);
  fprintf(sf, "# \n");

  fprintf(sf,"# Class Distributions\n");
  writeDist(sf, classDist, classPDF, MAX_CLASS+1, tolerance);
  fprintf(sf, "# \n");
  
  fprintf(sf, "# Directory Distributions\n");
  writeDist(sf, directoryDist, dirPDF, num_dirs, tolerance);
  fprintf(sf, "# \n");
  
  fprintf(sf, "# Session Distributions\n");
  writeDist(sf, sessionDist, sessionPDF, MAX_SESSION+1, tolerance);
  fprintf(sf, "\n");
  fflush(sf);
}


void writeDist(FILE * sf, double *dist, double *pdf, int size, double epsilon)
{
  double percent = 0.0;
  int i;
  
  fprintf(sf, "# %5s %8s %10s %10s %10s %5s\n",
          "Index", "Count", "Actual%", "Target%", "Diff%", "Met");
  for (i=0; i<size; i++) {
    if (dist == sessionDist) {
      percent = dist[i] * 100.0 / (double) session_count;
    } else {
      percent = dist[i] * 100.0 / (double) totalFiles;
    }
    fprintf(sf, "# %5d %8.0lf %10.6lf %10.6lf %10.6lf %5s\n", 
            i, dist[i], percent, pdf[i] * 100.0, 
            fabs(percent - (pdf[i] * 100.0)), 
            tolerance_met(dist, pdf, i, epsilon) ? "Yes" : "No");
  }
}

int main(int argc, char *argv[])
{
  int i;
  int file = 0;
  int class = 0;
  int dir = 0;
  char logfile[MAX_FILE_CHARS];
  FILE *fp;   /* For the log file */
  FILE * sf;  /* For the statistics file */
  int done = 0;
  int max_sessions = -1;
  int min_sessions = 0;
  int session_len = 0;
  double think = 0.0;
  int do_clear = 0;
  int ch;
  int dist_met = 0;
  int session_dist_met = 0;
  int user_seed = 0;
  char catCmd[128];
  
  
  progname = argv[0];

  while ((ch = getopt(argc, argv, "ch?Hd:m:pr:s:S:t:T")) != EOF) {
    switch(ch) {
    case 'c':
      do_clear = 1;
      break;
      
    case 'd':
      num_dirs = atoi(optarg);
      printf("Limiting directories to a maximum of %d\n", num_dirs);
      if (max_size != -1) {
        printf("Can't specify both maximum directory and maximum size\n");
        exit(1);
      }
      break;
      
    case 'p':
      prime_cache = 1;
      printf("Generating log to prime cache\n");
      if (max_sessions != -1) {
        printf("Can't use both prime the cache and specify the number of sessions\n");
        usage();
        exit(1);
      }
      max_sessions = 1;
      printf("Using one session\n");
      break;
      
    case 'r':
      user_seed = 1;
      random_seed = (unsigned int) atoi(optarg);
      break;

    case 's':
      min_sessions = atoi(optarg);
      printf("Generating a minimum of %d sessions\n", min_sessions);
      if (prime_cache) {
        printf("Can't use both prime the cache and specify "
               "the number of sessions\n");
        usage();
        exit(1);
      }
      break;
      
    case 'S':
      max_sessions = atoi(optarg);
      printf("Generating a maximum of %d sessions\n", max_sessions);
      if (prime_cache) {
        printf("Can't use both prime the cache and specify "
               "the number of sessions\n");
        usage();
        exit(1);
      }
      break;
      
    case 'm':
      max_size = atoi(optarg);
      printf("Limiting file set size to a maximum of %d MB\n", max_size);
      if (num_dirs != -1) {
        printf("Can't specify both maximum directory and maximum size\n");
        exit(1);
      }
      num_dirs = (int) floor((double) (max_size * 1024 * 1024) / 
                            (double) (BYTES_PER_DIR));
      printf("Done by limiting directories to a maximum of %d\n", num_dirs);
      printf("Using num_dirs = %d gives size of %d = %.2lf MB\n", 
             num_dirs, num_dirs * BYTES_PER_DIR,
             (double) (num_dirs * BYTES_PER_DIR) / 1024.0 / 1024.0);
      break;

    case 't':
      tolerance = atof(optarg);
      printf("Ensuring that distributions are within a factor of %1.1f\n", 
              tolerance);
      break;
      
    case 'T':
      use_think_times = 1;
      printf("Turning think times on\n");
      printf("Currently not implemented\n");
      exit(1);
      break;
      
    case 'h':
    case '?':
    case 'H':
      usage();
      exit(1);
      
    default:
      printf("Unrecognized option %c\n", ch);
      usage();
      exit(1);
    }
  }

  if (num_dirs == -1) {
    printf("Must specify either maximum directory or maximum size\n");
    usage();
    exit(1);
  }

  if (max_sessions == -1) {
    max_sessions = INT_MAX;
  }

  if (prime_cache) {
    if (do_clear) {
      strcpy(logfile, PRIME_CLEAR_LOGFILE_NAME);
      printf("logfile = %s\n", logfile);
    } else {
      strcpy(logfile, PRIME_LOGFILE_NAME);
      printf("logfile = %s\n", logfile);
    }
  } else {
    strcpy(logfile, LOGFILE_NAME);
    printf("logfile = %s\n", logfile);
  }

  fp = fopen(logfile, "w");
  if (fp == NULL) {
    perror("fopen failed ");
    exit(1);
  }
  

  // Seed the random number generator and initialize the tuple-tracking arrays
  if (!user_seed) {
    random_seed = (unsigned int) time(NULL);
  } 
  srand(random_seed);
  init();
  
  while (session_count <= max_sessions && !done) {
    
    session_count++;
    if (session_count > 1) {
      fprintf(fp, "\n");
    }

    /* if session_len is 1 then it really should be a HTTP/1.0 request */
    if (prime_cache) {
      session_len = num_dirs * FILES_PER_DIR;
    } else {
      session_len = get_session_len();
    }

    fprintf(fp, "# Session %d\n", session_count);

    for (i=0; i<session_len; i++) {
      if (prime_cache) {
        get_file_class_dir_prime(&file, &class, &dir, num_dirs);
        think = 0.0;
      } else {
        get_file_class_dir(&file, &class, &dir, num_dirs);
        think = get_think_time();
      }

      fprintf(fp, "%s%05d/class%1d_%1d think=%f\n", 
        REQUESTDIR, dir, class, file, think);

      ldbg("session_count %6d dir %5d class %d file %d\n",
      session_count, dir, class, file);
      
      dist_met = distribution_add_and_check(file, class, dir, tolerance);
    }

    /* Need to check the session distribution */
    session_dist_met = session_add_and_check( session_len, tolerance);
    if (session_dist_met && dist_met && (session_count > min_sessions)) {
         done = 1;
    }
  }

  printf("\n");
  if (! (dist_met && session_dist_met)) {
    printf("WARNING: distribution tolerances have not been met\n\n");
  }
  
  if (dist_met && session_dist_met && (session_count > min_sessions)) {
    printf("Program terminated because distribution tolerances and minimum "
           "sessions were met\n\n");
  }
  
  if (session_count > max_sessions) {
    printf("Program terminated because max_sessions was exceeded\n\n");
    
  }
  
  /* Write statistics to the beginning of the generated log file */
  sf = fopen(STATSFILE, "w");
  if (sf == NULL) {
    perror("Unable to open stats file. No statistics produced\n");
    exit(1);
  }

  if (do_clear) {
    fprintf(fp, "esc_stats_print\n");
    fprintf(fp, "esc_stats_clear\n");
  }

  writeStatsFile(sf, argc, argv);
  
  /* Close files and free up memory */
  fclose(sf);
  fclose(fp);
  
  snprintf(catCmd, 128, "/bin/cat %s %s > %s; /bin/mv %s %s; /bin/rm -f %s", STATSFILE, logfile, TEMPFILE, TEMPFILE, logfile, STATSFILE);
  // ldbg("catCmd = [%s]\n", catCmd);
  system(catCmd);  

  writeStatsFile(stdout, argc, argv );


  free( dirCDF );
  free( dirPDF );
  free( directoryDist );
  exit(0);
}

/* Generate a value according to the given CDF */
void 
genValue( double * CDF, int size, int * value ) 
{
  int i;
  double random;

  random = ((double) rand()) / ((double) RAND_MAX);
  for( i = 0; i < size; i++ ) {
    if( random <= CDF[i] ) {
      *value = i;
      return;
    }
  }
  *value = (size - 1);
}

/* Generate a (file,class,dir) tuple that conforms to the probability distributions
   used in the SpecWeb99 benchmarks
*/
void get_file_class_dir(int *file, int *class, int *dir, int num_dirs ) {
  
  genValue(fileCDF, (MAX_FILE+1), file );
  genValue(classCDF, (MAX_CLASS+1), class);
  genValue(dirCDF, num_dirs, dir );
}


void get_file_class_dir_prime(int *file, int *class, int *dir, int num_dirs)
{
  static int last_file = 0;
  static int last_class = 0;
  static int last_dir = 0;

  if (last_file > MAX_FILE) {
    last_file = 0;
    last_class++;
  }

  if (last_class > MAX_CLASS) {
    last_class = 0;
    last_dir++;
  }

  if (last_dir > num_dirs) {
    last_dir = 0;
  }

  *file = last_file;
  *class = last_class;
  *dir = last_dir;

  last_file++;
}


int get_session_len()
{
  int session_len = 0;
  double random;
  random = ((double) rand()) / ((double) RAND_MAX);
  
  if( random <= 0.3 ) {
    // HTTP 1.0 session with just 1 request per connection
    session_len = 1;
  } else {
    // HTTP 1.1 session with multiple requests per connection
    random = (((double) rand()) / ((double) RAND_MAX)) * 11.0;
    session_len = floor(5.0 + random);
  }
  return session_len;
}

double get_think_time()
{
  return(0.0);
}


/* We've just created a request for a this file and class.
 * Track the current distribution and when we have reached the
 * target distribtion for all files and all classes we've got
 * a set of sessions that meets the requirements.
 *
 * When we've met the requirements return 1.
 * Otherwise return 0.
 *
 */

/* Want to ensure that:
 *   sessions of length = 1    = 30%
 *   sessions of length = 5-15 = 70%
 *   sessions of length 5 - 15 are roughly uniform (average is 10)
 *
 *   class 0  = 35 %
 *   class 1  = 50 %
 *   class 2  = 14 %
 *   class 3  =  1 %
 *
 *   file  0  =  3.9 %
 *   file  1  =  5.9 %
 *   file  2  =  8.8 %
 *   file  3  = 17.7 %
 *   file  4  = 35.3 %
 *   file  5  = 11.8 %
 *   file  6  =  7.1 %
 *   file  7  =  5.0 %
 *   file  8  =  4.4 %
 */ 
int distribution_add_and_check(int file, int class, int dir, double epsilon) 
{
  int i;

  if( file <= MAX_FILE && class <= MAX_CLASS ) {
    totalFiles++;
    fileDist[file]++;
    classDist[class]++;
    directoryDist[dir]++;
    
    /* Check the file distribution */
    for( i = 0; i <= MAX_FILE; i++ ) {
      if (!tolerance_met(fileDist, filePDF, i, epsilon)) {
        ldbg("distribution_add_and_check: doesn't meet file distribution tolerances\n");
			  return 0;
      }
    }
    
    /* Now check the class distribution */
    for( i = 0; i <= MAX_CLASS; i++ ) {
      if (!tolerance_met(classDist, classPDF, i, epsilon))  {
        ldbg("distribution_add_and_check: doesn't meet class distribution tolerances\n");
				return 0;
      }
    }
    
    /* Now check the directory number distribution*/
    for( i = 0; i < num_dirs; i++ ) {
      if (!tolerance_met(directoryDist, dirPDF, i, epsilon)) {
        ldbg("distribution_add_and_check: doesn't meet directory distribution tolerances\n");
	      return 0;
      }
    }

    ldbg("distribution_add_and_check: all distribution tolerances met\n");
    return 1;

  } else {
    assert(0);
  }
  return 0;
}


/* Check the distribution of the generated sessions */
int session_add_and_check(int session_len, double epsilon ) {
  int i;

  ldbg("session_add_and_check: len = %d\n", session_len);
  assert(session_len > 0);
  assert(session_len != 2);
  assert(session_len != 3);
  assert(session_len != 4);
  assert(session_len <= MAX_SESSION);
  
  sessionDist[session_len]++;

  for( i = 0; i <= MAX_SESSION; i++ ) {      
    if( sessionPDF[i] > 0.0 ) {
        if (!tolerance_met(sessionDist, sessionPDF, i, epsilon)) return 0;
      }
    }
    
  ldbg("session_add_and_check: session distribution met\n");
  return 1;
}

int tolerance_met(double *dist, double *pdf, int index, double epsilon)
{
  int ret_val = 0;
  double fraction = 0.0;
  double value = 0.0;
 
  if (dist == sessionDist) {
    fraction = dist[index] / session_count;
  } else {
    fraction = dist[index] / totalFiles;
  }

	if (pdf[index] == 0.0) {
		if (dist[index] == 0.0) {
      ret_val = 1;    
		} else {
      ret_val = 0;    
		}
  } else {
	  value = fabs(pdf[index] - fraction) / pdf[index];

    if (value > epsilon) {
      ret_val = 0;    
    } else {
      ret_val = 1;    
    }
	}
  ldbg("tolerance_met: index = %d dist = %lf pdf = %lf value = %lf epsilon = %lf ret_val = %d\n", 
			  index, dist[index], pdf[index], value, epsilon, ret_val);
  return ret_val;
}
