#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 100000

/* Shared global counter WITHOUT synchronization */
long shared_counter = 0;

void *increment_unsafe(void *arg)
{
    (void)arg;
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        /* Non-atomic read-modify-write: causes lost updates! */
        shared_counter++;
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    long expected = (long)NUM_THREADS * INCREMENTS_PER_THREAD;

    printf("=== Chapter 7.1: Unsynchronized Race Condition ===\n");
    printf("Spawning %d threads, each incrementing shared_counter %d times...\n",
           NUM_THREADS, INCREMENTS_PER_THREAD);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, increment_unsafe, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Expected Counter : %ld\n", expected);
    printf("Actual Counter   : %ld\n", shared_counter);
    printf("Lost Updates     : %ld\n", expected - shared_counter);

    return 0;
}