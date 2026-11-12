#include "minios.h"
#include <pthread.h>

#define SYNC_THREADS 4
#define SYNC_ITERS 100000

static long g_unsafe_counter = 0;
static long g_safe_counter = 0;
static pthread_mutex_t g_sync_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *unsafe_worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < SYNC_ITERS; i++)
    {
        g_unsafe_counter++;
    }
    return NULL;
}

static void *safe_worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < SYNC_ITERS; i++)
    {
        pthread_mutex_lock(&g_sync_mutex);
        g_safe_counter++;
        pthread_mutex_unlock(&g_sync_mutex);
    }
    return NULL;
}

void run_sync_demo(void)
{
    pthread_t threads[SYNC_THREADS];
    long expected = (long)SYNC_THREADS * SYNC_ITERS;

    g_unsafe_counter = 0;
    g_safe_counter = 0;

    printf("\n=== MiniOS Synchronization Lab (Chapter 7) ===\n");

    /* 1. Unsynchronized run */
    for (int i = 0; i < SYNC_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, unsafe_worker, NULL);
    }
    for (int i = 0; i < SYNC_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("[Unsynchronized] Expected: %ld | Actual: %ld (Lost: %ld)\n",
           expected, g_unsafe_counter, expected - g_unsafe_counter);

    /* 2. Mutex-protected run */
    for (int i = 0; i < SYNC_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, safe_worker, NULL);
    }
    for (int i = 0; i < SYNC_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("[Mutex Locked]   Expected: %ld | Actual: %ld (Lost: %ld)\n",
           expected, g_safe_counter, expected - g_safe_counter);

    printf("==============================================\n\n");
}