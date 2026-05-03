*This project has been created as part of the 42 curriculum by advacher.*

## Description
Codexion is a multithreading and synchronization simulation written in C. It models a shared workspace where multiple coders must coordinate to use limited resources (USB dongles). Each coder alternates between compiling, debugging, and refactoring. To compile their quantum code, a coder must acquire two dongles simultaneously. The simulation's objective is to manage resource synchronization efficiently to prevent data corruption, deadlocks, and process starvation (burnout).

## Instructions
To compile the project, run `make` at the root of the repository.

Execute the program with the following mandatory arguments:
`./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler`

**Note:** The `scheduler` argument must be strictly set to either `fifo` or `edf`.

## Thread synchronization mechanisms
This project relies on POSIX threads (`pthreads`) and synchronization primitives to manage concurrent access to shared resources:

* **Threads (`pthread_create`)**: Each coder is represented by an individual thread. An independent monitor thread is also created to oversee the simulation's state.
* **Mutexes (`pthread_mutex_t`)**: Used to ensure exclusive access to shared resources. 
  * Each dongle is protected by its own mutex to prevent concurrent access by multiple coders. 
  * A `sim_mutex` protects the global simulation variables (like the start flag and the stop condition).
  * A `print_mutex` serializes the standard output to ensure that log messages are printed cleanly and never interleave on a single line.
* **Condition Variables (`pthread_cond_t`)**: Used for efficient thread synchronization without busy-waiting.
  * A global `start_cond` acts as a synchronization barrier: all coder threads are put to sleep upon creation and are awakened simultaneously via `pthread_cond_broadcast` by the main thread to ensure a fair start.
  * Condition variables are also used to put coders to sleep while they wait in the queue for a requested dongle.

## Blocking cases handled
It is crucial to understand the difference between how coders request dongles, and how the simulation grants them.

