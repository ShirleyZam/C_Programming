#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h> // thread synchronization 

#define THREAD_NUM 2 // creates constant

sem_t semEmpty; // stores empty slots
sem_t semFull; // stores full slots

pthread_mutex_t mutexBuffer; // ensures one thread at a time can access shared resource

int buffer[10]; // SHARED buffer between consumer and producer (10 positions)
int count = 0; // keeps track of how many items are in the buffer 
// producer increases amount (places in buffer) consumer decreases ammount (consumes data)

void* producer(void* args){ //generates random number 
        while(1){
            int x = rand() % 100;
            sleep(2); // 1 second
            
            sem_wait(&semEmpty); // wait until there is an empty space // semEmpty is at least 1
            // if(count<10){// if and else no longer needed --> semaphore
            pthread_mutex_lock(&mutexBuffer); //access shared data
            buffer[count] = x;  // Add to the buffer 
            count++;
            //} else {
            printf("Produced %d\n", x);
            //}
            pthread_mutex_unlock(&mutexBuffer);
            sem_post(&semFull); // tell the consumer there is now one more full space 
    }
    return NULL;
}

void* consumer(void* args){
    while(1){
        int y; 
        sem_wait(&semFull); // wait until at least theres 1 element in the buffer //semfull reps the count > 0
        pthread_mutex_lock(&mutexBuffer);
        //remove from the buffer 
        // if (count>0){
        //consume
        count--;
        y = buffer[count - 1]; // stack first in last out FIFO
        //  }
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty); // now theres an empty slot --> consumed 
        printf("Got %d\n", y);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char* argv[]){ //argc --> arugument count && argv --> arguement vector both allow to pass comand line arg to [program]
    printf("Program started!\n");
    srand(time(NULL));
    pthread_t th[2]; // creates space for two id threads
    sem_init(&semEmpty, 0, 10); //10 slots and no elements in the beginning
    sem_init(&semFull,0,0); // no elements in the buffer 
    pthread_mutex_init(&mutexBuffer, NULL); //creates a mutex to protect shared buffer --> access to buffer and count 
    int i; 
    for (i = 0; i < THREAD_NUM; i++){ // creates two threads 
        if (i % 2 == 0){ // is i even
            if (pthread_create(&th[i], NULL, &producer, NULL)!= 0){ // create a new thread and have it execute the producer function (NULL is defaukt thread attributes)
                //NULL is being passed to producer --> arg recieves NULL
                perror("Failed to create thread"); // 0 --> success && non-zero --> error
            }
        } else {
             if (pthread_create(&th[i], NULL, &consumer, NULL)!= 0){
                perror("Failed to create thread");
             }
        }
    }

    for (i=0; i < THREAD_NUM; i++){ // loops through the two threads 
        if (pthread_join(th[i], NULL) != 0){ // wait for th[i] to finsih 
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;


}