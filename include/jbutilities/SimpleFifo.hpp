/**
 * @file
 * @brief Simple Fifo class description
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

#ifndef SIMPLEFIFO_HPP_
#define SIMPLEFIFO_HPP_

#include <stdint.h>

namespace jblib
{
namespace jbutilities
{

class SimpleFifo
{
public:
	SimpleFifo(uint32_t size);
	SimpleFifo(uint8_t* externalBufPointer, uint32_t size);

    virtual ~SimpleFifo();

    void reset();
	uint8_t* getWritePointer();
	uint8_t* getReadPointer();
	uint8_t* getBufferPointer();
	void updateBW(uint32_t count);
	void updateBR(uint32_t count);
	void updateBRIndex(uint32_t br);
	bool isEmpty();
	bool isFull();
	uint8_t nextByte();
	uint32_t get(uint8_t* destination, uint32_t count);
	uint32_t getMemcpy(uint8_t* dest, uint32_t count);
	void gotoTmpWriteMode();
	void exitTmpWriteMode(bool updateCounters);
	void gotoTmpReadMode();
	void exitTmpReadMode(bool updateCounters);
	void write(uint8_t byte);
	void write(uint8_t* bytes, uint32_t count);
	bool insertAtStart(uint8_t byte);
	void writeMemcpy(uint8_t* bytes, uint32_t count);
	uint32_t count();
	uint32_t tmpWriteCount();
	uint32_t capacity();
	uint32_t R();
	uint32_t W();

private:
	uint8_t* buf_;
	uint32_t bw_;
	uint32_t br_;
	uint32_t bwTmp_;
	uint32_t brTmp_;
	uint32_t size_;
	bool extMemory_ = false;
	bool tmpWriteMode_;
	bool tmpReadMode_;

};

}
}

#endif /* SIMPLEFIFO_HPP_ */
