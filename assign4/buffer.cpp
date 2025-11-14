#include <iostream>
#include <pthread.h>
#include "buffer.h"
#include <semaphore.h>
#include <unistd.h>   // for sleep/usleep
#include <iostream>
using namespace std;
/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Benjamin Jones, Anshool Yuvera 
 * @brief Implementation file for the buffer class
 * @version 0.1
 */
extern pthread_mutex_t buffermutex;
extern sem_t bufferempty; 
extern sem_t bufferfull;
#include "buffer.h"

/**
 * @brief Construct a new Buffer object
 * * @param size the size of the buffer
 *
 * 
 */
Buffer::Buffer(int size){
    if (size > BUFFER_SIZE){
        this->size = BUFFER_SIZE;
    } else {
        this->size = size;
    }
    count = 0; 
    in = 0; 
    out = 0; 

for (int i = 0; i < BUFFER_SIZE; i++){
    buffer[i] = 0; //initializing buffer items to 0 
}

}

/**
 * @brief Destroy the Buffer object
 */ 
Buffer::~Buffer(){
    // No dynamic memory to free
}

/**
 * @brief Insert an item into the buffer
 * @param item the item to insert
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::insert_item(buffer_item item){

    //Waiting for an empty slot in the buffer
    sem_wait(&bufferempty);
    //Ensures exclusive access to the buffer
    pthread_mutex_lock(&buffermutex);
    bool success = true; 

    if (!is_full()){ 
    buffer[in] = item; // Insert item
    in = (in + 1) % size; // Update the input index
    count++; //update count 
} else{
    success = false; //buffer is full 
}
//unlocking mutex 
pthread_mutex_unlock(&buffermutex);
//signaling that a new item has been added
sem_post(&bufferfull);
return success;
}

/**
 * @brief Remove an item from the buffer
 * @param item the item to remove
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::remove_item(buffer_item *item){

    sem_wait(&bufferfull);
    pthread_mutex_lock(&buffermutex);

    bool success = true; 
    if (!is_empty()){
    *item = buffer[out]; // Remove item
    out = (out + 1) % size; // Update out index
    count--; // Decrement count
    } else{
success = false; //buffer is empty
    }
    pthread_mutex_unlock(&buffermutex);
    sem_post(&bufferempty);
    return success;
}


/**
 * @brief Get the size of the buffer
 * @return the size of the buffer
 */
int Buffer::get_size(){
    return size;
}
/**
 * @brief Get the number of items in the buffer
 * @return the number of items in the buffer
 */
int Buffer::get_count(){
    return count;
}
/**
 * @brief Chceck if the buffer is empty
 * @return true if the buffer is empty, else false
 */
bool Buffer::is_empty(){
    return count == 0;
}
/**
 * @brief Check if the buffer is full
 * @return true if the buffer is full, else false
 */
bool Buffer::is_full(){
    return count == size;
}
/**
 * @brief Print the buffer
 */
void Buffer::print_buffer(){
    cout << "Buffer: [";
    for (int i = 0; i < count; i++){
        cout << buffer[(out + i) % size];
        if (i < count - 1){
            cout << ", ";
    }
    cout << "]" << endl;
}
}
