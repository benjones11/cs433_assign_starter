#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>   // for sleep/usleep
#include <iostream>
/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.h
 * @author Benjamin Jones, Anshool Yuvera  
 * @brief header file for the buffer class
 * @version 0.1
 */

#ifndef ASSIGN4_BUFFER_H
#define ASSIGN4_BUFFER_H

// Define the data type of the buffer items
typedef int buffer_item;
#define BUFFER_SIZE 5 // default buffer size

/**
 * @brief The bounded buffer class. The number of items in the buffer cannot exceed the size of the buffer.
 */
class Buffer {
private:
buffer_item buffer[BUFFER_SIZE]; // the buffer array
    int size;           // the size of the buffer
    int count;          // the number of items in the buffer
    int in;             // the index of the next item to insert
    int out;            // the index of the next item to remove

public:
    /**
     * @brief Construct a new Buffer object
     * @param size the size of the buffer
     */
    Buffer(int size = BUFFER_SIZE);

    /**
     * @brief Destroy the Buffer object
     */
    ~Buffer();

    /**
     * @brief Insert an item into the buffer
     * @param item the item to insert
     * @return true if successful
     * @return false if not successful
     */
    bool insert_item(buffer_item item);

    /**
     * @brief Remove an item from the buffer
     * @param item the item to remove
     * @return true if successful
     * @return false if not successful
     */
    bool remove_item(buffer_item *item);

    /**
     * @brief Get the size of the buffer
     * @return the size of the buffer
     */
    int get_size();

    /**
     * @brief Get the number of items in the buffer
     * @return the number of items in the buffer
     */
    int get_count();

    /**
     * @brief Chceck if the buffer is empty
     * @return true if the buffer is empty, else false
     */
    bool is_empty();
    /**
     * @brief Check if the buffer is full
     * @return true if the buffer is full, else false
     */
    bool is_full();

    /**
     * @brief Print the buffer
     */
    void print_buffer();
};
#endif //ASSIGN4_BUFFER_H
