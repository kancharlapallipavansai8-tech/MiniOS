#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 12

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg)
{
    (void)arg;
    for (int item = 1; item <= TOTAL_ITEMS; item++)
    {
        pthread_mutex_lock(&buf_mutex);

        while (count == BUFFER_SIZE)
        {
            printf("  [Producer] Buffer FULL (%d/%d). Waiting...\n", count, BUFFER_SIZE);
            pthread_cond_wait(&not_full, &buf_mutex);
        }

        buffer[in] = item * 10;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("[Producer] Produced item %d (buffer count: %d/%d)\n", item * 10, count, BUFFER_SIZE);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&buf_mutex);
        usleep(80000);
    }
    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;
    for (int i = 1; i <= TOTAL_ITEMS; i++)
    {
        pthread_mutex_lock(&buf_mutex);

        while (count == 0)
        {
            printf("  [Consumer] Buffer EMPTY (0/%d). Waiting...\n", BUFFER_SIZE);
            pthread_cond_wait(&not_empty, &buf_mutex);
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("[Consumer] Consumed item %d (buffer count: %d/%d)\n", item, count, BUFFER_SIZE);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&buf_mutex);
        usleep(150000);
    }
    return NULL;
}

int main(void)
{
    pthread_t prod_tid, cons_tid;

    printf("=== Chapter 7.3: Producer-Consumer Bounded Buffer ===\n\n");

    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    printf("\nAll %d items produced and consumed safely!\n", TOTAL_ITEMS);
    return 0;
}