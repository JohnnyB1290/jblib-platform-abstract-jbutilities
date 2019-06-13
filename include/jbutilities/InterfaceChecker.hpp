/**
 * @file
 * @brief Interface checker class description
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

#ifndef INTERFACECHECKER_HPP_
#define INTERFACECHECKER_HPP_

#include <stdint.h>
#include "jb_common.h"
#include "callback_interfaces.hpp"
#include "IVoidChannel.hpp"

namespace jblib
{
namespace jbutilities
{

using namespace jbkernel;

class InterfaceChecker : public IVoidChannel,public IChannelCallback
{
public:
	InterfaceChecker(uint8_t* message, uint8_t* mask, uint8_t size);
	virtual ~InterfaceChecker(void);
	void addChannel(IVoidChannel* channel);
	virtual void initialize(void* (* const mallocFunc)(size_t),
			const uint16_t txBufferSize, IChannelCallback* const callback);
	virtual void deinitialize(void);
	virtual void tx(uint8_t* const buffer, const uint16_t size, void* parameter);
	virtual void getParameter(const uint8_t number, void* const value);
	virtual void setParameter(const uint8_t number, void* const value);
	virtual void channelCallback(uint8_t* const buffer, const uint16_t size,
			void* const source, void* parameter);

private:
	bool shiftCompare(uint8_t number, uint8_t byte);
	void afterLockDeinitialize(void);

	IVoidChannel* channels_[INTERFACE_CHECKER_NUM_CHANNELS];
	uint8_t channelsCount_ = 0;
	uint8_t* message_ = NULL;
	uint8_t messageSize_ = 0;
	uint8_t* messageMask_ = NULL;
	uint8_t* shiftRegisters_[INTERFACE_CHECKER_NUM_CHANNELS];
	bool isLocked_ = false;
	IVoidChannel* lockedChannel_ = NULL;
	bool isInitialized_ = false;
	IChannelCallback* callback_ = NULL;
};

}
}

#endif /* INTERFACECHECKER_HPP_ */
