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
#include "Controller.hpp"

namespace jblib::jbutilities
{

SimpleFifo::SimpleFifo(uint32_t size) {
	this->BW_tmp = 0;
	this->BR_tmp = 0;
	this->buf = (uint8_t*)malloc(size);
	this->size = size;
	this->reset();
}

SimpleFifo::SimpleFifo(uint8_t* externalBufPointer, uint32_t size) {
	this->BW_tmp = 0;
	this->BR_tmp = 0;
	if(externalBufPointer) {
		this->buf = externalBufPointer;
	} else {
		this->buf = (uint8_t*)malloc(size);
	}
	this->size = size;
	this->reset();
}

void SimpleFifo::reset() {
	this->BW = this->BR = 0;
	this->tmpWriteMode = false;
	this->tmpReadMode = false;
}

uint8_t* SimpleFifo::getWritePointer() {
	return &buf[BW];
}

uint8_t* SimpleFifo::getReadPointer() {
	return &buf[BR];
}

uint8_t* SimpleFifo::getBufferPointer() {
	return buf;
}

void SimpleFifo::updateBW(uint32_t count) {
	this->BW += count;
	this->BW = this->BW >= this->size ? (this->BW % this->size) : this->BW;
}

void SimpleFifo::updateBR(uint32_t count) {
	this->BR += count;
	this->BR = this->BR >= this->size ? (this->BR % this->size) : this->BR;
}

void SimpleFifo::updateBRIndex(uint32_t BR) {
	this->BR = BR % this->size;
}

bool SimpleFifo::isEmpty() {
	return BW == BR;
}

bool SimpleFifo::isFull() {
	return (capacity() - count()) <= 1;
}

uint8_t SimpleFifo::nextByte() {
	if(isEmpty()) {
		return 0;
	}
	uint8_t byte = buf[this->tmpReadMode ? BR_tmp : BR];
	if(this->tmpReadMode) {
		this->BR_tmp++;
		this->BR_tmp = this->BR_tmp >= this->size ? (this->BR_tmp % this->size) : this->BR_tmp;
	} else {
		this->BR++;
		this->BR = this->BR >= this->size ? (this->BR % this->size) : this->BR;
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

	uint32_t tbr = this->tmpReadMode ? BR_tmp : BR;

    const uint32_t toRead = (((count) < (size - tbr)) ? (count) : (size - tbr));
    memcpy(dest, buf + tbr, toRead);
    memcpy(dest + toRead, buf, count - toRead);

    if(this->tmpReadMode) {
    	this->BR_tmp += count;
    	this->BR_tmp = this->BR_tmp >= this->size ? (this->BR_tmp % this->size) : this->BR_tmp;
    } else {
    	this->BR += count;
    	this->BR = this->BR >= this->size ? (this->BR % this->size) : this->BR;
    }

    return count;
}

void SimpleFifo::gotoTmpWriteMode() {
	if(this->tmpWriteMode) {
		return;
	}
	this->BW_tmp = this->BW;
	this->tmpWriteMode = true;
}

void SimpleFifo::exitTmpWriteMode(bool updateCounters) {
	this->tmpWriteMode = false;
	if(updateCounters) {
		this->BW = this->BW_tmp;
	}
}

void SimpleFifo::gotoTmpReadMode() {
	this->BR_tmp = this->BR;
	this->tmpReadMode = true;
}

void SimpleFifo::exitTmpReadMode(bool updateCounters) {
	this->tmpReadMode = false;
	if(updateCounters) {
		this->BR = this->BR_tmp;
	}
}

void SimpleFifo::write(uint8_t byte) {
	uint32_t tbw = this->tmpWriteMode ? BW_tmp : BW;
	buf[tbw++] = byte;
	tbw = tbw == this->size ? 0 : tbw;
	if(this->tmpWriteMode) {
		this->BW_tmp = tbw;
	} else {
		this->BW = tbw;
	}
}

void SimpleFifo::write(uint8_t* bytes, uint32_t count) {
	uint32_t tbw = this->tmpWriteMode ? BW_tmp : BW;
	while(count-- > 0) {
		buf[tbw++] = *bytes++;
		tbw = tbw == this->size ? 0 : tbw;
	}
	if(this->tmpWriteMode) {
		this->BW_tmp = tbw;
	} else {
		this->BW = tbw;
	}
}

void SimpleFifo::writeMemcpy(uint8_t* bytes, uint32_t count) {
	uint32_t tbw = this->tmpWriteMode ? BW_tmp : BW;

    const uint32_t toWrite = (((count) < (size - tbw)) ? (count) : (size - tbw));
    memcpy(buf + tbw, bytes, toWrite);
    memcpy(buf, bytes + toWrite, count - toWrite);

    if(this->tmpWriteMode) {
    	this->BW_tmp += count;
    	this->BW_tmp = this->BW_tmp >= this->size ? (this->BW_tmp % this->size) : this->BW_tmp;
    } else {
    	this->BW += count;
    	this->BW = this->BW >= this->size ? (this->BW % this->size) : this->BW;
    }
}

uint32_t SimpleFifo::count() {
	return BW >= BR ? (BW - BR) : (size - BR + BW);
}

uint32_t SimpleFifo::tmpWriteCount() {
	if(this->tmpWriteMode) {
		return BW_tmp >= BR ? (BW_tmp - BR) : (size - BR + BW_tmp);
	} else {
		return BW >= BR ? (BW - BR) : (size - BR + BW);
	}
}

uint32_t SimpleFifo::capacity() {
	return this->size;
}

uint32_t SimpleFifo::R() {
	return BR;
}

uint32_t SimpleFifo::W() {
	return BW;
}

}
