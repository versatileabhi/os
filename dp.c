#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5

pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t forks[NUM_FORKS];

void* philosopher_lifecycle(void* num) {
    int id = *(int*)num;

    // Determine left and right fork indices
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    // Simulate thinking and eating iterations
    for (int i = 0; i < 3; i++) {
        printf("Philosopher %d is thinking.\n", id + 1);
        sleep(1); // Simulating thinking

        printf("Philosopher %d is hungry and trying to pick up forks.\n", id + 1);

        // Deadlock prevention logic: The last philosopher picks up the right fork first.
        if (id == NUM_PHILOSOPHERS - 1) {
            pthread_mutex_lock(&forks[right_fork]);
            pthread_mutex_lock(&forks[left_fork]);
        } else {
            pthread_mutex_lock(&forks[left_fork]);
            pthread_mutex_lock(&forks[right_fork]);
        }

        // Critical Section: Eating
        printf("Philosopher %d is EATING.\n", id + 1);
        sleep(2); // Simulating eating

        // Put down forks
        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);

        printf("Philosopher %d finished eating and put down forks.\n", id + 1);
    }

    free(num);
    return NULL;
}

int main() {
    // Initialize mutexes for each fork
    for (int i = 0; i < NUM_FORKS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&philosophers[i], NULL, philosopher_lifecycle, id);
    }

    // Wait for all philosophers to finish their lifecycles
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < NUM_FORKS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("Simulation completed successfully.\n");
    return 0;
}