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

### Deadlock prevention (Coffman's conditions)

A deadlock requires all four Coffman conditions to hold at once: 
1. mutual exclusion
2. hold and wait
3. no preemption
4. circular wait. 

The first and third are inherent to the problem. A dongle can only be plugged into one coder at a time, and a coder cannot have a dongle taken away mid-compile. The design therefore targets the other two.

**Circular wait** is broken by asymmetric acquisition order, following
Dijkstra's classic solution. `assign_order` makes even-numbered coders
request their right dongle first and odd-numbered coders their left dongle
first. Neighbours therefore never both reach for their shared dongle as
their second resource while holding their first. No cycle of "holding one,
waiting for the next" can close around the ring.

**Hold and wait** is reduced by checking both dongles jointly. When a coder
tries to take a dongle, it locks both dongles' mutexes (in the same per-coder
order) and only proceeds if *both* are available to it: its turn in both
queues, and both off cooldown. A coder never holds one dongle while waiting for the other.

**Single coder:** with one coder there is only one dongle. The coder can
never hold two, so it cannot compile. It waits until `time_to_burnout`
elapses and is reported as burned out, instead of hanging or deadlocking
on itself.

## Head-of-line blocking prevention (staggered start)

At startup every coder requests dongles at nearly the same moment, and
all EDF and FIFO deadlines are equal. Priority then depends on thread timing and
coder id, which can form a chain where each coder waits behind a
neighbour who is itself waiting. The whole ring then compiles one coder
at a time instead of in parallel, which can cause burnouts even with
feasible parameters.

