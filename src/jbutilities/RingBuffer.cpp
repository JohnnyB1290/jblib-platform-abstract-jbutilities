/**
 * @file
 * @brief Ring buffer class realization
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

// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdlib.h>
#include <string.h>
#include "jbkernel/jb_common.h"
#include "jbutilities/RingBuffer.hpp"

#define HEAD_INDEX                (this->head_ & (this->size_ - 1))
#define TAIL_INDEX                (this->tail_ & (this->size_ - 1))

namespace jblib
{
namespace jbutilities
{

RingBuffer::RingBuffer(void* const buffer, const uint32_t itemSize, const uint32_t size)
{
	if(buffer)
		this->buffer_ = buffer;
	else
		this->buffer_ = malloc_s(itemSize * size);
	this->size_ = size;
	this->itemSize_ = itemSize;
}

void* RingBuffer::getBuffer(void) const
{
	return this->buffer_;
}

void RingBuffer::flush(void)
{
	this->head_ = this->tail_ = 0;
}



uint32_t RingBuffer::getSize(void) const
{
	return this->size_;
}



uint32_t RingBuffer::getCount(void) const
{
	return (this->head_ - this->tail_);
}



uint32_t RingBuffer::getFree(void) const
{
	return (this->size_ - this->getCount());
}



bool RingBuffer::isFull(void)
{
	return (this->getCount() >= this->size_);
}



bool RingBuffer::isEmpty(void)
{
	return (this->head_ == this->tail_);
}



uint32_t RingBuffer::insert(const void* data)
{
	if (this->isFull())
		return 0;
	uint8_t* ptr = (uint8_t*)this->buffer_;
	ptr += HEAD_INDEX * this->itemSize_;
	memcpy(ptr, data, this->itemSize_);
	this->head_++;
	return 1;
}



uint32_t RingBuffer::insertMult(const void* data, uint32_t num)
{
	if (this->isFull())
		return 0;
	uint8_t* ptr = (uint8_t*)this->buffer_;
	uint32_t cnt1, cnt2;
	cnt1 = cnt2 = this->getFree();
	if (HEAD_INDEX + cnt1 >= this->size_)
		cnt1 = this->size_ - HEAD_INDEX;
	cnt2 -= cnt1;

	cnt1 = MIN(cnt1, num);
	num -= cnt1;
	cnt2 = MIN(cnt2, num);
	num -= cnt2;

	ptr += HEAD_INDEX * this->itemSize_;
	memcpy(ptr, data, cnt1 * this->itemSize_);
	this->head_ += cnt1;

	ptr = (uint8_t *)this->buffer_ + HEAD_INDEX * this->itemSize_;
	data = (const uint8_t *) data + cnt1 * this->itemSize_;
	memcpy(ptr, data, cnt2 * this->itemSize_);
	this->head_ += cnt2;

	return cnt1 + cnt2;
}



uint32_t RingBuffer::pop(void* const data)
{
	if(this->isEmpty())
		return 0;
	uint8_t* ptr = (uint8_t*)this->buffer_;
	ptr += TAIL_INDEX * this->itemSize_;
	memcpy(data, ptr, this->itemSize_);
	this->tail_++;
	return 1;
}



uint32_t RingBuffer::popMult(void* data, uint32_t num)
{
	if (this->isEmpty())
		return 0;
	uint8_t* ptr = (uint8_t*)this->buffer_;
	uint32_t cnt1, cnt2;
	cnt1 = cnt2 = this->getCount();
	if (TAIL_INDEX + cnt1 >= this->size_)
		cnt1 = this->size_ - TAIL_INDEX;
	cnt2 -= cnt1;

	cnt1 = MIN(cnt1, num);
	num -= cnt1;
	cnt2 = MIN(cnt2, num);
	num -= cnt2;

	ptr += TAIL_INDEX * this->itemSize_;
	memcpy(data, ptr, cnt1 * this->itemSize_);
	this->tail_ += cnt1;

	ptr = (uint8_t *) this->buffer_ + TAIL_INDEX * this->itemSize_;
	data = (uint8_t *) data + cnt1 * this->itemSize_;
	memcpy(data, ptr, cnt2 * this->itemSize_);
	this->tail_ += cnt2;
	return cnt1 + cnt2;
}



uint32_t RingBuffer::find(void* const data, const uint32_t index)
{
	if ((this->isEmpty()) || (index >= this->getCount()))
		return 0;

	uint8_t* ptr = (uint8_t*)this->buffer_;
	uint32_t tempTail = this->tail_ + index;
	ptr += (tempTail & (this->size_ - 1)) * this->itemSize_;
	memcpy(data, ptr, 1);
	return 1;
}



uint32_t RingBuffer::findMult(void* data, uint32_t num, uint32_t startIndex)
{
	if ((this->isEmpty()) || startIndex >= this->getCount())
		return 0;

	uint8_t* ptr = (uint8_t*)this->buffer_;
	uint32_t tempTail = this->tail_ + startIndex;
	uint32_t cnt1, cnt2;
	cnt1 = cnt2 = this->getCount() - startIndex;
	if ((tempTail & (this->size_ - 1)) + cnt1 >= this->size_)
		cnt1 = this->size_ - (tempTail & (this->size_ - 1));
	cnt2 -= cnt1;

	cnt1 = MIN(cnt1, num);
	num -= cnt1;
	cnt2 = MIN(cnt2, num);
	num -= cnt2;

	ptr += (tempTail & (this->size_ - 1)) * this->itemSize_;
	memcpy(data, ptr, cnt1 * this->itemSize_);
	tempTail += cnt1;

	ptr = (uint8_t *)this->buffer_ + (tempTail & (this->size_ - 1)) * this->itemSize_;
	data = (uint8_t *)data + cnt1 * this->itemSize_;
	memcpy(data, ptr, cnt2 * this->itemSize_);
	return cnt1 + cnt2;
}



uint32_t RingBuffer::delMult(uint32_t num)
{
	if(this->isEmpty())
		return 0;
	uint32_t tempCount = MIN(num, this->getCount());
	this->tail_ += tempCount;
	return tempCount;
}

}
}