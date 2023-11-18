#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000000
#define NUM_THREADS 32

int *array;
int length;
int *private_counts;
pthread_t threads[NUM_THREADS];

void *count1s(void *thread_id)
{
    int start = (*((int *)thread_id) * (length / NUM_THREADS));
    int end = start + (length / NUM_THREADS);
    int local_count = 0;

    for (int i = start; i < end; i++)
    {
        if (array[i] == 1)
        {
            local_count++;
        }
    }

    private_counts[*((int *)thread_id)] = local_count;

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
    // Allocate memory for private counts
    private_counts = (int *)malloc(NUM_THREADS * sizeof(int));

    // Create threads
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

    // Calculate the total count by combining private counts
    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        total_count += private_counts[i];
    }

    // Print the total count
    printf("Total count: %d\n", total_count);
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

    printf("counting ones took %f seconds to execute \n", time_taken);
    // Clean up
    free(array);
    free(private_counts);

    return 0;
}