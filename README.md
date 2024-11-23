# Philosophers



## Project Overview

The "Philosophers" project simulates a classical synchronization problem where philosophers sit at a round table, alternating between eating, thinking, and sleeping. The objective is to manage shared resources (forks) effectively to avoid starvation or deadlock while maintaining thread safety. The simulation ends when all philosophers have eaten a specified number of times or when one philosopher dies of starvation.



## Features and Highlights 🌟

1. **Thread Management and Synchronization** 🧵:
   - Each philosopher is represented as a thread, ensuring concurrent execution.
   - Mutexes are employed to protect shared resources such as forks and environment states.

2. **Resource Allocation** 🍴:
   - Philosophers must acquire two forks (mutexes) before eating, modeling a practical resource-sharing challenge.

3. **Dynamic Environment** 🔄:
   - The simulation adapts based on configurable parameters:
     - Number of philosophers.
     - Time to die, eat, and sleep (in milliseconds).
     - Minimum number of meals per philosopher.

4. **Custom Logging** 📝:
   - Real-time logs indicate the philosophers’ actions: eating, sleeping, thinking, or taking forks.
   - Events are timestamped to reflect precise timings and ensure clarity.

5. **Error Handling** 🚨:
   - Comprehensive error-checking for arguments, memory allocation, and mutex initialization.
   - Safeguards to prevent invalid input and undefined behaviors.

6. **Time-Sensitive Logic** ⏱️:
   - Philosophers monitor their last meal time, ensuring they don’t starve.
   - A supervisor thread oversees the simulation, detecting starvation or completing the simulation based on the defined conditions.



## Key Technical Components 🔧

- **Multithreading** 🧵:
  - Designed to efficiently utilize threads for concurrent processes.
  - Thread lifecycle management using `pthread_create`, `pthread_join`, and `pthread_detach`.

- **Mutex Synchronization** 🔒:
  - Ensured safe access to shared resources like forks using mutex locks/unlocks.
  - Implemented additional mutexes for meal tracking, logging, and environment state.

- **Efficient Memory Management** 💾:
  - Allocated resources dynamically for the environment and philosophers.
  - Freed all heap-allocated memory after use, maintaining a zero-leak policy.

- **Precise Timing** ⏰:
  - Used `gettimeofday` to implement millisecond-accurate timing for state changes.
  - Implemented a custom `msleep` function for microsecond-level sleep, allowing smooth synchronization.

- **Custom Utilities**:
  - Included helper functions for string-to-integer conversion, error logging, and thread-safe logging.


## How to Use 🛠️

### Compilation

Use the provided `Makefile` with the following commands:

```bash
make        # Builds the program
make clean  # Removes object files
make fclean # Removes object files and the executable
make re     # Cleans and rebuilds the program
```
### Execution

Run the program with the following parameters:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
Example:

```bash
./philo 5 800 200 200
```
### Behaviour

The simulation runs with the specified parameters, logging philosopher actions in real-time. Philosophers take turns eating, sleeping, and thinking, avoiding starvation and deadlock.

### Example Output 📸

Below is an example screenshot of the program's output:

![Screenshot 2024-11-23 232712](https://github.com/user-attachments/assets/af0957d5-7ab1-42e1-8754-0eb3cf8bb9eb)


## Conclusion 🎯

The Philosophers project demonstrates a well-crafted combination of multithreading, resource synchronization, and error handling. Through the use of mutexes and precise time-sensitive logic, it provides an engaging exploration of concurrent programming principles. 🧵🍴💭
