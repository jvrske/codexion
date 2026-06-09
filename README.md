*This project has been created as part of the 42 curriculum by csilva.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. Coders (threads) sit in a circular co-working hub and compete for limited USB dongles to compile quantum code. Each coder must acquire two dongles simultaneously to compile, then debug and refactor before repeating the cycle.

The simulation ends when all coders reach the required number of compiles, or when any coder burns out from lack of access to resources.

Key challenges: deadlock prevention, starvation avoidance, precise burnout detection, and fair dongle scheduling via FIFO or EDF policies.

## Instructions

### Compilation

    make all

### Usage

    ./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler

### Arguments

| Argument | Description |
|---|---|
| number_of_coders | Number of coders and dongles |
| time_to_burnout | Max ms a coder can go without starting a compile |
| time_to_compile | Ms to hold both dongles and compile |
| time_to_debug | Ms spent debugging after compile |
| time_to_refactor | Ms spent refactoring after debug |
| number_of_compiles_required | Simulation ends when all coders reach this count |
| dongle_cooldown | Ms a dongle is unavailable after being released |
| scheduler | fifo (first in, first out) or edf (earliest deadline first) |

### Example

    ./codexion 4 800 200 200 200 5 100 fifo

### Makefile rules

    make all    - build
    make clean  - remove object files
    make fclean - remove object files and binary
    make re     - full rebuild

## Blocking Cases Handled

### Deadlock prevention (Coffman's conditions)
To break the circular wait condition, the last coder acquires dongles in reverse order (right before left). All other coders acquire left then right. This eliminates the circular dependency that would cause all coders to hold one dongle and wait forever for the other.

### Starvation prevention
All dongle requests are queued in a per-dongle priority heap. Under FIFO, requests are served in arrival order. Under EDF, the coder with the earliest burnout deadline is served first, with a deterministic tie-breaker by coder ID. This guarantees no coder is indefinitely skipped.

### Cooldown handling
After a dongle is released, it remains unavailable for dongle_cooldown milliseconds. Coders use pthread_cond_timedwait with a 1ms timeout so they re-check availability as soon as the cooldown expires, without relying on an external broadcast.

### Precise burnout detection
A dedicated monitor thread checks all coders every 1ms. If a coder has not started compiling within time_to_burnout ms since their last compile (or since simulation start), the burnout is logged and the simulation stops. The 1ms polling interval ensures the burnout log appears within the required 10ms window.

### Log serialization
All state messages are protected by a print_mutex. Only one thread can write at a time, preventing any interleaving of log lines.

## Thread Synchronization Mechanisms

### pthread_mutex_t
- print_mutex: serializes all log output. Every call to log_state locks this mutex before writing and unlocks immediately after.
- state_mutex: protects shared simulation state: sim->running, coder->last_compile, and coder->compiles_done. All accesses by coder threads and the monitor are wrapped in lock/unlock pairs.
- Per-dongle mutex: each dongle has its own mutex protecting held, release_time, and the priority queue.

### pthread_cond_t
Each dongle has a pthread_cond_t for its waiting queue. When a coder cannot acquire a dongle, it calls pthread_cond_timedwait (1ms timeout) under the dongle mutex. When a dongle is released, pthread_cond_broadcast wakes all waiting coders; only the coder at the top of the priority heap proceeds.

### Custom priority queue (min-heap)
Each dongle maintains a min-heap of pending requests. Each entry stores the coder ID, request timestamp, and EDF deadline. Under FIFO, the earliest request_time wins; under EDF, the earliest deadline wins with coder_id as a deterministic tie-breaker.

### Race condition prevention examples
- coder->last_compile is written by the coder thread and read by the monitor. Both accesses are protected by state_mutex.
- dongle->held and dongle->release_time are written by dongle_release and read by dongle_blocked. Both happen under the dongle mutex.
- sim->running is written by the monitor and read by all coder threads via is_running(), which always acquires state_mutex.

## Resources

### References
- POSIX Threads Programming: https://hpc-tutorials.llnl.gov/posix/
- The Dining Philosophers Problem: https://en.wikipedia.org/wiki/Dining_philosophers_problem
- Earliest Deadline First Scheduling: https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling
- pthread_cond_timedwait man page: https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html

### AI Usage
Claude (Anthropic) was used exclusively for educational purposes — to understand concurrency concepts, POSIX thread primitives, heap data structures, and scheduling algorithms before and during implementation. All code was written by the author.