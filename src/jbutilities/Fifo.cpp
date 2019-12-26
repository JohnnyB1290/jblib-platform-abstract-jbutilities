/**
 * @file
 * @brief Fifo class realization
 *
 *
 * @note
 * Copyright © 2019 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * Copyright © 2019 Nikolai Pavlov. Contacts: <pawlow_nic@mail.ru>
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

// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <string.h>
#include "jbkernel/jb_common.h"
#include "jbutilities/Fifo.hpp"

namespace jblib
{
namespace jbutilities
{

Fifo::Fifo(uint8_t* buffer, uint16_t bufferSize)
{
	if(buffer)
		this->buffer_ = buffer;
	else
		this->buffer_ = (uint8_t*)malloc_s(bufferSize);
	this->bufferSize_ = bufferSize;
	this->free_ = bufferSize;
}



bool Fifo::push(uint8_t value)
{
	if (this->free_){
		*(this->buffer_ + this->wp_) = value;
		this->wp_++;
		if (this->wp_ >= this->bufferSize_)
			this->wp_ = 0;
		this->free_--;
		return true;
	}
	return false;
}



bool Fifo::pushMult(const uint8_t* sourse, uint16_t size)
{
	uint16_t tmpFree = this->free_;
	uint16_t tmpWp = this->wp_;

	if (tmpFree >= size) {
		uint16_t l1 = 0;
		uint16_t l2 = 0;
		if((tmpWp + size) >= this->bufferSize_) {
			l1 = this->bufferSize_ - tmpWp;
			l2 = size - l1;
			memcpy((this->buffer_ + tmpWp), sourse, l1);
			memcpy(this->buffer_, (sourse + l1), l2);
			tmpWp = l2;
		}
		else {
			memcpy((this->buffer_ + tmpWp), sourse, size);
			tmpWp += size;
		}
		this->free_ -= size;
		this->wp_ = tmpWp;
		return true;
	}

	return false;
}




bool Fifo::pop(uint8_t* const destination)
{
	if(this->free_ < this->bufferSize_) {
		*destination = *(this->buffer_ + this->rp_); // bufer->buffer_[bufer->rp_++];
		this->rp_++;
		if(this->rp_ >= this->bufferSize_)
			this->rp_ = 0;
		this->free_++;
		return true;
	}
	return false;
}



bool Fifo::find(uint8_t* const destination, uint16_t index) {

	if((this->free_ + index) < this->bufferSize_) {
		if ((this->rp_ + index) >= this->bufferSize_)
			*destination = *(this->buffer_ + this->rp_ + index - this->bufferSize_);
		else
			*destination = *(this->buffer_ + this->rp_ + index); // bufer->buffer_[bufer->rp_++];
		return true;
	}
	return false;
}



bool Fifo::findMult(uint8_t* const destination, uint16_t size, uint16_t startIndex)
{
	uint16_t tmpRp = this->rp_ + startIndex;
	uint16_t tmpFree = this->free_ + startIndex;

	if (tmpRp >= this->bufferSize_)
		tmpRp -= this->bufferSize_;

	if((this->bufferSize_ - tmpFree) >= size){
		if((tmpRp + size) >= this->bufferSize_) {
			uint16_t l1 = this->bufferSize_ - tmpRp;
			uint16_t l2 = size - l1;
			memcpy(destination, (this->buffer_ + tmpRp), l1);
			memcpy(destination + l1, this->buffer_, l2);
			tmpRp = l2;
		}
		else
			memcpy(destination, (this->buffer_ + tmpRp), size);
		return true;
	}
	return false;
}



bool Fifo::popMult(uint8_t* const destination, uint16_t size)
{
	uint16_t tmpFree = this->free_;
	uint16_t tmpRp = this->rp_;

	if((this->bufferSize_ - tmpFree) >= size) {
		if((tmpRp + size) >= this->bufferSize_) {
			uint16_t l1 = this->bufferSize_ - tmpRp;
			uint16_t l2 = size - l1;
			memcpy(destination, (this->buffer_ + tmpRp), l1);
			memcpy(destination + l1, this->buffer_, l2);
			tmpRp = l2;
		}
		else {
			memcpy(destination, (this->buffer_ + tmpRp), size);
			tmpRp += size;
		}
		this->free_ += size;
		this->rp_ = tmpRp;
		return true;
	}
	return false;
}



bool Fifo::isFree(void) const
{
	return (this->free_ == this->bufferSize_);
}



bool Fifo::isFull(void) const
{
	return (this->free_ == 0);
}



uint16_t Fifo::getLevel(void) const
{
	return (this->bufferSize_ - this->free_);
}



uint16_t Fifo::getFree(void) const
{
	return (this->free_);
}



bool Fifo::deleteMult(uint16_t size)
{
	uint16_t tmpFree = this->free_;
	uint16_t tmpRp = this->rp_;
	if((this->bufferSize_ - tmpFree) >= size) {
		if((tmpRp + size) >= this->bufferSize_) {
			uint16_t l1 = this->bufferSize_ - tmpRp;
			uint16_t l2 = size - l1;
			tmpRp = l2;
		}
		else
			tmpRp += size;

		this->free_ += size;
		this->rp_ = tmpRp;
		return true;
	}
	return false;
}

}
}