To avoid this, even-numbered coders wait briefly before their first
request. Odd-numbered coders are then first in line on all their
dongles, except for the first and last coder -- one of them will block the compile for the other.
That is expected behavior (for example, 5 coders, coder 1 and 5 can't compile at the same time, at most 2 coders can compile at the same time).
After the first round, EDF/FIFO keeps this alternation stable: the coders
who compiled earlier have earlier deadlines or ask for the dongles earlier, and win the next round.

### Starvation prevention

Every dongle owns a priority queue, a binary min-heap implemented from
scratch. Before trying to acquire, a coder pushes a request into the heaps
of both its dongles. A coder may only take a dongle when its request is at
the top of that dongle's heap (`my_turn`). The heap ordering depends on
the scheduler:

- `fifo`: requests are ordered by arrival time, so a waiting coder cannot
  be overtaken indefinitely by a neighbour that keeps coming back.
- `edf`: requests are ordered by deadline
  (`last_compile_start + time_to_burnout`), so the coder closest to burning
  out is served first. Equal deadlines fall back to the FIFO comparison,
  which keeps the policy fully deterministic.

When a coder is granted a dongle, its entry is removed from that heap by
lookup (`get_heap_index`) rather than by blindly popping the root. This
guarantees that the removed entry is always the one belonging to the coder
being served.

A known limitation is that arbitration is local to each dongle. A coder
must win two independent contests, one per neighbour. For that reason,
the two schedulers perform similarly under tight parameters in practice.

### Cooldown handling

Each dongle stores the timestamp of its last release. `dongle_ready`
treats a dongle as usable only once it is not taken and
`now >= release_time + dongle_cooldown`. When a coder whose turn it is is
blocked only by cooldown, it does not spin. It sleeps with
`pthread_cond_timedwait` until the moment the dongle becomes available
(`available_at`), so it wakes up exactly when the cooldown expires, even if
no other thread signals it.

### Precise burnout detection

A dedicated monitor thread loops every ~1 ms. For each coder, it compares
the current time against `last_compile_start + time_to_burnout`. Every coder
starts with the simulation start time as its last compile start, so
burnouts are measured from the true beginning of the simulation. The short
polling interval keeps the gap between the actual burnout and the printed
log well under the 10 ms requirement.

The same loop also checks the completion condition. The simulation ends
once every coder has compiled at least `number_of_compiles_required` times.
A coder's compile count is incremented only after its compile has actually
finished.

### Log serialization

All output goes through a single `log_event` function guarded by a log
mutex. Each line is written while holding that mutex, so two messages can
never interleave. `log_event` also checks whether the simulation is still
running before printing. As a result, all log requests are blocked once the simulation stops, and no printing is possible after burnout has been detected.

## Thread synchronization mechanisms

### Mutexes and conds used

| Primitive | Protects / purpose |
|---|---|
| `pthread_mutex_t` per dongle | `taken` flag, release timestamp, and that dongle's request heap |
| `pthread_cond_t` per dongle | lets waiting coders sleep until the dongle is released or its cooldown ends |
| `sim_mtx` | the shared `running` flag (start gate and stop condition) and error flag |
| `log_mutex` | standard output |
| `table_mutex` | `last_compile_start` and compile count, shared between a coder and the monitor |

### Dongles

A coder never reads or writes a dongle's state without holding that
dongle's mutex. In the joint availability check it holds *both* mutexes, so
the "both available" decision is made on a consistent snapshot. Another
thread cannot take or release either dongle between the check and the
grant. This prevents the race where two neighbours both see a shared
dongle as free and both mark it `taken`.

Waiting uses the standard condition-variable pattern. The predicate is
re-checked in a `while` loop after every wakeup, which handles spurious
wakeups and wakeups meant for someone else. Before sleeping, the coder
releases the second mutex and waits on the first dongle's condition
variable, so it never sleeps while blocking a neighbour. The wait is
always bounded (`pthread_cond_timedwait`). A signal that goes to the other
dongle's condition variable therefore cannot leave a coder asleep forever;
it wakes up at the latest when a cooldown expires and re-evaluates.

On release, the coder sets `taken = false` and records the release time
under the mutex. It then calls `pthread_cond_broadcast` so every waiter
re-checks whether it is now first in the queue.

### Coders and the monitor

Coders and the monitor never communicate directly; they share state
through mutex-protected fields:

- A coder writes its `last_compile_start` (and later its compile count)
  under `table_mtx` The monitor reads the same fields under the same
  mutex, so it never observes a half-updated value.
- The `running` flag is read and written only under `sim_mtx`. Using a
  single mutex for every access removes the race where one thread writes
  under one lock and another reads under a different one, which Helgrind
  flagged during development.
- When the monitor detects a burnout or completion, it sets `running` to
  false under `sim_mtx`. It then broadcasts on every dongle's condition
  variable so blocked coders wake up, see the flag, release their mutexes,
  and exit their routine.

### Start gate

All threads are created before the simulation begins. Each coder waits on
`is_running` until the main thread sets it and records the start time.
Consequently, no coder can begin, and no burnout deadline can be computed,
before a valid start time exists.

### Shutdown

Coder threads are joined first, then the monitor. Only after all threads
are joined are the mutexes and condition variables destroyed and the heaps
and arrays freed. As a result, no thread can touch a destroyed primitive.

## Resources

### Concurrency and deadlocks

- [Dining philosophers problem – Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem): the classic problem Codexion is based on, including Dijkstra's resource-ordering solution.
- [Coffman Conditions – lukasl.dev](https://notes.lukasl.dev/Knowledge/Coffman-Conditions): the four conditions required for a deadlock.
- [Deadlock Prevention – lukasl.dev](https://notes.lukasl.dev/Knowledge/Deadlock-Prevention): strategies for breaking each Coffman condition.
- `man pthread.h` and the individual man pages (`pthread_create`, `pthread_mutex_lock`, `pthread_cond_wait`, `pthread_cond_timedwait`, etc.): the reference for thread, mutex, and condition variable behaviour.

### Scheduling

- [Earliest deadline first scheduling – Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling): the theory behind the `edf` scheduler.

### Heaps and priority queues

- [Binary heap – Wikipedia](https://en.wikipedia.org/wiki/Binary_heap): heap structure, insertion, removal, and heapify.
- [Heap in C – GeeksforGeeks](https://www.geeksforgeeks.org/c/heap-in-c/): array-based heap implementation in C.
- [Introduction to Priority Queues using Binary Heaps – takeUforward](https://takeuforward.org/data-structure/introduction-to-priority-queues-using-binary-heaps): using a heap as a priority queue.
- [Video explanation of heaps (YouTube)](https://www.youtube.com/watch?v=HqPJF2L5h9U)

### Peers and community

- Discussions with fellow Codam students about their interpretation of the subject spec, because some things were not completely clear / were left open to the interpretation of the coder.
- Write-ups by other 42 students documenting their implementations of Philosophers, on Medium, GitHub, and YouTube.

### Use of AI

AI (Claude) was used as a discussion and debugging partner throughout the project:

- **Design discussion:** reasoning about deadlock prevention, hold-and-wait, lock ordering, and condition variable usage.
- **Debugging:** tracing logs and code to locate bugs such as argument-parsing errors, data races on shared flags, missed wakeups, and a faulty EDF comparison. Fixes were written and verified by me with Valgrind/Helgrind and repeated test runs.
- **Documentation:** drafting and structuring parts of this README, which I then reviewed and edited.
- **Interpretation:** discussing certain interpretation of the subject specification.

All code in the repository was written and understood by odschreu (me).