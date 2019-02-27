/*
 * Ring_buf.hpp
 *
 *  Created on: 05.09.2017
 *      Author: Stalker1290
 */

#ifndef RING_BUF_HPP_
#define RING_BUF_HPP_

#include "Common_interfaces.hpp"
#include "chip.h"


class ring_buf_t:public Channel_Call_Interface_t
{
public:
	ring_buf_t(void* buffer, int itemSize, int count);
	void Flush(void);
	int GetSize(void);
	int GetCount(void);
	int GetFree(void);
	int IsFull(void);
	int IsEmpty(void);
	int Insert(void* data);
	int InsertMult(void* data, int num);
	int Pop(void* data);
	int PopMult(void* data, int num);
	int Find(void* data,uint32_t index);
	int FindMult(void* data, int num, uint32_t start_index);
	int DelMult(int num);
	virtual void channel_callback(uint8_t *mes,uint16_t m_size,void* channel_pointer, void* parameters);
	void* getBuffer(void);
private:
	RINGBUFF_T RingBuff;
	void* buffer;
};



#endif /* RING_BUF_HPP_ */
