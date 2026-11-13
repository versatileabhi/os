#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;   // Counts empty spaces
sem_t full;    // Counts filled spaces
pthread_mutex_t mutex;

void *producer(void *arg)
{
    int item;
    for (int i = 1; i <= 10; i++)
    {
        item = i;

        // Wait for an empty space
        sem_wait(&empty);

        // Enter critical section
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;

        // Leave critical section
        pthread_mutex_unlock(&mutex);

        // Increase number of filled spaces
        sem_post(&full);

        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int item;
    for (int i = 1; i <= 10; i++)
    {
        // Wait for an available item
        sem_wait(&full);

        // Enter critical section
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        printf("Consumer consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;

        // Leave critical section
        pthread_mutex_unlock(&mutex);

        // Increase number of empty spaces
        sem_post(&empty);

        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy synchronization objects
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}