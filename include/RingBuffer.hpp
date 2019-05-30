/**
 * @file
 * @brief Ring buffer class description
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

#ifndef RINGBUFFER_HPP_
#define RINGBUFFER_HPP_

#include <stdint.h>

namespace jblib
{
namespace jbutilities
{


class RingBuffer
{
public:
	RingBuffer(void* const buffer, const uint32_t itemSize,
			const uint32_t size);
	void flush(void);
	uint32_t getSize(void) const;
	uint32_t getCount(void) const;
	uint32_t getFree(void) const;
	bool isFull(void);
	bool isEmpty(void);
	uint32_t insert(const void* data);
	uint32_t insertMult(const void* data, uint32_t num);
	uint32_t pop(void* const data);
	uint32_t popMult(void* data, uint32_t num);
	uint32_t find(void* const data, const uint32_t index);
	uint32_t findMult(void* data, uint32_t num, uint32_t startIndex);
	uint32_t delMult(uint32_t num);
	void* getBuffer(void) const;

private:
	void* buffer_ = NULL;
	uint32_t size_ = 0;
	uint32_t itemSize_ = 0;
	uint32_t head_ = 0;
	uint32_t tail_ = 0;
};

}
}

#endif /* RINGBUFFER_HPP_ */
