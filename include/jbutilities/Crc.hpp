/**
 * @file
 * @brief CRC Calculation class description
 *
 *
 * @note
 * Copyright © 2019 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
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

#ifndef CRC_HPP_
#define CRC_HPP_

#include <stdint.h>

namespace jblib
{
namespace jbutilities
{

class Crc
{
public:
	static uint8_t crc8(uint8_t* data, uint16_t size);
	static uint8_t crc8(uint8_t crc, uint8_t* data, uint16_t size);
	static uint16_t crc16(uint8_t* data, uint16_t size);
	static uint16_t crc16(uint16_t crc, uint8_t* data, uint16_t size);
	static uint16_t crc16(uint8_t* data, uint16_t size, uint16_t crc);

private:
	static const uint8_t crc8Table_[256];
};

}
}

#endif /* CRC_HPP_ */
