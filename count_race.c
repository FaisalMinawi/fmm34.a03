#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//Global Variables
int *array;
int count;
int length;
int chunk;


void* create1s(void* arg) {
    int index = *(int*)arg;
    count = 0;
    for (int j = 0; j < chunk; j++) {
        if(array[index + j] == 1) {
            count++;
        }
    }
    printf("Thread's 1s: %d\n", count);
    *(int*)arg = count;
    return arg;
}

//function add integers between 0 and 5 to array* randomly
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

int main(int argc, char* argv[]) {
    int number_of_threads;
    printf("enter the length of the array: ");
    scanf("%d", &length);
    array = (int *)malloc(length * sizeof(int));
    printf("Main thread started\n");
    printf("Input the number of threads: ");
    scanf("%d", &number_of_threads);
    chunk = length / number_of_threads;
    add(length);
    pthread_t threads[number_of_threads];
    int i;
    for (i = 0; i < number_of_threads; i++) {
        int* a = malloc(sizeof(int));
        *a = i * (length / number_of_threads);
        if (pthread_create(&threads[i], NULL, &create1s, a) != 0) {
            perror("Failed to create thread");
        }
    }
    int globalSum = 0;
    for (i = 0; i < number_of_threads; i++) {
        int* r;
        if (pthread_join(threads[i], (void**) &r) != 0) {
            perror("Failed to join thread");
        }
        globalSum += *r;
        free(r);
    }
    printf("Total 1s: %d\n", globalSum);
    free(array);
    printf("Main thread finished\n");
    return 0;
}


