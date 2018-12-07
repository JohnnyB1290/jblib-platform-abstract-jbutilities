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
	this->BW = 0;
	this->BR = 0;
}

void SimpleFifo::reset() {
//	for(int i = 0; i < this->size; i++) {
//		this->buf[i] = 0;
//	}
	this->BW = this->BR = 0;
}

uint8_t* SimpleFifo::getWritePointer() {
	return &buf[BW];
}

uint8_t* SimpleFifo::getReadPointer() {
	return &buf[BR];
}

void SimpleFifo::updateBW(uint16_t count) {
	this->BW += count;
	this->BW = this->BW >= this->size ? (this->BW % this->size) : this->BW;
}

void SimpleFifo::updateBR(uint16_t count) {
	this->BR += count;
	this->BR = this->BR >= this->size ? (this->BR % this->size) : this->BR;
}

bool SimpleFifo::isEmpty() {
	return BW == BR;
}

uint8_t SimpleFifo::nextByte() {
	if(isEmpty()) {
		return 0;
	}
	return buf[BR++];
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

void SimpleFifo::write(uint8_t byte) {
	uint16_t tbw = BW;
	buf[tbw++] = byte;
	tbw = tbw == this->size ? 0 : tbw;
	this->BW = tbw;
}

void SimpleFifo::write(uint8_t* bytes, uint16_t count) {
	uint16_t tbw = BW;
	while(count-- > 0) {
		buf[tbw++] = *bytes++;
		tbw = tbw == this->size ? 0 : tbw;
	}
	BW = tbw;
}

uint16_t SimpleFifo::count() {
	return (BW - BR);
}

uint16_t SimpleFifo::R() {
	return BR;
}

uint16_t SimpleFifo::W() {
	return BW;
}

