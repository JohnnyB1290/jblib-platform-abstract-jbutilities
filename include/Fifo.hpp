/*
 * Fifo.hpp
 *
 * Created: 03.11.2017 16:49:57
 *  Author: Pawlow_nic, Stalker1290
 */ 


#ifndef FIFO_HPP_
#define FIFO_HPP_

#include "chip.h"
#include "Common_interfaces.hpp"

class Fifo_t
{
public:
	Fifo_t(uint8_t* buf,uint16_t bufsize);
	bool Fifo_IsFull(void);
	bool Fifo_IsFree(void);
	bool Fifo_NPop(uint8_t *pdest, uint16_t Num);
	bool Fifo_Pop(uint8_t *pvalue);
	bool Fifo_NPush(const uint8_t* psourse, uint16_t Num);
	bool Fifo_Push(uint8_t value);
	uint16_t Fifo_GetLevel(void);
	uint16_t Fifo_GetFree(void);
	bool Fifo_Find(uint8_t *pvalue, uint16_t index);
	bool Fifo_NFind(uint8_t *pdest, uint16_t Num, uint16_t start_index);
	bool Fifo_NDel(uint16_t Num);
private:
	uint8_t* buf;
	uint16_t bufsize;
	uint16_t Free;
	uint16_t RP;
	uint16_t WP;
};

#endif /* FIFO_HPP_ */
