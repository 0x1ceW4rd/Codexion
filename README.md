*This project has been created as part of the 42 curriculum by aezzirar.*

## Description

**Codexion** is a multithreaded resource scheduling simulation project that models a competitive race scenario inspired by the classical "Dining Philosophers" problem. In this simulation, multiple coders compete to acquire shared compilation dongles before they burn out due to missing their compilation deadlines.

### Goal

The primary objective is to explore and implement sophisticated concurrent programming techniques to:
- Prevent race conditions in accessing shared resources (dongles)
- Implement deadlock prevention through resource ordering
- Manage thread synchronization and fair resource allocation
- Support multiple scheduling policies (FIFO and Earliest Deadline First)
- Detect and handle coder burnout (failure to compile before deadline)

### Overview

The simulation features:
- **Coders**: Independent threads that cycle through compile → debug → refactor phases
- **Dongles**: Shared compilation resources that must be acquired in pairs
- **Monitor**: A dedicated thread that oversees system state and detects burnout
- **Scheduling Policies**: FIFO and EDF (Earliest Deadline First) algorithms for fair resource allocation
- **Cooldown Mechanism**: Dongles enter a cooldown period after use to prevent resource hoarding

## Instructions

### Compilation

To compile the project, simply run:

```bash
make
```

This will compile all source files and produce the executable named `codexion`. Additional make targets:
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Clean and rebuild

### Execution

Run the simulation with the following syntax:

```bash
./codexion <num_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <num_compiles_required> <dongle_cooldown> <scheduler>
```

**Parameters:**
- `<num_coders>` - Number of coder threads to spawn
- `<time_to_burnout>` - Time (ms) before a coder burns out if no progress is made
- `<time_to_compile>` - Time (ms) required to compile
- `<time_to_debug>` - Time (ms) required to debug
- `<time_to_refactor>` - Time (ms) required to refactor
- `<num_compiles_required>` - Number of successful compilations required per coder (0 = infinite)
- `<dongle_cooldown>` - Cooldown period (ms) for dongles after use
- `<scheduler>` - Scheduling algorithm: `fifo` or `edf`

**Example:**

```bash
./codexion 4 800 200 100 50 3 50 edf
```

This runs 4 coders with 800ms burnout time, requiring 3 compilations each, using EDF scheduling.

### Output

The simulation prints events in real-time with the following format:

```
<elapsed_time> <coder_id> <action>
```

Example events:
- `1234 1 has taken a dongle`
- `1456 1 is compiling`
- `1678 1 is debugging`
- `1834 1 is refactoring`
- `2100 2 burned out`

## Blocking Cases Handled

The Codexion implementation addresses all critical concurrency challenges:

### 1. Deadlock Prevention (Coffman's Conditions)

The implementation eliminates deadlock by preventing multiple Coffman conditions:
- **Resource Ordering**: Coders always acquire dongles in a consistent circular order (left → right). This prevents the circular wait condition by ensuring all threads attempt resource acquisition in the same order.
- **No Hold-and-Wait**: While waiting for a dongle to become available, coders release and re-acquire the main state mutex periodically through `pthread_cond_wait()`, preventing indefinite resource hoarding.

### 2. Starvation Prevention

- **Priority Scheduling**: Heap-based queue system using two algorithms:
  - **FIFO**: Requests prioritized by sequence number for fairness
  - **EDF (Earliest Deadline First)**: Prioritizes coders with the earliest deadline, then breaks ties with sequence number
- **Fairness Guarantee**: Each dongle maintains an independent priority queue, ensuring coders waiting longer or with earlier deadlines eventually get access
- **Condition Variables**: `pthread_cond_broadcast()` ensures all waiting threads are notified when resources become available

### 3. Cooldown Handling

After a coder releases dongles, a `cooldown_until` timestamp prevents immediate re-acquisition:
- Coders check `now < dongle->cooldown_until` before attempting acquisition
- This prevents hot-spot contention and gives other coders fair access time
- The cooldown period is precisely enforced using `usleep()` with microsecond granularity

### 4. Precise Burnout Detection

The Monitor thread performs exact burnout detection without race conditions:
- Checks if `now - last_compile_start >= time_to_burnout` under mutex protection
- Skips coders that have already completed their required compilations
- Sets `stop_flag` atomically and broadcasts to wake all waiting threads
- Burnout detection runs every 2ms to catch failures within tight timing windows

### 5. Log Serialization

All output is protected by `print_mutex` to prevent garbled concurrent writes:
- `pthread_mutex_lock(&sim->print_mutex)` before `printf()`
- Ensures timestamp, coder_id, and message print atomically
- Maintains readable, chronological simulation logs

## Thread Synchronization Mechanisms

### Primitives Used

The implementation employs standard POSIX threading primitives:

#### 1. `pthread_mutex_t` - Mutual Exclusion Locks

**`state_mutex`**: Protects the core simulation state
- **Protected Data**: `coders[]`, `dongles[]`, `stop_flag`, `finished_coders`, `req_seq`, `start_time`
- **Usage**:
  ```c
  pthread_mutex_lock(&sim->state_mutex);
  coder->last_compile_start = now;
  pthread_mutex_unlock(&sim->state_mutex);
  ```
- **Prevents Race Conditions**: Multiple threads cannot simultaneously modify burnout timestamps or resource state

**`print_mutex`**: Serializes log output
- **Purpose**: Ensures thread-safe writes to stdout
- **Usage**: Held during `printf()` operations to prevent interleaving

#### 2. `pthread_cond_t` - Condition Variables

