#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <sstream>

#define NUM_PHILOSOPHERS 5

void test(int id); // Forward declaration for test function

// Global variables
pthread_mutex_t mutex;
pthread_cond_t conditions[NUM_PHILOSOPHERS];
enum State { THINKING, HUNGRY, EATING };
State states[NUM_PHILOSOPHERS];
pthread_mutex_t print_mutex;

// Macro to calculate left and right neighbors
#define LEFT(id) ((id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS)
#define RIGHT(id) ((id + 1) % NUM_PHILOSOPHERS)

// Thread-safe printing function
void safe_print(const std::string &msg) {
    pthread_mutex_lock(&print_mutex);
    std::cout << msg << std::endl;
    pthread_mutex_unlock(&print_mutex);
}

// Generalized simulation function for thinking/eating
void simulate_activity(const std::string &activity, int id) {
    int duration = (rand() % 3 + 1) * 1000; // Random time between 1000ms and 3000ms
    auto start = std::chrono::high_resolution_clock::now();
    usleep(duration * 1000); // Convert ms to microseconds
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    safe_print("Philosopher #" + std::to_string(id) + " took " + std::to_string(elapsed) + "ms " + activity + ".");
}

// Pickup forks (enter critical section)
void pickup_forks(int id) {
    pthread_mutex_lock(&mutex);
    states[id] = HUNGRY;
    test(id); // Check if philosopher can eat
    while (states[id] != EATING)
        pthread_cond_wait(&conditions[id], &mutex);
    pthread_mutex_unlock(&mutex);
}

// Return forks (exit critical section)
void return_forks(int id) {
    pthread_mutex_lock(&mutex);
    states[id] = THINKING;

    // Log fork release
    safe_print("\tFork #" + std::to_string(id) + " is with Philosopher #" + std::to_string(id));
    safe_print("\tFork #" + std::to_string((id + 1) % NUM_PHILOSOPHERS) +
               " is with Philosopher #" + std::to_string((id + 1) % NUM_PHILOSOPHERS));

    // Test neighbors
    test(LEFT(id));
    test(RIGHT(id));
    pthread_mutex_unlock(&mutex);
}

// Test if a philosopher can start eating
void test(int id) {
    if (states[id] == HUNGRY &&
        states[LEFT(id)] != EATING &&
        states[RIGHT(id)] != EATING) {
        states[id] = EATING;
        pthread_cond_signal(&conditions[id]); // Signal philosopher to start eating
    }
}

// Philosopher thread function
void* philosopher(void* num) {
    int id = *(int*)num;
    
    while (true) {
        simulate_activity("thinking", id); // Think
        pickup_forks(id);                 // Try to pick up forks
        simulate_activity("eating", id);  // Eat
        return_forks(id);                 // Return forks
    }
    
    return nullptr;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    srand(time(nullptr)); // Seed random number generator

    // Initialize mutexes and condition variables
    pthread_mutex_init(&mutex, nullptr);
    pthread_mutex_init(&print_mutex, nullptr);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&conditions[i], nullptr);
        states[i] = THINKING; // All philosophers start in THINKING state
        ids[i] = i;
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&threads[i], nullptr, philosopher, &ids[i]);
    }

    // Join threads (this will never happen as philosophers loop infinitely)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}
