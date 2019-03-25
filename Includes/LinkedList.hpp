/*
 * LinkedList.hpp
 *
 *  Created on: 19 мар. 2019 г.
 *      Author: Stalker1290
 */

#ifndef LINKEDLIST_HPP_
#define LINKEDLIST_HPP_

#include "chip.h"

template <class T>
class LinkedList_t{
private:
	class Node_t{
	public:
		Node_t(T* dataPtr){
			this->nextPtr = NULL;
			this->previousPtr = NULL;
			this->dataPtr = dataPtr;
		}
		Node_t* nextPtr;
		Node_t* previousPtr;
		T* dataPtr;
	};
	Node_t* firstPtr;
	Node_t* lastPtr;
	uint32_t size;
public:

	LinkedList_t(void){
		this->firstPtr = NULL;
		this->lastPtr = NULL;
		this->size = 0;
		this->iteratorPtr = new LinkIterator_t(this);
	}

	void insertFirst(T* newDataPtr){
	    Node_t* newNode = new Node_t(newDataPtr);
	    newNode->nextPtr = this->firstPtr;
	    if(!this->isEmpty()) this->firstPtr->previousPtr = newNode;
	    else this->lastPtr = newNode;
	    this->firstPtr = newNode;
	    this->size++;
	}

	T* deleteFirst(void){
	    if(this->isEmpty()) return NULL;
	    else {
	    	T* oldFirstDataPtr = this->firstPtr->dataPtr;
	    	Node_t* oldFirstPtr = this->firstPtr;
	        this->firstPtr = this->firstPtr->nextPtr;
	        if(this->firstPtr != NULL)
	        	this->firstPtr->previousPtr = NULL;
	        this->size--;
	        if(this->isEmpty()) this->lastPtr = NULL;
	        delete oldFirstPtr;
	        return oldFirstDataPtr;
	    }
	}

	T* getFirst(void){
		return this->firstPtr->dataPtr;
	}

	void insertLast(T* newDataPtr){
	    Node_t* newNode = new Node_t(newDataPtr);
	    if(this->isEmpty()) this->firstPtr = newNode;
	    else {
	        newNode->previousPtr = this->lastPtr;
	        this->lastPtr->nextPtr = newNode;
	    }
	    this->lastPtr = newNode;
	    this->size++;
	}

	T* deleteLast(void){
	    if(this->isEmpty()) return NULL;
	    T* oldLastDataPtr = this->lastPtr->dataPtr;
	    Node_t* oldLastPtr = this->lastPtr;
	    if(this->lastPtr->previousPtr != NULL)
	    	this->lastPtr->previousPtr->nextPtr = NULL;
	    this->lastPtr = this->lastPtr->previousPtr;
	    this->size--;
	    if(this->lastPtr == NULL) this->firstPtr = NULL;
	    delete oldLastPtr;
	    return oldLastDataPtr;
	}

	int findIndex(T* dataPtr){
		int index = -1;
	    Node_t* currentPtr = this->firstPtr;
	    while((currentPtr != NULL) && (currentPtr->dataPtr != dataPtr)){
	    	currentPtr = currentPtr->nextPtr;
	    	index++;
	    }
	    return index;
	}

	void insert(uint32_t index, T* dataPtr){
	    if(index > this->size) return;
	    if(index == 0){
	        this->insertFirst(dataPtr);
	        return;
	    }
	    else if(index == this->size){
	        this->insertLast(dataPtr);
	        return;
	    }

	    uint32_t indexCurrent = 0;
	    Node_t* currentPtr = this->firstPtr;
	    Node_t* newNodePtr = new Node_t(dataPtr);

	    while (indexCurrent < index){
	        indexCurrent++;
	        currentPtr = currentPtr->nextPtr;
	    }

	    newNodePtr->previousPtr = currentPtr->previousPtr;
	    currentPtr->previousPtr->nextPtr = newNodePtr;
	    currentPtr->previousPtr = newNodePtr;
	    newNodePtr->nextPtr = currentPtr;
	    this->size++;
	}

	T* deleteNode(T* dataPtr){
		Node_t* currentPtr = this->firstPtr;
	    while((currentPtr != NULL) && (currentPtr->dataPtr != dataPtr)){
	    	currentPtr = currentPtr->nextPtr;
	    }

		if(currentPtr == NULL) return NULL;
		else if(currentPtr->previousPtr == NULL) return this->deleteFirst();
		else if(currentPtr->nextPtr == NULL) return this->deleteLast();

		currentPtr->previousPtr->nextPtr = currentPtr->nextPtr;
		currentPtr->nextPtr->previousPtr = currentPtr->previousPtr;
		this->size--;
		T* currentDataPtr = currentPtr->dataPtr;
		delete currentPtr;
		return currentDataPtr;
	}

