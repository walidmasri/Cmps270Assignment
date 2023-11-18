#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000000
#define NUM_THREADS 32
int *array;
int length;
int count = 0;
pthread_mutex_t count_mutex;
// each thread maintains its own local_count, and after processing its portion of the array,
// it updates the global count under the protection of a mutex to avoid race conditions.
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

    pthread_mutex_lock(&count_mutex);
    count += local_count;
    pthread_mutex_unlock(&count_mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = atoi(argv[1]);

    // Initialize array with random values between 0 and 5
    length = ARRAY_SIZE;
    array = (int *)malloc(length * sizeof(int));
    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 6;
    }

    // Initialize mutex
    pthread_mutex_init(&count_mutex, NULL);

    // Create threads
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    for (int i = 0; i < num_threads; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, count1s, (void *)&thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the total count
    printf("Total count: %d\n", count);

    // Clean up
    free(array);
    pthread_mutex_destroy(&count_mutex);

    return 0;
}