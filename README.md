*This project has been created as part of the 42 curriculum by odschreu.*

# Codexion

## Description

Codexion is a concurrency simulation written in C with POSIX threads. A group of coders sits around a circular table with one USB odngle between each neighbour. To compile, a coder needs both the dongle on their left and the one on their right. After compiling they debug, then refactor, then try to compile again. If a coder goes longer than `time_to_burnout` milliseconds without starting a new compile, they burn out and the simulation ends.

The goal is to share a limited set of resources between threads without deadlock, starvation or data races, while respecting a per-dongle cooldown and a configurable arbitration policy (`fifo` or `edf`). The simulation ends either when a coder burns out or when every coder has compiled at least `number_of_compiles_required` times.

## Instructions


### Compilation
 
```sh
make        # builds ./codexion
make clean  # removes object files
make fclean # removes object files and the binary
make re     # full rebuild
```
 
The project compiles with `cc -Wall -Wextra -Werror -pthread`.
 
### Usage
 
```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```
 
All arguments are mandatory. Times are in milliseconds. `scheduler` must be exactly `fifo` or `edf`. Negative numbers, non-integers, and any other scheduler value are rejected with an error message.
 
### Examples
 
```sh
./codexion 4 800 200 200 200 5 10 edf    # should run to completion, no burnout
./codexion 5 600 200 200 200 7 0 fifo    # tight but feasible
./codexion 4 310 200 100 100 5 0 edf     # a coder should burn out
./codexion 1 800 200 200 200 5 0 fifo    # single coder: one dongle, must burn out
```

## Blocking cases handled


- deadlock prevention and Coffman's conditions

- starvation prevention
- cooldown handling
- precise burnout detection
- log serialiazation

## Thread synchronization mechanisms

- explaining the specific threading primitives used in your implementation (pthread_mutex_t, pthread_cond_t, custom event implementation) and how they coordinate access to shared resources (dongles, looging, monitor state). include examples of how race conditions are prevented and how thread-safe communication is achieved between coders and the monitor

## Resources



---


https://www.geeksforgeeks.org/c/heap-in-c/
https://notes.lukasl.dev/Knowledge/Coffman-Conditions
https://en.wikipedia.org/wiki/Dining_philosophers_problem
https://takeuforward.org/data-structure/introduction-to-priority-queues-using-binary-heaps
https://www.youtube.com/watch?v=HqPJF2L5h9U