	uint32_t getSize(void){
		return this->size;
	}

	bool isEmpty(void){
		return (this->firstPtr == NULL);
	}

	class LinkIterator_t{
	public:
		LinkIterator_t(LinkedList_t<T>* linkedListPtr){
			this->linkedListPtr = linkedListPtr;
			this->currentPtr = this->linkedListPtr->firstPtr;
		}

		void reset(void){
			this->currentPtr = this->linkedListPtr->firstPtr;
		}

		bool atEnd(void){
			return (this->linkedListPtr->isEmpty()) ||
					(this->currentPtr->nextPtr == NULL);
		}

		bool atStart(void){
			return (this->linkedListPtr->isEmpty()) ||
					(this->currentPtr->previousPtr == NULL);
		}

		void nextLink(void){
			 if((!this->linkedListPtr->isEmpty()) && this->currentPtr->nextPtr != NULL)
				 this->currentPtr = this->currentPtr->nextPtr;
		}

		void previousLink(void){
			 if((!this->linkedListPtr->isEmpty()) && this->currentPtr->previousPtr != NULL)
				 this->currentPtr = this->currentPtr->previousPtr;
		}

		T* getCurrent(void){
			return (this->currentPtr == NULL)? NULL : this->currentPtr->dataPtr;
		}

		void insertAfter(T* dataPtr){
		    if (this->linkedListPtr->isEmpty()){
		    	this->linkedListPtr->insertFirst(dataPtr);
		        this->currentPtr = this->linkedListPtr->firstPtr;
		    }
		    else if(this->atEnd()){
		        this->linkedListPtr->insertLast(dataPtr);
		        this->currentPtr = this->linkedListPtr->lastPtr;
		    } else{
		    	Node_t* newNodePtr = new Node_t(dataPtr);
		    	newNodePtr->previousPtr = this->currentPtr;
		    	newNodePtr->nextPtr = this->currentPtr->nextPtr;
		        this->currentPtr->nextPtr->previousPtr = newNodePtr;
		        this->currentPtr->nextPtr = newNodePtr;
		        this->nextLink();
		        this->linkedListPtr->size++;
		    }
		}

		void insertBefore(T* dataPtr){
		    if (this->atStart()){
		        this->linkedListPtr->insertFirst(dataPtr);
		        this->currentPtr = this->linkedListPtr->firstPtr;
		    }
		    else{
		        Node_t* newNodePtr = new Node_t(dataPtr);
		        newNodePtr->previousPtr = this->currentPtr->previousPtr;
		        this->currentPtr->previousPtr->nextPtr = newNodePtr;
		        this->currentPtr->previousPtr = newNodePtr;
		        newNodePtr->nextPtr = this->currentPtr;
		        this->previousLink();
		        this->linkedListPtr->size++;
		    }
		}

		T* deleteCurrent(void){
		    T* oldDataPtr = NULL;

		    if(this->atStart()) {
		        oldDataPtr = this->linkedListPtr->deleteFirst();
		        this->currentPtr = this->linkedListPtr->firstPtr;
		    }
		    else if(this->atEnd()){
		        oldDataPtr = this->linkedListPtr->deleteLast();
		        this->currentPtr = this->linkedListPtr->lastPtr;
		    }
		    else{
		        oldDataPtr = this->currentPtr->dataPtr;
		        this->currentPtr->nextPtr->previousPtr = this->currentPtr->previousPtr;
		        this->currentPtr->previousPtr->nextPtr = this->currentPtr->nextPtr;
		        Node_t* tempNodePtr = this->currentPtr;
		        this->currentPtr = this->currentPtr->nextPtr;
		        delete tempNodePtr;
		        this->linkedListPtr->size--;
		    }
		    return oldDataPtr;
		}

	private:
		Node_t* currentPtr;
		LinkedList_t<T>* linkedListPtr;
	};

	LinkIterator_t* getIteratorPtr(void){
		return this->iteratorPtr;
	}

private:
	LinkIterator_t* iteratorPtr;
};

#endif /* LINKEDLIST_HPP_ */
