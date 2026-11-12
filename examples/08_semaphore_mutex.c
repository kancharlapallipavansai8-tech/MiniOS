#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 100000

long mutex_counter = 0;
long sem_counter = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t *macos_sem = NULL;

void *increment_with_mutex(void *arg)
{
    (void)arg;
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        pthread_mutex_lock(&lock); /* Enter critical section */
        mutex_counter++;
        pthread_mutex_unlock(&lock); /* Exit critical section */
    }
    return NULL;
}

void *increment_with_semaphore(void *arg)
{
    (void)arg;
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        sem_wait(macos_sem); /* P() / wait operation */
        sem_counter++;
        sem_post(macos_sem); /* V() / signal operation */
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    long expected = (long)NUM_THREADS * INCREMENTS_PER_THREAD;

    printf("=== Chapter 7.2: Mutual Exclusion (macOS Adapted) ===\n");

    /* Part A: Using pthread_mutex_t */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, increment_with_mutex, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("[Mutex]     Expected: %ld | Actual: %ld\n", expected, mutex_counter);

    /* Part B: Using macOS Named Semaphore (since sem_init is deprecated on Darwin) */
    sem_unlink("/minios_demo_sem");
    macos_sem = sem_open("/minios_demo_sem", O_CREAT | O_EXCL, 0644, 1);
    if (macos_sem == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, increment_with_semaphore, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("[Semaphore] Expected: %ld | Actual: %ld\n", expected, sem_counter);

    sem_close(macos_sem);
    sem_unlink("/minios_demo_sem");
    pthread_mutex_destroy(&lock);

    return 0;
}