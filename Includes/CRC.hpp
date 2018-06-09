/*
 * CRC.hpp
 *
 * Created: 02.11.2017 11:57:25
 *  Author: Pawlow_nic, Stalker1290
 */ 

#ifndef CRC_HPP_
#define CRC_HPP_

#include "chip.h"

class CRC_t
{
public:
	static uint16_t Crc16(uint8_t* pcBlock, uint32_t len);
	static uint16_t Crc16(uint8_t* pcBlock, uint32_t len, uint16_t startFill);
	//my CRC
	static uint16_t crc16(uint16_t crc, uint8_t* dataPointer, uint16_t count);
	static uint8_t crc8(uint8_t crc, uint8_t* dataPointer, uint16_t count);

private:
	static uint16_t crc16_xmodem_update(uint16_t crc, uint8_t data);
	static const uint8_t crc8Table[256];
};

#endif /* CRC_HPP_ */
