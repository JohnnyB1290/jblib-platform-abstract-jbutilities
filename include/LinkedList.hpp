/**
 * @file
 * @brief Linked List realization
 *
 *
 * @note
 * Copyright © 2019 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 * @note
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * @note
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @note
 * This file is a part of JB_Lib.
 */

#ifndef LINKEDLIST_HPP_
#define LINKEDLIST_HPP_

namespace jblib
{
namespace jbutilities
{

template <class T>
class LinkedList
{
private:
	class Node;

public:
	class LinkIterator;

	void insertFirst(T* newData)
	{
	    Node* newNode = new Node(newData);
	    newNode->next_ = this->first_;
	    if(!this->isEmpty())
	    	this->first_->previous_ = newNode;
	    else
	    	this->last_ = newNode;
	    this->first_ = newNode;
	    this->size_++;
	}



	T* deleteFirst(void)
	{
	    if(this->isEmpty())
	    	return NULL;
	    else {
	    	T* oldFirstData = this->first_->data_;
	    	Node* oldFirst = this->first_;
	        this->first_ = this->first_->next_;
	        if(this->first_)
	        	this->first_->previous_ = NULL;
	        this->size_--;
	        if(this->isEmpty())
	        	this->last_ = NULL;
	        delete oldFirst;
	        return oldFirstData;
	    }
	}



	T* getFirst(void)
	{
		return this->first_->data_;
	}



	void insertLast(T* newData)
	{
	    Node* newNode = new Node(newData);
	    if(this->isEmpty())
	    	this->first_ = newNode;
	    else {
	        newNode->previous_ = this->last_;
	        this->last_->next_ = newNode;
	    }
	    this->last_ = newNode;
	    this->size_++;
	}



	T* deleteLast(void)
	{
	    if(this->isEmpty()) return NULL;
	    T* oldLastDataPtr = this->last_->data_;
	    Node* oldLastPtr = this->last_;
	    if(this->last_->previous_ != NULL)
	    	this->last_->previous_->next_ = NULL;
	    this->last_ = this->last_->previous_;
	    this->size_--;
	    if(this->last_ == NULL) this->first_ = NULL;
	    delete oldLastPtr;
	    return oldLastDataPtr;
	}



	int findIndex(T* data)
	{
		int index = -1;
	    Node* current = this->first_;
	    while((current != NULL) && (current->data_ != data)){
	    	current = current->next_;
	    	index++;
	    }
	    return index;
	}



	void insert(uint32_t index, T* data)
	{
	    if(index > this->size_)
	    	return;
	    if(index == 0){
	        this->insertFirst(data);
	        return;
	    }
	    else if(index == this->size_) {
	        this->insertLast(data);
	        return;
	    }

	    uint32_t indexCurrent = 0;
	    Node* current = this->first_;
	    Node* newNode = new Node(data);

	    while (indexCurrent < index){
	        indexCurrent++;
	        current = current->next_;
	    }

	    newNode->previous_ = current->previous_;
	    current->previous_->next_ = newNode;
	    current->previous_ = newNode;
	    newNode->next_ = current;
	    this->size_++;
	}



	T* deleteNode(T* data)
	{
		Node* current = this->first_;
	    while((current != NULL) && (current->data_ != data)) {
	    	current = current->next_;
	    }

		if(current == NULL)
			return NULL;
		else if(current->previous_ == NULL)
			return this->deleteFirst();
		else if(current->next_ == NULL)
			return this->deleteLast();

		current->previous_->next_ = current->next_;
		current->next_->previous_ = current->previous_;
		this->size_--;
		T* currentData = current->data_;
		delete current;
		return currentData;
	}



	uint32_t getSize(void)
	{
		return this->size_;
	}



	bool isEmpty(void)
	{
		return (this->first_ == NULL);
	}

	LinkIterator* getIteratorPtr(void){
		return &this->iterator;
	}

	class LinkIterator
	{
	public:
		LinkIterator(LinkedList<T>* linkedList)
		{
			this->linkedList_ = linkedList;
			this->current_ = this->linkedList_->first_;
		}



		void reset(void)
		{
			this->current_ = this->linkedList_->first_;
		}



		bool atEnd(void)
		{
			return (this->linkedList_->isEmpty()) ||
					(this->current_->next_ == NULL);
		}



		bool atStart(void)
		{
			return (this->linkedList_->isEmpty()) ||
					(this->current_->previous_ == NULL);
		}



		void nextLink(void)
		{
			 if((!this->linkedList_->isEmpty()) && this->current_->next_ != NULL)
				 this->current_ = this->current_->next_;
		}



		void previousLink(void)
		{
			 if((!this->linkedList_->isEmpty()) && this->current_->previous_ != NULL)
				 this->current_ = this->current_->previous_;
		}



		T* getCurrent(void)
		{
			return (this->current_ == NULL)? NULL : this->current_->data_;
		}



		void insertAfter(T* data)
		{
		    if (this->linkedList_->isEmpty()) {
		    	this->linkedList_->insertFirst(data);
		        this->current_ = this->linkedList_->first_;
		    }
		    else if(this->atEnd()) {
		        this->linkedList_->insertLast(data);
		        this->current_ = this->linkedList_->last_;
		    }
		    else{
		    	Node* newNode = new Node(data);
		    	newNode->previous_ = this->current_;
		    	newNode->next_ = this->current_->next_;
		        this->current_->next_->previous_ = newNode;
		        this->current_->next_ = newNode;
		        this->nextLink();
		        this->linkedList_->size_++;
		    }
		}



		void insertBefore(T* data)
		{
		    if (this->atStart()) {
		        this->linkedList_->insertFirst(data);
		        this->current_ = this->linkedList_->first_;
		    }
		    else {
		        Node* newNode = new Node(data);
		        newNode->previous_ = this->current_->previous_;
		        this->current_->previous_->next_ = newNode;
		        this->current_->previous_ = newNode;
		        newNode->next_ = this->current_;
		        this->previousLink();
		        this->linkedList_->size_++;
		    }
		}



		T* deleteCurrent(void)
		{
		    T* oldData = NULL;

		    if(this->atStart()) {
		        oldData = this->linkedList_->deleteFirst();
		        this->current_ = this->linkedList_->first_;
		    }
		    else if(this->atEnd()) {
		        oldData = this->linkedList_->deleteLast();
		        this->current_ = this->linkedList_->last_;
		    }
		    else {
		        oldData = this->current_->data_;
		        this->current_->next_->previous_ = this->current_->previous_;
		        this->current_->previous_->next_ = this->current_->next_;
		        Node* tempNode = this->current_;
		        this->current_ = this->current_->next_;
		        delete tempNode;
		        this->linkedList_->size_--;
		    }
		    return oldData;
		}

	private:
		Node* current_ = NULL;
		LinkedList<T>* linkedList_ = NULL;
	};

private:

	class Node
	{
	public:
		Node(T* data)
		{
			this->data_ = data;
		}
		Node* next_ = NULL;
		Node* previous_ = NULL;
		T* data_ = NULL;
	};

	Node* first_ = NULL;
	Node* last_ = NULL;
	uint32_t size_ = 0;
	LinkIterator iterator =  LinkIterator(this);
};

}
}

#endif /* LINKEDLIST_HPP_ */