* **Deadlock Prevention (Coffman's conditions)**: To prevent circular wait (where coders hold one dongle and indefinitely wait for the second), the program implements an Even/Odd acquisition strategy.

  * Odd-numbered coders always attempt to lock their left dongle first. 
  * Even-numbered coders lock their right dongle first. This guarantees that at least one coder can acquire both dongles, entirely avoiding deadlocks even with a large number of coders.

* **Scheduling**: When multiple coders request the same dongle, the dongle grants access based on the chosen arbitration policy (`scheduler`):
  * `fifo`: The dongle is granted to the coder whose request arrived first.
  * `edf` (Earliest Deadline First): The dongle is granted to the coder closest to their burnout deadline.
* **Cooldown Handling**: Once a coder finishes compiling and releases their dongles, the dongles are marked as unavailable until the mandatory `dongle_cooldown` duration has passed.
* **Burnout Detection**: The dedicated monitor thread runs in a continuous loop to check every coder's timestamps. If it detects that a coder has missed their compile deadline (`now - last_compile_start > time_to_burnout`), it immediately flags the simulation to stop, broadcasts a signal to wake up any sleeping threads for a clean exit, and prints the burnout log within the required 10 ms precision.

## Resources
* Unix `pthread` documentation (man pages for mutexes and condition variables).
* **Multithreading in C:** [CodeVault - Pthreads & Multithreading Courses](https://www.youtube.com/@CodeVault/courses)
* **AI Usage**: Artificial Intelligence was used to brainstorm deadlock prevention strategies, specifically transitioning from an ID-based hierarchy to the Even/Odd acquisition algorithm, and to review the English phrasing of this README.

## Tests and Analysis of the Simulation 
To validate the robustness of the multithreaded architecture and the scheduling algorithm, several test scenarios were executed. Below is the analysis of the simulation's behavior under these constraints.

### 1. Cooldown Period Enforcement
* **Objective:** Verify that a released dongle is not immediately reusable and strictly respects the imposed cooldown period.
* **Command:** `./codexion 2 500 100 0 0 2 200 fifo`
* **Observed Result:** Coder 1 takes the dongles at time `0` and finishes compiling at time `100`. Coder 2 only acquires the dongles at timestamp `300`.
* **Analysis:** The 200ms cooldown period is perfectly respected. Access to the dongles is properly locked during this transitional period, proving that the `ft_get_time() < dongle->available_at` condition works as intended.
* **Why it burns out:** Coder 1 starves because they must wait for the long 200ms cooldown to finish on both turns before they can compile a second time, exceeding their 500ms lifespan.

### 2. Burnout Detection (Monitor Precision)
* **Objective:** Ensure the monitor thread accurately detects a timeout (burnout) and cleanly stops the simulation without waiting for other ongoing actions to finish.
* **Command:** `./codexion 3 150 200 0 0 1 0 fifo`
* **Observed Result:** A coder begins compiling (duration 200ms). Since the survival time (burnout limit) is set to 150ms, Coder 1 outputs `151 1 burned out`.
* **Analysis:** The monitor detected the timeout down to the millisecond (151ms). It immediately halted the simulation, cleanly interrupting the running threads.(Coder 3 did not finish its 200ms compilation).
* **Why it burns out:** Survival is mathematically impossible because a single compilation takes longer (200ms) than the total time allowed to live (150ms).

### 3. Scheduler Comparison (FIFO vs. EDF)
* **Objective:** Demonstrate the impact of the scheduling algorithm on resource allocation order.
* **Compared Commands:** `./codexion 3 300 100 0 0 3 10 fifo` and `./codexion 3 300 100 0 0 3 10 edf`
* **Observed Result:** 
  * In **FIFO** mode, the initial dongle allocation order is: Coder 1, Coder 3, Coder 2.
  * In **EDF** mode, the initial order dynamically changes to: Coder 3, Coder 1, Coder 2.
* **Analysis:** Changing the scheduler alters the flow of the simulation even with identical parameters. The EDF (Earliest Deadline First) algorithm overrides the basic arrival order to prioritize coders based on the urgency of their next burnout.
* **Why it burns out:** With 3 coders and 3 dongles, only one coder can eat at a time. The cumulative wait time forces the last coder in the rotation to exceed their 300ms limit.

### 4. Multithreading Safety (Deadlock Prevention)
* **Objective:** Prove the structural prevention of deadlocks (circular waiting) under conditions of extreme resource contention.
* **Command:** `valgrind --tool=helgrind ./codexion 10 5000 1 1 1 100 0 edf`
* **Observed Result:** Thousands of mutex locks and unlocks occur per second. The simulation does not freeze and successfully reaches completion for all 1000 required compilations. Additionally, running this under `valgrind --tool=helgrind` confirms with the final report: `ERROR SUMMARY: 0 errors from 0 contexts`.
* **Analysis:** The absence of artificial delays (`0` cooldown, `1`ms action times) forces the threads into maximum collision. The fact that the simulation successfully completes without hanging proves that the asymmetric lock logic makes deadlocks impossible, regardless of thread execution speed.

### 5. Rigorous Memory Management
* **Objective:** Verify that no memory leaks occur, even in the event of a premature simulation stop.
* **Executed Commands:**
  1. Natural finish: `valgrind -s --leak-check=full ./codexion 4 600 100 50 50 2 10 edf`
  2. Premature finish: `valgrind -s --leak-check=full ./codexion 3 50 200 0 0 1 0 fifo`
* **Observed Result:** In both cases, Valgrind confirms: `All heap blocks were freed -- no leaks are possible`.
* **Analysis:** The cleanup module is robust. Regardless of why the program stops, all dynamically allocated memory structures (`malloc`) are freed, and all Mutexes are cleanly destroyed before the process terminates.

### 6. Full Survival (Simulation Finished)
* **Objective:** Demonstrate a scenario where resources and timings allow all threads to complete their required tasks without any burnouts.
* **Command:** `./codexion 4 800 200 50 50 2 10 edf`
* **Observed Result:** All 4 coders successfully complete their 2 required compilations. The program exits naturally without any `burned out` messages.
* **Analysis:** With 4 dongles available, two coders can compile simultaneously. Combined with a generous survival time (800ms) and dynamic EDF scheduling, the resource rotation is fast enough for everyone to survive and trigger the intended `SIM_FINISHED` state.

## Project Architecture
Below is a visual representation of how the different components of **Codexion** interact:
```mermaid
graph TD
    %% Define Styles
    classDef thread fill:#e1f5fe,stroke:#03a9f4,stroke-width:2px;
    classDef resource fill:#fff3e0,stroke:#ff9800,stroke-width:2px;
    classDef monitor fill:#ffebee,stroke:#f44336,stroke-width:2px;
    classDef action fill:#f1f8e9,stroke:#8bc34a,stroke-width:1px;

    %% Main Process
    subgraph Main_Process ["Main Thread"]
        A["Initialization & Parsing"] --> B["Create Monitor Thread"]
        A --> C["Create Coder Threads"]
        B -.Waiting.-> Barrier(("Start Barrier<br/>is_ready + cond_wait"))
        C -.Waiting.-> Barrier
        A -->|"Broadcast Signal (GO!)"| Barrier
    end

    %% Monitor Thread
    subgraph Monitor_Thread ["Monitor Thread"]
        Barrier --> M1["Monitoring Loop"]
        M1 --> M2{"Check Status"}
        M2 -->|"now - last_compile > burnout"| M3["Burnout Detected!"]
        M2 -->|"All required compiles done"| M4["Simulation Finished"]
        M2 -->|"All good"| M1
        M3 --> M5["Set stop_simulation = true"]
        M4 --> M5
        M5 --> M6["Broadcast & Wake up all coders"]
        M6 --> M7["End Monitor"]
    end

    %% Coder Thread
    subgraph Coder_Thread ["Coder Thread x N"]
        Barrier --> C1{"stop_simulation == true?"}
        C1 -->|Yes| C_End["Clean Thread Exit"]
        C1 -->|No| C2["ft_take_dongles"]
        
        %% The New Logic: Deadlock Prevention Strategy
        C2 --> C3{"Is Coder ID Even or Odd?"}
        C3 -->|Odd ID| C3_Odd["1. Lock Left Dongle<br/>2. Lock Right Dongle"]
        C3 -->|Even ID| C3_Even["1. Lock Right Dongle<br/>2. Lock Left Dongle"]
        
        %% The Scheduler Queue
        C3_Odd -.->|"Wait in Queue (FIFO or EDF)"| Shared_Dongles
        C3_Even -.->|"Wait in Queue (FIFO or EDF)"| Shared_Dongles
        
        Shared_Dongles -.->|Access Granted| C4["Compiling"]
        
        C4 --> C5["ft_drop_dongles"]
        C5 -.->|"Update Cooldown & Release"| Shared_Dongles
        
        C5 --> C6["Debugging"]
        C6 --> C7["Refactoring"]
        C7 --> C1
    end

    %% Shared Resources (Mutexes)
    subgraph Shared_Resources ["Shared Resources"]
        Shared_Dongles[("Dongles Array<br/>Mutex + Wait Queue")]
        Sim_Mutex[("sim_mutex<br/>Protects flags & timestamps")]
        Print_Mutex[("print_mutex<br/>Log serialization")]
    end

    %% Interactions with Shared Resources
    C4 -.->|"Update last_compile"| Sim_Mutex
    M2 -.->|"Read last_compile"| Sim_Mutex
    
    C4 -.->|"printf 'is compiling'"| Print_Mutex
    C6 -.->|"printf 'is debugging'"| Print_Mutex
    C7 -.->|"printf 'is refactoring'"| Print_Mutex
    M3 -.->|"printf 'burned out'"| Print_Mutex

    %% Apply Styles (CORRECTED)
    class A,B,C thread;
    class M1,M2,M3,M4,M5,M6,M7 monitor;
    class C1,C2,C3,C3_Odd,C3_Even,C4,C5,C6,C7,C_End action;
    class Shared_Dongles,Sim_Mutex,Print_Mutex resource;