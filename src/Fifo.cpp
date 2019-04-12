/*
 * Fifo.cpp
 *
 * Created: 03.11.2017 16:49:26
 *  Author: Pawlow_nic, Stalker1290
 */ 

#include "Fifo.hpp"
#include "string.h"


Fifo_t::Fifo_t(uint8_t* buf,uint16_t bufsize)
{
	this->buf = buf;
	this->bufsize = bufsize;
	this->Free = bufsize;
	this->RP = 0;
	this->WP = 0;
}

bool Fifo_t::Fifo_Push(uint8_t value)
{
	bool retVal = false;
	
	if (this->Free != 0)
	{
		*(this->buf +this->WP) = value;
		this->WP++;
		if (this->WP >= this->bufsize) this->WP = 0;
		this->Free--;
		retVal = true;
	}
	return retVal;
}

bool Fifo_t::Fifo_NPush(const uint8_t* psourse, uint16_t Num)
{
	uint16_t l1,l2;
	bool retVal = false;
	uint16_t tmpFree,tmpWP;

	tmpFree = this->Free;
	tmpWP = this->WP;
	

	if (tmpFree >= Num)
	{
		if((tmpWP+Num) >= this->bufsize)
		{
			l1 = this->bufsize - tmpWP;
			l2 = Num - l1;
			memcpy((this->buf + tmpWP), psourse, l1);
			memcpy(this->buf,psourse+l1,l2);
			tmpWP = l2;
		}
		else
		{
			memcpy((this->buf + tmpWP), psourse, Num);
			tmpWP += Num;
		}

		this->Free -= Num;
		this->WP = tmpWP;

		retVal = true;
	}

	return retVal;
}

bool Fifo_t::Fifo_Pop(uint8_t *pvalue)
{
	bool retVal = false;
		
	if(this->Free < this->bufsize)
	{
		*pvalue = *(this->buf + this->RP);// bufer->buf[bufer->RP++];
		this->RP++;
		if(this->RP >= this->bufsize) this->RP = 0;
		this->Free++;
		retVal = true;
	}
	
	return retVal;
}

bool Fifo_t::Fifo_Find(uint8_t *pvalue, uint16_t index)
{
	bool retVal = false;
	

	if((this->Free+index) < this->bufsize)
	{
		if ((this->RP+index) >= this->bufsize)
		{
			*pvalue = *(this->buf + this->RP + index - this->bufsize);
		}
		else
		{
			*pvalue = *(this->buf + this->RP + index);// bufer->buf[bufer->RP++];
		}
		retVal = true;
	}
	return retVal;
}

bool Fifo_t::Fifo_NFind(uint8_t *pdest, uint16_t Num, uint16_t start_index)
{
	uint16_t l1,l2;
	bool retVal = false;
	uint16_t tmpRP,tmpFree;
	
	tmpFree = this->Free + start_index;

	tmpRP = this->RP + start_index;
	if (tmpRP>= this->bufsize)
	{
		tmpRP-=this->bufsize;
	}

	if((this->bufsize - tmpFree) >= Num)
	{
		if((tmpRP+Num) >= this->bufsize)
		{
			l1 = this->bufsize - tmpRP;
			l2 = Num - l1;
			memcpy(pdest,(this->buf + tmpRP), l1);
			memcpy(pdest+l1,this->buf,l2);
			tmpRP = l2;
		}
		else
		{
			memcpy(pdest,(this->buf + tmpRP), Num);
		}
		retVal =  true;
	}
	return retVal;
}

bool Fifo_t::Fifo_NPop(uint8_t *pdest, uint16_t Num)
{
	uint16_t l1,l2;
	bool retVal = false;
	uint16_t tmpRP, tmpFree;
	
	tmpFree = this->Free;
	tmpRP = this->RP;

	if((this->bufsize - tmpFree) >= Num)
	{
		if((tmpRP+Num) >= this->bufsize)
		{
			l1 = this->bufsize - tmpRP;
			l2 = Num - l1;
			memcpy(pdest,(this->buf + tmpRP), l1);
			memcpy(pdest+l1,this->buf,l2);
			tmpRP = l2;
		}
		else
		{
			memcpy(pdest,(this->buf + tmpRP), Num);
			tmpRP += Num;
		}

		this->Free += Num;
		this->RP = tmpRP;

		retVal =  true;
	}
	return retVal;
}

bool Fifo_t::Fifo_IsFree(void)
{
	bool retVal; 

	retVal = (this->Free == this->bufsize);

	return retVal;
}

bool Fifo_t::Fifo_IsFull(void)
{
	bool retVal;

	retVal = (this->Free == 0);
	
	return retVal;
}

uint16_t Fifo_t::Fifo_GetLevel(void)
{
	uint16_t retVal;
	
	retVal = (this->bufsize - this->Free);

	return retVal;
}

uint16_t Fifo_t::Fifo_GetFree(void)
{
	uint16_t retVal;
	
	retVal = (this->Free);
	return retVal;	
}

bool Fifo_t::Fifo_NDel(uint16_t Num)
{
	uint16_t l1,l2;
	bool retVal = false;
	uint16_t tmpRP, tmpFree;
	
		tmpFree = this->Free;
		tmpRP = this->RP;
		
	if((this->bufsize - tmpFree) >= Num)
	{
		if((tmpRP+Num) >= this->bufsize)
		{
			l1 = this->bufsize - tmpRP;
			l2 = Num - l1;
			tmpRP = l2;
		}
		else
		{
			tmpRP += Num;
		}

		this->Free += Num;
		this->RP = tmpRP;
		retVal =  true;
	}

	return retVal;	
}
