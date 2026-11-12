#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

/* Shared global variable (accessible by all threads in the process) */
int shared_total = 0;

void *worker_routine(void *arg)
{
    int thread_id = *(int *)arg;
    /* Local variable lives on each thread's private stack */
    int local_work = (thread_id + 1) * 10;

    printf("[Thread %d] Started. Stack var local_work=%d (addr=%p) | Global shared_total addr=%p\n",
           thread_id, local_work, (void *)&local_work, (void *)&shared_total);

    sleep(1); /* Simulate work */

    shared_total += local_work;
    printf("[Thread %d] Finished adding %d.\n", thread_id, local_work);

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    printf("=== Chapter 6: POSIX Threads Intro ===\n");
    printf("Main Process PID: %d\n\n", getpid());

    /* 1. Spawn threads */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, worker_routine, &thread_ids[i]) != 0)
        {
            perror("pthread_create");
            return 1;
        }
    }

    /* 2. Wait for all threads to complete (like waitpid for processes) */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nAll threads joined. Final shared_total = %d (Expected: 100)\n", shared_total);
    return 0;
}