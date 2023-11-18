#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define ARRAY_SIZE 1000000000
#define NUM_THREADS 32

int *array;
int length;
int count = 0;
pthread_mutex_t count_mutex;

void *count1s(void *thread_id)
{
    int start = (*((int *)thread_id) * (length / NUM_THREADS));
    int end = start + (length / NUM_THREADS);

    for (int i = start; i < end; i++)
    {
        if (array[i] == 1)
        {
            pthread_mutex_lock(&count_mutex);
            count++;
            pthread_mutex_unlock(&count_mutex);
        }
    }

    pthread_exit(NULL);
}

int main()
{

    // Initialize array with random values between 0 and 5
    length = ARRAY_SIZE;
    array = (int *)malloc(length * sizeof(int));
    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 6;
    }
    // Calculate the time taken
    clock_t t;
    t = clock();
    // Initialize mutex
    pthread_mutex_init(&count_mutex, NULL);

    // Create threads
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, count1s, (void *)&thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the total count
    printf("Total count: %d\n", count);

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

    printf("counting ones took %f seconds to execute \n", time_taken);

    // Clean up
    free(array);
    pthread_mutex_destroy(&count_mutex);

    return 0;
}