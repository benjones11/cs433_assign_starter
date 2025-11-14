#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>   // for sleep/usleep
#include <cstdlib>
#include <ctime>
/**
* Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Benjamin Jones, Anshool Yuvera 
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */
#include <iostream>
#include "buffer.h"
#include <unistd.h>

using namespace std;

// global buffer object
Buffer buffer;

//adding semaphores and mutex
pthread_mutex_t buffermutex;
sem_t bufferempty; 
sem_t bufferfull;

// Producer thread function
void *producer(void *param) {
    // Each producer insert its own ID into the buffer
    // For example, thread 1 will insert 1, thread 2 will insert 2, and so on.
    buffer_item item = *((int *) param); //Getting producer ID from param 

    while (true) {
        /* sleep for a random period of time */
        usleep(rand()%1000000); //simulating realistic asnychronus behavior
        if (buffer.insert_item(item)) {
            cout << "Producer " << item << ": Inserted item " << item << endl;
            buffer.print_buffer();
        } else {
            cout << "Producer error condition"  << endl;    // shouldn't come here
        }
    }
    return nullptr;
}
// Consumer thread function

void *consumer(void *param) {
    buffer_item item;

    while (true) {
        /* sleep for a random period of time */
        usleep(rand() % 1000000);

        if (buffer.remove_item(&item)) {
            cout << "Consumer Removed item " << item << endl;
            buffer.print_buffer();
        } else {
            cout << "Consumer error condition" << endl;    // shouldn't come here
        }
    }
    return nullptr;
}

int main(int argc, char *argv[]) {

    if (argc != 4) { //checking for number of arguments 
        cerr << "Usage: " << argv[0] << " <sleep time> <Producers> <Consumer>" << endl;
        return 1;
    }
//Whatever is passed in the command line will determine the number per variable down below 
    int sleepTime = atoi(argv[1]); //using atoi to convert string to int
    int numProducers = atoi(argv[2]);
    int numConsumers = atoi(argv[3]);

    //validating input 
    if (sleepTime < 0 || numProducers <= 0 || numConsumers <= 0) { 
        cerr << "All arguments must be positive integers." << endl;
        return 1;
    }

    srand(time(NULL)); // Seed the random number generator


    pthread_mutex_init(&buffermutex, nullptr);
    sem_init(&bufferempty, 0, buffer.get_size()); // initializing empty slots
    sem_init(&bufferfull, 0, 0);// initializing full slots to 0

    pthread_t *producers = new pthread_t[numProducers]; //array of producer threads
    int *producerIDs = new int[numProducers]; //each producer gets its own ID
    for (int i = 0; i < numProducers; i++) {
        producerIDs[i] = i + 1; // IDs start from 1
        pthread_create(&producers[i], nullptr, producer, &producerIDs[i]);
    }

     pthread_t *consumers = new pthread_t[numConsumers];
    int *consumerIDs = new int[numConsumers];
    for (int i = 0; i < numConsumers; i++) {
        consumerIDs[i] = i + 1; // IDs start from 1
        pthread_create(&consumers[i], nullptr, consumer, &consumerIDs[i]);
    }

    sleep(sleepTime);

    exit(0);
}
