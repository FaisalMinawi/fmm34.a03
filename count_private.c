/*Another way to deal with race conditions is by using private variables that are not shared across
threads, and syncing the results after the threads are done with their local workload. Implement
a third version of this program that introduces an array of private counts that will be accessed
privately by the corresponding threads. This version should not use a mutex. Repeat the same
experiments and include your analysis. Do you notice any performance improvement? Why or why
not?*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int *array;
int count;
int length;
int chunk;
int number_of_threads;
int *private_count;

void *create1s(void *arg) {
    int index = *(int *)arg;
    private_count[index] = 0;
    for (int j = 0; j < chunk; j++)
    {
        if (array[index + j] == 1)
        {
            private_count[index]++;
        }
    }
    printf("Thread's 1s: %d\n", private_count[index]);
    *(int *)arg = private_count[index];
    return arg;
}

// function add integers between 0 and 5 to array* randomly
void add(int length){
    srand(time(NULL));
    for(int i = 0; i < length; i++){
        int random = rand() % 6;
        array[i] = random;
    }

    //print array
    for(int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    printf("enter the length of the array: ");
    scanf("%d", &length);
    array = (int *)malloc(length * sizeof(int));
    private_count = (int *)malloc(length * sizeof(int));
    printf("Main thread started \n");
    printf("Input the number of threads: ");
    scanf("%d", &number_of_threads);
    chunk = length / number_of_threads;
    add(length);
    pthread_t threads[number_of_threads];
    int i;
    for (i = 0; i < number_of_threads; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * (length / number_of_threads);
        if (pthread_create(&threads[i], NULL, create1s, a) != 0)
        {
            perror("Failed to create thread");
        }
    }
    int globalSum = 0;
    for (i = 0; i < number_of_threads; i++)
    {
        int *result;
        if (pthread_join(threads[i], (void **)&result) != 0)
        {
            perror("Failed to join thread");
        }
        globalSum += *result;
        free(result);
    }
    printf("Global sum: %d\n", globalSum);
    free(array);
    printf("Main thread finished\n");
    return 0;
}



