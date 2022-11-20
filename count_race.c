#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//Global variables
int *array;
int count;
int length;
int chunk;
int number_of_threads;
float time_spent = 0.0;

//requires: nothing as an argument except the global variables (array, count, length > 0)
//effects: counts the number of 1s in the array
int count1s() //without the use of multiple threads for comparison
{
    int i;
    count = 0;
    for (i = 0; i < length; i++)
    {
        if (array[i] == 1)
        {
            count++;
        }
    }
    return count;
}
// requires: a pointer to an integer as an argument: that is the index of the array where the tread should start from depending on its number
//           pointing to zero or positive integer value
// effects: returns the number of 1s in the array 
void *create1s(void *arg) {
    int index = *(int *)arg;
    count = 0;
    if (index/chunk == number_of_threads - 1) //if the thread is the last one
    {
        for (int j = index; j < length; j++)
        {
            if (array[j] == 1)
            {
                count++;
            }
        }
    }
    else
    {
        for (int j = 0; j < chunk; j++)
        {
            if (array[index + j] == 1)
            {
                count++;
            }
    }
    }
    *(int *)arg = count;
    return arg;
}

// requires: a positive integer as an argument: the length of the array
// effecys: fills the array with random integers between 0 and 5
void add(int length)
{
    srand(time(NULL));
    for (int i = 0; i < length; i++)
    {
        int random = rand() % 6;
        array[i] = random;
    }
}

//requires: none
//effects: runs the program and return 1 if the program runs successfully and 0 otherwise
int program(){

    // Fill the array with random integers between 0 and 5
    add(length);

    //first count the number of 1s without the use of multiple threads
    int true_count = count1s();   


    // Create an array of threads
    pthread_t threads[number_of_threads];
    int i;
    for (i = 0; i < number_of_threads; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * (length / number_of_threads);
        if (pthread_create(&threads[i], NULL, &create1s, a) != 0)
        {
            perror("Failed to create thread");
        }
    }

    // Wait for all threads to finish
    int total_number_of_ones = 0;    
    //start timer 
    clock_t begin = clock();
    for (i = 0; i < number_of_threads; i++)
    {
        int *r;
        if (pthread_join(threads[i], (void **)&r) != 0)
        {
            perror("Failed to join thread");
        }
        total_number_of_ones += *r;
        free(r);
    }
    //end timer
    clock_t end = clock();

    //calculate the average time spent
    time_spent += (float)(end - begin) / CLOCKS_PER_SEC;

    //check if the total number of 1s is equal to the true count
    if (total_number_of_ones == true_count)
    {
        return 1;
    }
    return 0;
}


int main()
{
    // Input the length of the array
    printf("enter the length of the array: ");
    scanf("%d", &length);

    // Allocate memory for the array
    array = (int *)malloc(length * sizeof(int));

    // Input the number of threads
    printf("Main thread started\n");
    printf("Input the number of threads: ");
    scanf("%d", &number_of_threads);

    // Calculate the chunk size that each thread should work on
    chunk = length / number_of_threads;

    //call the program function
    int how_many_times_the_program_worked = 0;

    for (int i = 0; i < 100; i++)
    {
        how_many_times_the_program_worked += program();
    }

    time_spent = time_spent / 100; //on average


    if (how_many_times_the_program_worked == 100)
    {
        printf("The program worked 100 times\n");
    }
    else
    {
        printf("The program did not work %d times\n", 100 - how_many_times_the_program_worked);
    }
    free(array);
    printf("Main thread finished\n");

    printf("Time spent: %f\n", time_spent);
    return 0;
}

