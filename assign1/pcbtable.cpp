/**
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Benjamin Jones (TODO: Benjamin Jones)
 * @brief This is the implementation file for the PCBTable class.
 * //You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
 * // Remember to add sufficient comments to your code
 */

#include "pcbtable.h"

/**
 * @brief Construct a new PCBTable object of the given size (number of PCBs)
 *
 * @param size: the capacity of the PCBTable
 */
PCBTable::PCBTable(int size) {
   // TODO: add your code here
   head = NULL; 
   tail = NULL; 
   this->size = size; 
}

/**
 * @brief Destroy the PCBTable object. Make sure to delete all the PCBs in the table.
 *
 */
PCBTable::~PCBTable() {
   // TODO: add your code here
   // Delete all the PCBs in the table
    PCBNode* current = head; 

    while(current != NULL) {
        PCBNode* next = current->next; 
        delete current->process; //delete the PCB
        delete current; //delete the node
        current = next;
}
head = NULL; 
tail = NULL; 
}

/**
 * @brief Get the PCB at index "idx" of the PCBTable.
 *
 * @param idx: the index of the PCB to get
 * @return PCB*: pointer to the PCB at index "idx"
 */
PCB* PCBTable::getPCB(unsigned int idx) {
    // TODO: add your code here
    if(idx >= size) {
        return NULL; //index out of bounds
    }

    PCBNode* current = head;

    for(unsigned int i = 0; i < idx; i++) {
        current = current->next;
        }
        return current->process;
    }
        


/**
 * @brief Add a PCB pointer to the PCBTable at index idx.
 *
 * @param pcb: the PCB to add
 */
void PCBTable::addPCB(PCB *pcb, unsigned int idx) {
    // TODO: add your code here
    // Add a PCB pointer to the PCBTable at index idx.
    if (size >= 100) {
        cout << "PCBTable is full. Cannot add more PCBs." << endl;
        return;
    }
    if (idx >= size) {
        cout << "Index out of bounds. Cannot add PCB at index " << idx << endl;
        return;
    }

    PCBNode* newNode = new PCBNode();
    newNode->process = pcb;
    newNode->next = NULL;
    newNode->prev = NULL;

    if(head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        PCBNode* current = head;
        for(unsigned int i = 0; i < idx; i++) {
            if(current->next == NULL) {
                break;
            }
            current = current->next;
        }
        if(current == head) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        } else if(current == tail) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else {
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
        }
    }
}
