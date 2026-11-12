#include "minios.h"
#include <pthread.h>

#define MAX_WORKERS 16

typedef struct
{
    int id;
    int tasks_completed;
} WorkerContext;

static void *minios_worker_thread(void *arg)
{
    WorkerContext *ctx = (WorkerContext *)arg;
    printf("  [Worker %d] Active on thread ID %p | Processing batch...\n",
           ctx->id, (void *)pthread_self());

    /* Simulate computational work */
    usleep(200000 * ctx->id);
    ctx->tasks_completed = ctx->id * 5;

    printf("  [Worker %d] Done (%d tasks completed).\n",
           ctx->id, ctx->tasks_completed);
    return NULL;
}

int run_workers(int count)
{
    if (count < 1 || count > MAX_WORKERS)
    {
        printf("Usage: workers <count> (choose between 1 and %d)\n", MAX_WORKERS);
        return -1;
    }

    pthread_t threads[MAX_WORKERS];
    WorkerContext contexts[MAX_WORKERS];

    printf("\n[MiniOS Threads] Spawning %d concurrent worker threads in PID %d...\n",
           count, getpid());

    for (int i = 0; i < count; i++)
    {
        contexts[i].id = i + 1;
        contexts[i].tasks_completed = 0;

        if (pthread_create(&threads[i], NULL, minios_worker_thread, &contexts[i]) != 0)
        {
            perror("pthread_create");
            return -1;
        }
    }

    int total_tasks = 0;
    for (int i = 0; i < count; i++)
    {
        pthread_join(threads[i], NULL);
        total_tasks += contexts[i].tasks_completed;
    }

    printf("[MiniOS Threads] All %d workers joined. Total tasks processed: %d\n\n",
           count, total_tasks);
    return 0;
}