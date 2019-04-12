/*
 * Interface_checker.hpp
 *
 *  Created on: 09.11.2017
 *      Author: Stalker1290
 */

#ifndef INTERFACE_CHECKER_HPP_
#define INTERFACE_CHECKER_HPP_

#include "chip.h"
#include "Void_Channel.hpp"

class Interface_checker_t:public void_channel_t,public Channel_Call_Interface_t
{
public:
	Interface_checker_t(uint8_t* check_mes_ptr, uint8_t* check_mes_mask_ptr, uint8_t check_mes_size);
	virtual ~Interface_checker_t(void);
	void Add_interface(void_channel_t* iface);
	virtual void Initialize(void* (*mem_alloc)(size_t),uint16_t tx_buf_size, Channel_Call_Interface_t* call_interface_ptr);
	virtual void DEInitialize(void);
	virtual void Tx(uint8_t *mes,uint16_t m_size,void* param);
	virtual void GetParameter(uint8_t ParamName, void* ParamValue);
	virtual void SetParameter(uint8_t ParamName, void* ParamValue);
	virtual void channel_callback(uint8_t *mes,uint16_t m_size,void* channel_pointer,void* parameters);
private:
	bool Shift_and_compare(uint8_t number, uint8_t bt);
	void DEiniAfterLock(void);
	void_channel_t* Ifaces_ptrs_mass[Iface_checker_max_num_ifaces];
	uint8_t Ifaces_count;
	uint8_t* check_mes_ptr;
	uint8_t check_mes_size;
	uint8_t* check_mes_mask_ptr;
	uint8_t* Shift_registers_ptrs[Iface_checker_max_num_ifaces];
	bool lock;
	void_channel_t* Locked_channel_ptr;
	bool Initialized;
	Channel_Call_Interface_t* call_interface_ptr;
};



#endif /* INTERFACE_CHECKER_HPP_ */
