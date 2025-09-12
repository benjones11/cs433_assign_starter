#include <iostream>
#include "readyqueue.h"

using namespace std;

//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient comments to your code


/**
 * @brief Constructor for the ReadyQueue class.
 */
 ReadyQueue::ReadyQueue()  {
     //TODO: add your code here
     head = NULL; 
     count = 0; 
 }

/**
 *@brief Destructor
*/
ReadyQueue::~ReadyQueue() {
    //TODO: add your code to release dynamically allocate memory
    Node* current = head;
    while(current != NULL){
        Node* next = current->next;
        delete current;
        current = next;
    }
}

/**
 * @brief Add a PCB representing a process into the ready queue.
 *
 * @param pcbPtr: the pointer to the PCB to be added
 */
void ReadyQueue::addPCB(PCB *pcbPtr) {
    //TODO: add your code here
    // When adding a PCB to the queue, you must change its state to READY.
   if (!pcbPtr) return; 

   pcbPtr->state = ProcState::READY;
    Node* newNode = new Node(pcbPtr);
    count++; 

    if(!head || pcbPtr->priority > head->pcb->priority){
        newNode->next = head; 
        head = newNode; 
        return; 
    }
Node* current = head;
while(current->next && current-> next->pcb->priority >= pcbPtr->priority){
    current = current->next; 
}
newNode->next = current->next; 
current->next = newNode; 
}


/**
 * @brief Remove and return the PCB with the highest priority from the queue
 *
 * @return PCB*: the pointer to the PCB with the highest priority
 */
PCB* ReadyQueue::removePCB() {
    //TODO: add your code here
    // When removing a PCB from the queue, you must change its state to RUNNING.
    if(!head) return NULL;

    Node* newNode = head; 
    head = head->next; 

    PCB* out = newNode->pcb;
    delete newNode; 
    count--; 

    if (out) out->state = ProcState::RUNNING; 

    return out; 

}

/**
 * @brief Returns the number of elements in the queue.
 *
 * @return int: the number of PCBs in the queue
 */
int ReadyQueue::size() {
    //TODO: add your code here
    return count; 
}

/**
 * @brief Display the PCBs in the queue.
 */
void ReadyQueue::displayAll() {
    //TODO: add your code here
    if(!head){
        cout << "empty" << endl; 
        return; 
    }
    Node* current = head; 
    while (current){
        cout << "ID: " << current->pcb->id << ", Priority: " << current->pcb->priority << endl;
        current = current->next;
    }

}