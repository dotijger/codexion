/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   codexion.h                                      \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 13:20:40 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/10 18:13:11 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // for printf, fprintf
#include <unistd.h> // for write, usleep
#include <pthread.h> // for mutex: _init, _lock, _unlock, _destroy
					// for threads: _create, _join
					// for cond: _init, _wait, _timedwait, _signal, _broadcast, _destroy
#include <stdlib.h> // for malloc, free, atoi
#include <string.h> // for strcmp, strlen, memset
#include <sys/time.h> // for gettimeofday
#include <stdbool.h>
#include <stdint.h> // for booleans and uint64_t and INT_MAX
#include <limits.h>

/*
 * Global macros:
 * ANSI colors for fprintf
 * DEBUG_MODE (default = 0, if 1 will print more extensive logging)
*/

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

#define DEBUG_MODE 0

/*
 * ENUM structures (for getting/setting and safe handling of threads/mutexes --> avoiding the overwriting errors by lock/unlock) as well as time
 */

typedef pthread_mutex_t t_mtx;
typedef pthread_cond_t t_cond;

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
	DETACH,
	WAIT,
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

  t_mtx dongle_mtx;
  t_cond dongle_cond;
  int dongle_id;
  bool	taken;
  bool	cooling_down;
  long	release_time_in_ms;


} t_dongle;

typedef struct s_coder {

  int id;
  int number_of_compiles;
  bool done;
  long last_compile_start;
  long time_to_burnout;
  t_dongle *left_dongle;
  t_dongle *right_dongle;
  pthread_t thread_id;
  t_mtx	coder_mtx;
  t_data *data;

}		t_coder;

typedef struct s_data {

  long number_of_coders;
  long time_to_burnout;
  long time_to_compile;
  long time_to_debug;
  long time_to_refactor;
  long number_of_compiles_required;
  long dongle_cooldown;
  char *scheduler;
  bool start;
  bool quit;
  t_mtx read_mtx;
  t_mtx	write_mtx;
  t_coder *coders;
  t_dongle *dongles;

}		t_data;

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


// utils.c
long	get_time(t_time_format time_code);
void	error_exit(char *exit_msg);

// parser.c
void	parse_input(t_data *data_table, char **av);

// init.c
void	codexion_init(t_data *data_table);

// safe_utils.c
void	*safe_malloc(size_t size);
void	safe_mutex_handle(t_mtx *mtx, t_pthread_code code);
void	safe_thread_handle(pthread_t *thread, void *(*start_routine)(void *), void *data, t_pthread_code code);
void	safe_cond_handle(t_cond *cond, t_mtx *mtx, t_pthread_code code);

// codexion.c
void	codexion_start(t_data *data_table);
