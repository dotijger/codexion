/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   codexion.h                                      \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 13:20:40 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/17 16:21:42 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // for printf, fprintf
#include <unistd.h> // for write, usleep
#include <pthread.h> // for mutex: _init, _lock, _unlock, _destroy
					// for threads: _create, _join
					// for cond: _init, _wait, _timedwait, _signal, _broadcast, _destroy
#include <stdlib.h> // for malloc, free, atoi
#include <string.h> // for strcmp, strlen, memset
#include <sys/time.h> // for gettimeofday, struct timespec / timeval
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

/*
 * ENUM structures (for getting/setting and safe handling of threads/mutexes --> avoiding the overwriting errors by lock/unlock) as well as time
 */

typedef pthread_mutex_t t_mtx;
typedef pthread_cond_t t_cond;

typedef enum	e_scheduler
{
	FIFO,
	EDF,

}	t_scheduler;

typedef enum	e_time_format
{
	SECOND,
	MILLISECOND,
	MICROSECOND,

}		t_time_format;

typedef struct	s_request
{
	long	arrival_time;
	long	deadline_time;
	int		coder_id;

}	t_request;

typedef struct	s_heap
{
	t_request	*queue;
	int			size;
	int			capacity;
	int			(*cmp)(t_request, t_request);

}	t_heap;

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

  int		dongle_id;
  bool		taken;
  long		release_time_in_ms;

} t_dongle;

typedef struct s_coder {

  int 		coder_id;
  int 		compiles;
  long		last_compile_start;
  long 		burnout_deadline;
  t_dongle	*left;
  t_dongle	*right;
  t_coder	*left_rival;
  t_coder	*right_rival;
  pthread_t thread;
  t_data 	*data_table;

}		t_coder;

typedef struct s_data {

  long		number_of_coders;
  long 		time_to_burnout;
  long 		time_to_compile;
  long 		time_to_debug;
  long 		time_to_refactor;
  long 		compiles_required;
  long 		dongle_cooldown;
  char 		*scheduler;
  long		start_time;
  pthread_t	monitor;
  bool 		running;
  t_cond	dongle_cond;
  t_cond	monitor_cond;
  t_mtx 	log_mtx;
  t_mtx		table_mtx;
  t_mtx		dongle_mtx;
  t_coder 	*coders;
  t_dongle 	*dongles;
  t_heap	*heap;

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
void	error_exit(char *exit_msg);
void	log_event(t_data *data_table, int id, char *event);
long	get_time(e_time_format time_code);
void	precise_usleep(long usec, t_data *data_table);
int		left(int i, int n);
int		right(int i, int n);

// coder_routine_utils.c
void	debug(int i, t_data *data_table, long time_to_debug);
void	refactor(int i, t_data *data_table, long time_to_refactor);
void	compile(int i, t_data *data_table, long time_to_compile);
void	acquire_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);

// dongle_utils.c
bool	dongle_ready(t_dongle *a, long cooldown_ms);
bool	my_turn(t_heap *heap, t_coder *coder, t_coder *rival);
void	new_request(t_coder *coder, t_heap *heap);

// routine.c
void	coding_routine(void *arg);
void	monitor_routine(void *arg);

// heap.c
void 		init_heap(t_heap **heap, int capacity, int (*cmp)(t_request, t_request));
int			get_heap_index(t_heap *heap, int coder_id);
void		insert(t_heap *heap, t_request request);
t_request	*pop(t_heap *heap);
void		remove_at_index(t_heap *heap, int index);

// scheduler.c
int	fifo_cmp(t_request a, t_request b);
int	edf_cmp(t_request a, t_request b);

// safe_utils.c
void	*safe_malloc(size_t size);
void	safe_mutex_handle(t_mtx *mtx, t_pthread_code code);
void	safe_thread_handle(pthread_t *thread, void *(*start_routine)(void *), void *data, t_pthread_code code);
void	safe_cond_handle(t_cond *cond, t_mtx *mtx, timespec time, t_pthread_code code);

// parser.c
void	parse_input(t_data *data_table, char **av);

// init.c
void	codexion_init(t_data *data_table);

// run.c
void	codexion(t_data *data_table);