**`state_cond`**: Coordinates state changes between coders and monitor
- **Usage in Coders**:
  ```c
  while (!can_take_dongles(coder, now)) {
      pthread_cond_wait(&sim->state_cond, &sim->state_mutex);
      // Woken when dongles are released or stop_flag is set
  }
  ```
- **Usage in Release**:
  ```c
  pthread_cond_broadcast(&sim->state_cond);  // Wake all waiting coders
  ```
- **Usage in Monitor**:
  ```c
  if (check_coder_burnout(sim)) {
      pthread_cond_broadcast(&sim->state_cond);  // Signal burnout
      break;
  }
  ```
- **Prevents Busy-Waiting**: Coders sleep without CPU consumption until notified

#### 3. Custom Event: Priority Heap Queues

Each dongle maintains a **min-heap priority queue** of pending requests:
```c
typedef struct s_heap {
    t_request  *data;
    int         size;
    int         capacity;
} t_heap;

typedef struct s_request {
    int        coder_id;
    int        seq;
    long long  deadline;
} t_request;
```

**How It Prevents Race Conditions**:
- **Atomic Operations Under Mutex**: All heap operations (`heap_push`, `heap_peek`, `heap_remove`) execute within `state_mutex` protection
- **Ordered Access**: Priority comparison (`is_higher_priority`) ensures deterministic ordering
- **FIFO Tiebreaker**: When deadlines match, sequence numbers ensure FIFO fairness

### Thread-Safe Communication Between Coders and Monitor

#### Dongle Acquisition Flow

1. **Coder enqueues request**:
   ```c
   req.deadline = coder->last_compile_start + sim->time_to_burnout;
   heap_push(&coder->left_dongle->queue, req, sim->scheduler);
   heap_push(&coder->right_dongle->queue, req, sim->scheduler);
   ```
   (Protected by `state_mutex`)

2. **Coder waits for its turn**:
   ```c
   if (can_take_dongles(coder, now)) {
       // Both dongles are available and this coder is at the front
       coder->left_dongle->is_in_use = 1;
       coder->right_dongle->is_in_use = 1;
       // Proceed to compile
   } else {
       wait_for_turn(coder);  // Releases mutex and sleeps
   }
   ```

3. **Coder releases dongles**:
   ```c
   coder->left_dongle->is_in_use = 0;
   coder->left_dongle->cooldown_until = now + sim->dongle_cooldown;
   coder->compiles_count++;
   if (all_coders_finished) {
       sim->stop_flag = 1;
   }
   pthread_cond_broadcast(&sim->state_cond);  // Wake waiting coders
   ```
   (Protected by `state_mutex`)

4. **Monitor checks burnout**:
   ```c
   long long elapsed = now - coder->last_compile_start;
   if (elapsed >= sim->time_to_burnout) {
       sim->stop_flag = 1;
       pthread_cond_broadcast(&sim->state_cond);  // Wake all threads
       break;
   }
   ```
   (Protected by `state_mutex`)

#### Key Synchronization Guarantees

- **Atomicity**: State changes (is_in_use, cooldown_until, stop_flag) are indivisible under mutex
- **Visibility**: Mutex operations provide memory barriers; all threads see consistent state
- **Notification**: `pthread_cond_broadcast()` ensures no thread misses critical updates
- **Precision**: `usleep()` with 1ms granularity enables fine-grained timing control

### Example: Race Condition Prevention

**Without Synchronization** (Buggy):
```c
// Thread A
if (!dongle->is_in_use) {
    dongle->is_in_use = 1;
}

// Thread B (simultaneous)
if (!dongle->is_in_use) {  // Sees false (A hasn't written yet)
    dongle->is_in_use = 1;  // Both think they own the dongle!
}
```

**With Mutex Protection** (Correct):
```c
pthread_mutex_lock(&sim->state_mutex);
if (!dongle->is_in_use && now >= dongle->cooldown_until) {
    dongle->is_in_use = 1;  // Atomic check-and-set
    success = 1;
}
pthread_mutex_unlock(&sim->state_mutex);
```

The mutex ensures the check and assignment happen indivisibly, eliminating the race window.

## Resources

### Documentation

- **POSIX Threads Programming**: https://computing.llnl.gov/tutorials/pthreads/
- **Condition Variables**: https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_cond_wait.html
- **Memory Synchronization**: https://en.wikipedia.org/wiki/Memory_barrier
- **Scheduling Algorithms**:
  - FIFO: https://en.wikipedia.org/wiki/First-come,_first-served_scheduling
  - EDF: https://en.wikipedia.org/wiki/Earliest_deadline_first

### Classical References

- **Dining Philosophers Problem**: Dijkstra, E. W. (1971) - Foundation for deadlock prevention concepts
- **Resource Allocation Graphs**: Classic OS theory on detecting circular dependencies
- **Mutex and Condition Variables**: Butenhof, D. R. (1997) "Programming with POSIX Threads"

### Articles

- **Thread Synchronization Patterns**: https://www.1024cores.net/ (High-performance concurrency)
- **Lock-Free Programming**: https://preshing.com/20120612/an-introduction-to-lock-free-programming/

### AI Usage

AI was utilized for:
- **Code Structure & Organization**: Generating boilerplate threading patterns and file organization
- **Algorithm Implementation**: Heap operations (sift_up, sift_down) and priority comparison logic
- **Documentation**: Drafting technical descriptions of synchronization mechanisms
- **Testing Edge Cases**: Suggesting corner cases for single-coder handling and timing edge cases

AI was NOT used for:
- Core synchronization logic and mutex/condition variable placement (manually implemented for correctness)
- Deadlock prevention strategy (manually designed using resource ordering principle)
- Burnout detection timing precision (carefully hand-tuned)
- Testing and debugging synchronization issues
