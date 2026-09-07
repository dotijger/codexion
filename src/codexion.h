// #MY42HEADER HERE

#include <stdio.h>
// for printf, fprintf
#include <unistdio.h>
// for write, usleep
#include <pthread.h>
// for mutex: _init, _lock, _unlock, _destroy
// for threads: _create, _join
// for cond: _init, _wait, _timedwait, _signal, _broadcast, _destroy
#include <stdlib.h>
// for malloc, free, atoi
#include <string.h>
// for strcmp, strlen, memset
#include <sys/time.h>
// for gettimeofday
#include <stdbool.h>
#include <stdint.h>
// for booleans and uint64_t and INT_MAX

typedef struct s_data t_data;

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

// list of function definitions
uint64_t get_current_time_ms(void);
