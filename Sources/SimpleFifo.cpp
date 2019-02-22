/*
 * SimpleFifo.cpp
 *
 * Created: 04.04.2018 13:55:57
 *  Author: kgn
 */ 

#include "SimpleFifo.hpp"
#include "stdlib.h"

SimpleFifo::SimpleFifo(uint16_t size) {
	this->buf = (uint8_t*)malloc(size);
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

void SimpleFifo::updateBW(uint16_t count) {
	this->BW += count;
	this->BW = this->BW >= this->size ? (this->BW % this->size) : this->BW;
}

void SimpleFifo::updateBR(uint16_t count) {
	this->BR += count;
	this->BR = this->BR >= this->size ? (this->BR % this->size) : this->BR;
}

void SimpleFifo::updateBRIndex(uint16_t BR) {
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

uint16_t SimpleFifo::get(uint8_t* destination, uint16_t count) {
	if(count > this->count()) {
		count = this->count();
	}
	for(int i = 0; i < count; i++) {
		*destination++ = nextByte();
	}
	return count;
}

void SimpleFifo::gotoTmpWriteMode() {
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
	uint16_t tbw = this->tmpWriteMode ? BW_tmp : BW;
	buf[tbw++] = byte;
	tbw = tbw == this->size ? 0 : tbw;
	if(this->tmpWriteMode) {
		this->BW_tmp = tbw;
	} else {
		this->BW = tbw;
	}
}

void SimpleFifo::write(uint8_t* bytes, uint16_t count) {
	uint16_t tbw = this->tmpWriteMode ? BW_tmp : BW;
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

uint16_t SimpleFifo::count() {
	return BW >= BR ? (BW - BR) : (size - BR + BW);
}

uint16_t SimpleFifo::capacity() {
	return this->size;
}

uint16_t SimpleFifo::R() {
	return BR;
}

uint16_t SimpleFifo::W() {
	return BW;
}

