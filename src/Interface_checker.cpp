/*
 * Interface_checker.cpp
 *
 * Created: 09.11.2017 18:09:25
 *  Author: Stalker1290
 */ 

#include "Interface_checker.hpp"
#include "stdlib.h"
#include "string.h"

Interface_checker_t::Interface_checker_t(uint8_t* check_mes_ptr, uint8_t* check_mes_mask_ptr, uint8_t check_mes_size):void_channel_t(),Channel_Call_Interface_t()
{
	for(int i = 0; i<Iface_checker_max_num_ifaces; i++)
	{
		this->Ifaces_ptrs_mass[i] = (void_channel_t*)NULL;
		this->Shift_registers_ptrs[i] = (uint8_t*)NULL;
	}
	this->Ifaces_count = 0;
	this->lock = false;
	this->Locked_channel_ptr = (void_channel_t*)NULL;
	this->Initialized = false;
	this->check_mes_ptr = (uint8_t*)NULL;
	this->check_mes_mask_ptr = (uint8_t*)NULL;
	this->call_interface_ptr = (Channel_Call_Interface_t*)NULL;

	this->check_mes_size = check_mes_size;
	this->check_mes_ptr = (uint8_t*)malloc(check_mes_size);
	this->check_mes_mask_ptr = (uint8_t*)malloc(check_mes_size);

	for(int i = 0; i<Iface_checker_max_num_ifaces; i++)
	{
		this->Shift_registers_ptrs[i] = (uint8_t*)malloc(check_mes_size);
		if(this->Shift_registers_ptrs[i] == (uint8_t*)NULL) return;
	}

	if((this->check_mes_ptr != (uint8_t*)NULL)&&(this->check_mes_mask_ptr != (uint8_t*)NULL)&&
			(check_mes_ptr != (uint8_t*)NULL)&&(check_mes_mask_ptr != (uint8_t*)NULL))
	{
		memcpy(this->check_mes_ptr, check_mes_ptr, check_mes_size);
		memcpy(this->check_mes_mask_ptr, check_mes_mask_ptr, check_mes_size);
	}
}

Interface_checker_t::~Interface_checker_t(void)
{
	if(this->check_mes_ptr != (uint8_t*)NULL) free(this->check_mes_ptr);
	if(this->check_mes_mask_ptr != (uint8_t*)NULL) free(this->check_mes_mask_ptr);
	for(int i = 0; i<Iface_checker_max_num_ifaces; i++)
	{
		if(this->Shift_registers_ptrs[i] != (uint8_t*)NULL)
		{
			free(this->Shift_registers_ptrs[i]);
		}
	}
}

void Interface_checker_t::Add_interface(void_channel_t* iface)
{
	if(this->Ifaces_count == Iface_checker_max_num_ifaces) this->Ifaces_count = 0;
	this->Ifaces_ptrs_mass[this->Ifaces_count] = iface;
	this->Ifaces_count++;
}

void Interface_checker_t::Initialize(void* (*mem_alloc)(size_t),uint16_t tx_buf_size, Channel_Call_Interface_t* call_interface_ptr)
{
	if(this->Initialized == false)
	{
		this->call_interface_ptr = call_interface_ptr;
		for(int i = 0; i<this->Ifaces_count; i++)
		{
			if(this->Ifaces_ptrs_mass[i] != (void_channel_t*)NULL) this->Ifaces_ptrs_mass[i]->Initialize(mem_alloc,tx_buf_size,this);
		}
		this->Initialized = true;
	}
}

void Interface_checker_t::DEInitialize(void)
{
	if(this->Initialized)
	{
		for(int i = 0; i<this->Ifaces_count; i++)
		{
			if(this->Ifaces_ptrs_mass[i] != (void_channel_t*)NULL) this->Ifaces_ptrs_mass[i]->DEInitialize();
		}
	}
}

void Interface_checker_t::Tx(uint8_t *mes,uint16_t m_size,void* param)
{
	if(this->Initialized)
	{
		if(this->lock)
		{
			if(this->Locked_channel_ptr != (void_channel_t*)NULL) this->Locked_channel_ptr->Tx(mes,m_size,param);
		}
		else
		{
			for(int i = 0; i<this->Ifaces_count; i++)
			{
				if(this->Ifaces_ptrs_mass[i] != (void_channel_t*)NULL) this->Ifaces_ptrs_mass[i]->Tx(mes,m_size,param);
			}
		}
	}
}

void Interface_checker_t::channel_callback(uint8_t *mes,uint16_t m_size,void* channel_pointer,void* parameters)
{
	void_channel_t* Interface_type = (void_channel_t*)channel_pointer;
	
	if(this->lock)
	{
		if(Interface_type == this->Locked_channel_ptr)
		{
			if(this->call_interface_ptr != (Channel_Call_Interface_t*)NULL) this->call_interface_ptr->channel_callback(mes,m_size,this,parameters);
		}
	}
	else
	{
		for(int i = 0; i<this->Ifaces_count; i++)
		{
			if(Interface_type == this->Ifaces_ptrs_mass[i])
			{
				for(int j = 0; j< m_size; j++)
				{
					if(this->Shift_and_compare(i, mes[j]))
					{
						this->lock = true;
						this->Locked_channel_ptr = this->Ifaces_ptrs_mass[i];
						this->Ifaces_ptrs_mass[i] = (void_channel_t*)NULL;
						this->DEiniAfterLock();
						return;
					}
				}
			}
		}
	}
}

bool Interface_checker_t::Shift_and_compare(uint8_t number, uint8_t bt)
{
	uint8_t temp = 0;
	
	if(this->check_mes_size == 0) return true;
	else
	{
		for(int k = 0; k<this->check_mes_size; k++)
		{
			this->Shift_registers_ptrs[number][k] = this->Shift_registers_ptrs[number][k+1];
		}
		this->Shift_registers_ptrs[number][this->check_mes_size-1] = bt;
		for(int l = 0; l<this->check_mes_size; l++)
		{
			if(this->check_mes_mask_ptr[l])
			{
				if(this->check_mes_ptr[l] != this->Shift_registers_ptrs[number][l]) temp++;
			}
		}
		if(temp) return false;
		else return true;
	}
}

void Interface_checker_t::DEiniAfterLock(void)
{
	if(this->Initialized)
	{
		for(int i = 0; i<this->Ifaces_count; i++)
		{
			if(this->Ifaces_ptrs_mass[i] != (void_channel_t*)NULL) this->Ifaces_ptrs_mass[i]->DEInitialize();
		}
	}
}

void Interface_checker_t::GetParameter(uint8_t ParamName, void* ParamValue)
{
	
}

void Interface_checker_t::SetParameter(uint8_t ParamName, void* ParamValue)
{
	
}
