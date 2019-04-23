/*
 * Ring_buf.cpp
 *
 *  Created on: 05.09.2017
 *      Author: Stalker1290
 */
// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Ring_buf.hpp"


ring_buf_t::ring_buf_t(void* buffer, int itemSize, int count):Channel_Call_Interface_t()
{
	this->buffer = buffer;
	RingBuffer_Init(&(this->RingBuff), buffer, itemSize, count);
}

void* ring_buf_t::getBuffer(void){
	return this->buffer;
}

void ring_buf_t::Flush(void)
{
	RingBuffer_Flush(&(this->RingBuff));
}

int ring_buf_t::GetSize(void)
{
	return RingBuffer_GetSize(&(this->RingBuff));
}

int ring_buf_t::GetCount(void)
{
	return RingBuffer_GetCount(&(this->RingBuff));
}

int ring_buf_t::GetFree(void)
{
	return RingBuffer_GetFree(&(this->RingBuff));
}

int ring_buf_t::IsFull(void)
{
	return RingBuffer_IsFull(&(this->RingBuff));
}

int ring_buf_t::IsEmpty(void)
{
	return RingBuffer_IsEmpty(&(this->RingBuff));
}

int ring_buf_t::Insert(void* data)
{
	return RingBuffer_Insert(&(this->RingBuff), data);
}

int ring_buf_t::InsertMult(void* data, int num)
{
	return RingBuffer_InsertMult(&(this->RingBuff), data, num);
}

int ring_buf_t::Pop(void* data)
{
	return RingBuffer_Pop(&(this->RingBuff), data);
}

int ring_buf_t::PopMult(void* data, int num)
{
	return RingBuffer_PopMult(&(this->RingBuff), data, num);
}

int ring_buf_t::Find(void* data,uint32_t index)
{
	return RingBuffer_Find(&(this->RingBuff), data, index);
}

int ring_buf_t::FindMult(void* data, int num, uint32_t start_index)
{
	return RingBuffer_FindMult(&(this->RingBuff), data, num, start_index);
}

int ring_buf_t::DelMult(int num)
{
	return RingBuffer_DelMult(&(this->RingBuff), num);
}

void ring_buf_t::channel_callback(uint8_t *mes,uint16_t m_size,void* channel_pointer,void* parameters)
{
	RingBuffer_InsertMult(&(this->RingBuff), mes, m_size);
}
