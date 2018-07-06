/*
 * SimpleFifo.hpp
 *
 * Created: 04.04.2018 13:55:57
 *  Author: kgn
 */ 


#ifndef SIMPLE_FIFO_HPP_
#define SIMPLE_FIFO_HPP_

#include "chip.h"

class SimpleFifo {

public:

	SimpleFifo(uint16_t size);
	void reset();
	uint8_t* getWritePointer();
	uint8_t* getReadPointer();
	void updateBW(uint16_t count);
	void updateBR(uint16_t count);
	bool isEmpty();
	uint8_t nextByte();
	uint16_t get(uint8_t* destination, uint16_t count);
	void write(uint8_t byte);
	void write(uint8_t* bytes, uint16_t count);
	uint16_t count();
	uint16_t R();
	uint16_t W();


private:
	uint8_t* buf;
	uint16_t BW;
	uint16_t BR;
	uint16_t size;

};

#endif /* SIMPLE_FIFO_HPP_ */
