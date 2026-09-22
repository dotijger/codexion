/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   codexion.h                                      \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 13:20:40 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/22 11:21:04 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define RED     "\033[31m"
#define RESET   "\033[0m"

typedef pthread_mutex_t t_mtx;
typedef pthread_cond_t t_cond;

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;

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
	long		arrival_time;
	long		deadline_time;
	int			coder_id;

}	t_request;

typedef struct	s_heap
{
	t_request	*queue;
	int			size;
	int			capacity;
	bool		(*cmp)(t_request, t_request);

}	t_heap;

typedef struct s_dongle {

  int			dongle_id;
  bool			taken;
  long			release_time_in_ms;
  t_heap		*heap;
  t_mtx			mtx;
  bool			mtx_success;
  t_cond		cond;
  bool			cond_success;

} t_dongle;

typedef struct s_coder {

  int 			coder_id;
  int 			compiles;
  long			last_compile_start;
  long 			burnout_deadline;
  t_dongle		*left;
  t_dongle		*right;
  t_coder		*left_rival;
  t_coder		*right_rival;
  pthread_t 	thread;
  t_data 		*data_table;

}		t_coder;

typedef struct s_data {

  long			number_of_coders;
  long 			time_to_burnout;
  long 			time_to_compile;
  long 			time_to_debug;
  long 			time_to_refactor;
  long 			compiles_required;
  long 			dongle_cooldown;
  t_scheduler	scheduler;
  long			start_time;
  int			dongles_created;
  int			mtx_created;
  pthread_t		monitor;
  bool 			running;
  bool			failed;
  t_mtx 		log_mtx;
  t_mtx			table_mtx;
  t_mtx			sim_mtx;
  t_coder 		*coders;
  t_dongle 		*dongles;
  t_heap		*heap;

}		t_data;

// clean.c
void			clean_up(t_data *data_table);

// coder_routine_utils.c
void			debug(int i, t_data *data_table, long time_to_debug);
void			refactor(int i, t_data *data_table, long time_to_refactor);
void			compile(int i, t_data *data_table, long time_to_compile);
int				acquire_dongles(t_coder *coder);
void			release_dongles(t_coder *coder);

// dongle.c
int				acquire_dongle(t_coder *coder, t_dongle *dongle);
void			release_dongle(t_dongle *dongle);

// dongle_utils.c
bool			dongle_ready(t_dongle *a, long cooldown_ms);
long			available_at(t_dongle *a, long cooldown_ms);
t_coder			*get_rival(t_coder *coder, t_dongle *a);
bool			my_turn(t_heap *heap, t_coder *coder, t_dongle *a);
int				new_request(t_coder *coder, t_heap *heap);

// heap.c
int 			init_heap(t_heap **heap, bool (*cmp)(t_request, t_request));
int				get_heap_index(t_heap *heap, int coder_id);
int				insert(t_heap *heap, t_request request);
int				remove_at_index(t_heap *heap, int index);
void			print_queue(t_heap *heap);

// heap_utils.c
void			swap(t_request *a, t_request *b);
void			heapify(t_heap *heap, int i);

// init.c
int				codexion_init(t_data *data_table);

// parser.c
int				parse_input(t_data *data_table, char **av);

// routine.c
void			*coding_routine(void *arg);
void			*monitor_routine(void *arg);

// run.c
int				codexion(t_data *data_table);

// scheduler.c
bool			fifo_cmp(t_request a, t_request b);
bool			edf_cmp(t_request a, t_request b);

// sim_utils.c
void			log_event(t_data *data_table, int id, char *event);
void			precise_usleep(long usec, t_data *data_table);
bool			is_running(t_data *data_table);
void			stop_sim(t_data *data_table);
void			fail_sim(t_data *data_table);

// utils.c
int				ft_strncmp(const char *s1, const char *s2, int n);
int				fail(char *exit_msg);
long			get_time(t_time_format time_code);
struct timespec	ms_to_ts(long ms);
