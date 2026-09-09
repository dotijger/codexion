// #MY42HEADER HERE

#include <stdio.h> // for printf, fprintf
#include <unistdio.h> // for write, usleep
#include <pthread.h> // for mutex: _init, _lock, _unlock, _destroy
					// for threads: _create, _join
					// for cond: _init, _wait, _timedwait, _signal, _broadcast, _destroy
#include <stdlib.h> // for malloc, free, atoi
#include <string.h> // for strcmp, strlen, memset
#include <sys/time.h> // for gettimeofday
#include <stdbool.h>
#include <stdint.h> // for booleans and uint64_t and INT_MAX

/*
 * Global macros:
*/

#DEFINE DEBUG_MODE 0

/*
 * ENUM structures (for getting/setting and safe handling of threads/mutexes --> avoiding the overwriting errors by lock/unlock) as well as time
 */

typedef pthread_mutex_t t_mtx;

typedef enum e_time_format
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}		t_time_format;

typedef enum e_pthread_code
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	CREATE,
	JOIN,
	WAIT,
	TIMEDWAIT,
	SIGNAL,
	BROADCAST,
}		t_pthread_code;

/*
 * For the structures:
 * 		t_data (data table to pass around functions)
 * 			- *coders (array of pthreads of each coder)
 * 			- *dongles (array of the dongles between the coders)
 * 			- monitor (thread for the monitor)
 * 		t_dongle
 * 		t_coder
*/

typedef struct s_data t_data; // IOU for the compiler (aka = we define this later)

typedef struct s_dongle {

  pthread_mutex_t dongle;
  int dongle_id;

} t_dongle;

typedef struct s_coder {

  int id;
  int number_of_compiles;
  bool done;
  uint64_t last_compile_start;
  uint64_t time_to_burnout;
  t_dongle *left_dongle;
  t_dongle *right_dongle;
  pthread_t thread_id;
  t_data *data;

} t_coder;

typedef struct s_data {

  long number_of_coders;
  long time_to_burnout;
  long time_to_compile;
  long time_to_debug;
  long time_to_refactor;
  long number_of_compiles_required;
  long dongle_cooldown;
  char *scheduler;
  long start;
  bool quit;
  t_coder *coders;
  t_dongle *dongles;

} t_data;

/*
 * For the functions:
 * 		safe getting and setting and mutex/thread handling (using mutex)
 * 		init (to initialize the coder and dongle structs and the main data struct)
 * 		parsing
 * 		utils (getting time, a precise usleep, error_exit, clean up function, writing status)
 * 		synchro_utils (synchronization of the monitor with the start of the simulation -> once all coders are ready / threads have been created)
 *		monitor
 *		codexion
 *
*/


// list of function definitions
uint64_t get_current_time_ms(void);
