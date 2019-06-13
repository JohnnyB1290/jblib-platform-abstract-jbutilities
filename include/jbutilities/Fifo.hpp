/**
 * @file
 * @brief Fifo class description
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


#ifndef FIFO_HPP_
#define FIFO_HPP_

#include <stdint.h>

namespace jblib
{
namespace jbutilities
{


class Fifo
{
public:
	Fifo(uint8_t* buffer, uint16_t bufferSize);
	bool isFull(void) const;
	bool isFree(void) const;
	bool popMult(uint8_t* const destination, uint16_t size);
	bool pop(uint8_t* const destination);
	bool pushMult(const uint8_t* sourse, uint16_t size);
	bool push(uint8_t value);
	uint16_t getLevel(void) const;
	uint16_t getFree(void) const;
	bool find(uint8_t* const destination, uint16_t index);
	bool findMult(uint8_t* const destination, uint16_t size, uint16_t startIndex);
	bool deleteMult(uint16_t size);

private:
	uint8_t* buffer_ = NULL;
	uint16_t bufferSize_ = 0;
	uint16_t free_ = 0;
	uint16_t rp_ = 0;
	uint16_t wp_ = 0;
};

}
}

#endif /* FIFO_HPP_ */
