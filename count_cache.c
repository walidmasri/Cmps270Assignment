#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000000
#define NUM_THREADS 32

// Define the cache line size (you may need to adjust this based on your system)
#define CACHE_LINE_SIZE 64

typedef struct
{
    int private_count;
    char padding[CACHE_LINE_SIZE - sizeof(int)];
} ThreadData;

int *array;
int length;
ThreadData *private_counts[NUM_THREADS];
pthread_t threads[NUM_THREADS];

void *count1s(void *thread_data)
{
    ThreadData *data = (ThreadData *)thread_data;
    int start = data->private_count * (length / NUM_THREADS);
    int end = start + (length / NUM_THREADS);
    int local_count = 0;

    for (int i = start; i < end; i++)
    {
        if (array[i] == 1)
        {
            local_count++;
        }
    }

    data->private_count = local_count;

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
    // Allocate aligned memory for private counts
    for (int i = 0; i < NUM_THREADS; i++)
    {
        private_counts[i] = (ThreadData *)malloc(sizeof(ThreadData));
        private_counts[i]->private_count = i;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, count1s, (void *)private_counts[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Calculate the total count
    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        total_count += private_counts[i]->private_count;
        free(private_counts[i]);
    }

    // Print the total count
    printf("Total count: %d\n", total_count);

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

    printf("counting ones took %f seconds to execute \n", time_taken);
    // Clean up
    free(array);

    return 0;
}