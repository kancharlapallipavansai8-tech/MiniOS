#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int worker_id;
    int start;
    int end;
} TaskRange;

void *sum_range(void *arg)
{
    TaskRange *range = (TaskRange *)arg;
    long *partial_sum = malloc(sizeof(long));
    if (!partial_sum)
    {
        perror("malloc");
        pthread_exit(NULL);
    }

    *partial_sum = 0;
    for (int i = range->start; i <= range->end; i++)
    {
        *partial_sum += i;
    }

    printf("[Worker %d] Computed sum(%d..%d) = %ld\n",
           range->worker_id, range->start, range->end, *partial_sum);

    /* Return heap pointer to main thread */
    return (void *)partial_sum;
}

int main(void)
{
    pthread_t t1, t2;
    TaskRange r1 = {1, 1, 500};
    TaskRange r2 = {2, 501, 1000};

    pthread_create(&t1, NULL, sum_range, &r1);
    pthread_create(&t2, NULL, sum_range, &r2);

    void *res1 = NULL;
    void *res2 = NULL;

    pthread_join(t1, &res1);
    pthread_join(t2, &res2);

    long total = *(long *)res1 + *(long *)res2;
    printf("\nCombined Total (1..1000) = %ld (Expected: 500500)\n", total);

    free(res1);
    free(res2);
    return 0;
}
