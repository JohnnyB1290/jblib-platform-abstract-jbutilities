/**
 * @file
 * @brief Simple Fifo class realization
 *
 *
 * @note
 * Copyright © 2019 Gleb Koba. Contacts: <koban4ik_11@mail.ru>
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
#include <stdlib.h>
#include "jbutilities/SimpleFifo.hpp"

namespace jblib
{
namespace jbutilities
{

SimpleFifo::SimpleFifo(uint32_t size) {
	this->bwTmp_ = 0;
	this->brTmp_ = 0;
	this->buf_ = (uint8_t*)malloc(size);
	this->size_ = size;
	this->reset();
}

SimpleFifo::SimpleFifo(uint8_t* externalBufPointer, uint32_t size) {
	this->bwTmp_ = 0;
	this->brTmp_ = 0;
	if(externalBufPointer) {
		this->buf_ = externalBufPointer;
		this->extMemory_ = true;
	} else {
		this->buf_ = (uint8_t*)malloc(size);
	}
	this->size_ = size;
	this->reset();
}


SimpleFifo::~SimpleFifo() {
	if(this->extMemory_) {
		return;
	}
	free(this->buf_);
}

void SimpleFifo::reset() {
	this->bw_ = this->br_ = 0;
	this->tmpWriteMode_ = false;
	this->tmpReadMode_ = false;
}

uint8_t* SimpleFifo::getWritePointer() {
	return &this->buf_[this->bw_];
}

uint8_t* SimpleFifo::getReadPointer() {
	return &this->buf_[this->br_];
}

uint8_t* SimpleFifo::getBufferPointer() {
	return this->buf_;
}

void SimpleFifo::updateBW(uint32_t count) {
	this->bw_ += count;
	this->bw_ = this->bw_ >= this->size_ ? (this->bw_ % this->size_) : this->bw_;
}

void SimpleFifo::updateBR(uint32_t count) {
	this->br_ += count;
	this->br_ = this->br_ >= this->size_ ? (this->br_ % this->size_) : this->br_;
}

void SimpleFifo::updateBRIndex(uint32_t BR) {
	this->br_ = BR % this->size_;
}

bool SimpleFifo::isEmpty() {
	return this->bw_ == this->br_;
}

bool SimpleFifo::isFull() {
	return (capacity() - count()) <= 1;
}

uint8_t SimpleFifo::nextByte() {
	if(isEmpty()) {
		return 0;
	}
	uint8_t byte = this->buf_[this->tmpReadMode_ ? brTmp_ : this->br_];
	if(this->tmpReadMode_) {
		this->brTmp_++;
		this->brTmp_ = this->brTmp_ >= this->size_ ? (this->brTmp_ % this->size_) : this->brTmp_;
	} else {
		this->br_++;
		this->br_ = this->br_ >= this->size_ ? (this->br_ % this->size_) : this->br_;
	}
	return byte;
}

uint32_t SimpleFifo::get(uint8_t* destination, uint32_t count) {
	if(count > this->count()) {
		count = this->count();
	}
	for(uint32_t i = 0; i < count; i++) {
		*destination++ = nextByte();
	}
	return count;
}

uint32_t SimpleFifo::getMemcpy(uint8_t* dest, uint32_t count) {
	if(count > this->count()) {
		count = this->count();
	}

	uint32_t tbr = this->tmpReadMode_ ? brTmp_ : this->br_;

    const uint32_t toRead = (((count) < (this->size_ - tbr)) ? (count) : (this->size_ - tbr));
    memcpy(dest, this->buf_ + tbr, toRead);
    memcpy(dest + toRead, this->buf_, count - toRead);

    if(this->tmpReadMode_) {
    	this->brTmp_ += count;
    	this->brTmp_ = this->brTmp_ >= this->size_ ? (this->brTmp_ % this->size_) : this->brTmp_;
    } else {
    	this->br_ += count;
    	this->br_ = this->br_ >= this->size_ ? (this->br_ % this->size_) : this->br_;
    }

    return count;
}

void SimpleFifo::gotoTmpWriteMode() {
	if(this->tmpWriteMode_) {
		return;
	}
	this->bwTmp_ = this->bw_;
	this->tmpWriteMode_ = true;
}

void SimpleFifo::exitTmpWriteMode(bool updateCounters) {
	this->tmpWriteMode_ = false;
	if(updateCounters) {
		this->bw_ = this->bwTmp_;
	}
}

void SimpleFifo::gotoTmpReadMode() {
	this->brTmp_ = this->br_;
	this->tmpReadMode_ = true;
}

void SimpleFifo::exitTmpReadMode(bool updateCounters) {
	this->tmpReadMode_ = false;
	if(updateCounters) {
		this->br_ = this->brTmp_;
	}
}

void SimpleFifo::write(uint8_t byte) {
	uint32_t tbw = this->tmpWriteMode_ ? bwTmp_ : bw_;
	this->buf_[tbw++] = byte;
	tbw = tbw == this->size_ ? 0 : tbw;
	if(this->tmpWriteMode_) {
		this->bwTmp_ = tbw;
	} else {
		this->bw_ = tbw;
	}
}

bool SimpleFifo::insertAtStart(uint8_t byte) {
	uint32_t tbw = this->bw_;
	uint32_t tbr = this->br_;
	if(tbw <= tbr || this->tmpWriteMode_) {
		return false;
	}
	for(uint32_t i = (tbw -1); i >= tbr; i--) {
		this->buf_[i + 1] = this->buf_[i];
		if(i == 0) {
			break;
		}
	}
	this->buf_[tbr] = byte;
	tbw++;
	tbw = tbw == this->size_ ? 0 : tbw;
	this->bw_ = tbw;
	return true;
}


void SimpleFifo::write(uint8_t* bytes, uint32_t count) {
	uint32_t tbw = this->tmpWriteMode_ ? bwTmp_ : bw_;
	while(count-- > 0) {
		this->buf_[tbw++] = *bytes++;
		tbw = tbw == this->size_ ? 0 : tbw;
	}
	if(this->tmpWriteMode_) {
		this->bwTmp_ = tbw;
	} else {
		this->bw_ = tbw;
	}
}

void SimpleFifo::writeMemcpy(uint8_t* bytes, uint32_t count) {
	uint32_t tbw = this->tmpWriteMode_ ? bwTmp_ : bw_;

    const uint32_t toWrite = (((count) < (this->size_ - tbw)) ? (count) : (this->size_ - tbw));
    memcpy(this->buf_ + tbw, bytes, toWrite);
    memcpy(this->buf_, bytes + toWrite, count - toWrite);

    if(this->tmpWriteMode_) {
    	this->bwTmp_ += count;
    	this->bwTmp_ = this->bwTmp_ >= this->size_ ? (this->bwTmp_ % this->size_) : this->bwTmp_;
    } else {
    	this->bw_ += count;
    	this->bw_ = this->bw_ >= this->size_ ? (this->bw_ % this->size_) : this->bw_;
    }
}

uint32_t SimpleFifo::count() {
	return bw_ >= this->br_ ? (bw_ - this->br_) : (this->size_ - this->br_ + bw_);
}

uint32_t SimpleFifo::tmpWriteCount() {
	if(this->tmpWriteMode_) {
		return bwTmp_ >= this->br_ ? (bwTmp_ - this->br_) : (this->size_ - this->br_ + bwTmp_);
	} else {
		return bw_ >= this->br_ ? (bw_ - this->br_) : (this->size_ - this->br_ + bw_);
	}
}

uint32_t SimpleFifo::capacity() {
	return this->size_;
}

uint32_t SimpleFifo::R() {
	return this->br_;
}

uint32_t SimpleFifo::W() {
	return this->bw_;
}

}
}