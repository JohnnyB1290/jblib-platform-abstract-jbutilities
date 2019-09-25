/**
 * @file
 * @brief Interface checker class realization
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
#include "jbutilities/InterfaceChecker.hpp"

namespace jblib::jbutilities
{

using namespace jbkernel;

InterfaceChecker::InterfaceChecker(uint8_t* message, uint8_t* mask, uint8_t size) :
		IVoidChannel(), IChannelCallback()
{
	for(uint32_t i = 0; i < INTERFACE_CHECKER_NUM_CHANNELS; i++) {
		this->channels_[i] = NULL;
		this->shiftRegisters_[i] = NULL;
	}

	this->messageSize_ = size;
	this->message_ = (uint8_t*)malloc_s(size);
	this->messageMask_ = (uint8_t*)malloc_s(size);
	if((this->message_ == NULL) || (this->messageMask_ == NULL))
		return;

	for(uint32_t i = 0; i < INTERFACE_CHECKER_NUM_CHANNELS; i++){
		this->shiftRegisters_[i] = (uint8_t*)malloc_s(size);
		if(this->shiftRegisters_[i] == NULL)
			return;
	}

	if((message != NULL)&&(mask != NULL)){
		memcpy(this->message_, message, size);
		memcpy(this->messageMask_, mask, size);
	}
}



InterfaceChecker::~InterfaceChecker(void)
{
	free_s(this->message_);
	free_s(this->messageMask_);
	for(uint32_t i = 0; i < INTERFACE_CHECKER_NUM_CHANNELS; i++)
		free_s(this->shiftRegisters_[i]);
}



void InterfaceChecker::addChannel(IVoidChannel* channel)
{
	if(this->channelsCount_ == INTERFACE_CHECKER_NUM_CHANNELS)
		this->channelsCount_ = 0;
	this->channels_[this->channelsCount_] = channel;
	this->channelsCount_++;
}



void InterfaceChecker::initialize(void* (* const mallocFunc)(size_t),
		const uint16_t txBufferSize, IChannelCallback* const callback)
{
	if(!this->isInitialized_){
		this->callback_ = callback;
		for(uint32_t i = 0; i < this->channelsCount_; i++){
			if(this->channels_[i])
				this->channels_[i]->initialize(mallocFunc, txBufferSize, this);
		}
		this->isInitialized_ = true;
	}
}



void InterfaceChecker::deinitialize(void)
{
	if(this->isInitialized_){
		if(this->isLocked_){
			if(this->lockedChannel_){
				this->lockedChannel_->deinitialize();
				this->lockedChannel_ = NULL;
			}
			this->isLocked_ = false;
		}
		else{
			for(uint32_t i = 0; i < this->channelsCount_; i++){
				if(this->channels_[i])
					this->channels_[i]->deinitialize();
			}
		}
		this->channelsCount_ = 0;
		this->callback_ = NULL;
		this->isInitialized_ = false;
	}
}



void InterfaceChecker::tx(uint8_t* const buffer, const uint16_t size, void* parameter)
{
	if(this->isInitialized_) {
		if(this->isLocked_) {
			if(this->lockedChannel_)
				this->lockedChannel_->tx(buffer, size, parameter);
		}
		else {
			for(uint32_t i = 0; i < this->channelsCount_; i++) {
				if(this->channels_[i])
					this->channels_[i]->tx(buffer, size, parameter);
			}
		}
	}
}



void InterfaceChecker:: channelCallback(uint8_t* const buffer, const uint16_t size,
		void* const source, void* parameter)
{
	IVoidChannel* channel = (IVoidChannel*)source;
	if(this->isLocked_) {
		if(channel == this->lockedChannel_) {
			if(this->callback_)
				this->callback_->channelCallback(buffer, size, this, parameter);
		}
	}
	else {
		for(uint32_t i = 0; i < this->channelsCount_; i++) {
			if(channel == this->channels_[i]) {
				for(uint32_t j = 0; j < size; j++) {
					if(this->shiftCompare(i, buffer[j])){
						this->isLocked_ = true;
						this->lockedChannel_ = this->channels_[i];
						this->channels_[i] = (IVoidChannel*)NULL;
						this->afterLockDeinitialize();
						return;
					}
				}
			}
		}
	}
}



bool InterfaceChecker::shiftCompare(uint8_t number, uint8_t byte)
{
	if(this->messageSize_ == 0)
		return true;
	else {
		for(uint32_t k = 0; k < this->messageSize_; k++){
			this->shiftRegisters_[number][k] = this->shiftRegisters_[number][k+1];
		}
		this->shiftRegisters_[number][this->messageSize_-1] = byte;
		for(uint32_t l = 0; l < this->messageSize_; l++){
			if(this->messageMask_[l]) {
				if(this->message_[l] != this->shiftRegisters_[number][l])
					return false;
			}
		}
		return true;
	}
}



void InterfaceChecker::afterLockDeinitialize(void)
{
	if(this->isInitialized_) {
		for(uint32_t i = 0; i < this->channelsCount_; i++) {
			if(this->channels_[i])
				this->channels_[i]->deinitialize();
		}
	}
}



void InterfaceChecker::getParameter(const uint8_t number, void* const value)
{
	
}



void InterfaceChecker::setParameter(const uint8_t number, void* const value)
{
	
}

}
