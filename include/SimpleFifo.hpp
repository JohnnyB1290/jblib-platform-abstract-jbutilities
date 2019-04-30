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

namespace jblib::jbutilities
{

class SimpleFifo
{
public:
	SimpleFifo(uint16_t size);
	void reset();
	uint8_t* getWritePointer();
	uint8_t* getReadPointer();
	uint8_t* getBufferPointer();
	void updateBW(uint16_t count);
	void updateBR(uint16_t count);
	void updateBRIndex(uint16_t BR);
	bool isEmpty();
	bool isFull();
	uint8_t nextByte();
	uint16_t get(uint8_t* destination, uint16_t count);
	void gotoTmpWriteMode();
	void exitTmpWriteMode(bool updateCounters);
	void gotoTmpReadMode();
	void exitTmpReadMode(bool updateCounters);
	void write(uint8_t byte);
	void write(uint8_t* bytes, uint16_t count);
	uint16_t count();
	uint16_t capacity();
	uint16_t R();
	uint16_t W();

private:
	uint8_t* buf;
	uint16_t BW;
	uint16_t BR;
	uint16_t BW_tmp;
	uint16_t BR_tmp;
	uint16_t size;
	bool tmpWriteMode;
	bool tmpReadMode;

};

}

#endif /* SIMPLEFIFO_HPP_ */
