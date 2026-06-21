#include <intrins.h>
#include "config.h"
#include "eeprom.h"


static void IAP_Idle();
static void IAP_WriteByte(uint16_t addr, uint8_t dat);
static void IAP_EraseSector(uint16_t addr);
static uint8_t IAP_ReadByte(uint16_t addr);

static void IAP_Idle()
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0;
}

static void IAP_WriteByte(uint16_t addr, uint8_t dat)
{
	IAP_CONTR = IAP_ENABLE;
	IAP_CMD = IAP_CMD_WRITE;
	IAP_ADDRL = (uint8_t) (addr & 0xFF);
	IAP_ADDRH = (uint8_t) (addr >> 8);
	IAP_DATA = dat;
	IAP_TRIG = 0x46;
	IAP_TRIG = 0xB9;
	_nop_();
	
	IAP_Idle();
}

static void IAP_EraseSector(uint16_t addr)
{
	IAP_CONTR = IAP_ENABLE;
	IAP_CMD = IAP_CMD_ERASE;
	
	IAP_ADDRL = (uint8_t)(addr & 0xFF);
  IAP_ADDRH = (uint8_t)(addr >> 8);
  IAP_TRIG  = 0x46;
  IAP_TRIG  = 0xB9;
  _nop_();
	
  IAP_Idle();
}

static uint8_t IAP_ReadByte(uint16_t addr)
{
    uint8_t dat;

    IAP_CONTR = IAP_ENABLE;
    IAP_CMD   = IAP_CMD_READ;
    IAP_ADDRL = (uint8_t)(addr & 0xFF);
    IAP_ADDRH = (uint8_t)(addr >> 8);
    IAP_TRIG  = 0x46;
    IAP_TRIG  = 0xB9;
    _nop_();
	
    dat = IAP_DATA;
	
    IAP_Idle();
    return dat;
}

void EEPROM_Save()
{
	EA = 0;
	
	IAP_EraseSector(EEPROM_MAGIC_ADDR);
	IAP_WriteByte(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);
	IAP_WriteByte(EEPROM_WORK_ADDR, work_time);
	IAP_WriteByte(EEPROM_REST_ADDR, rest_time);
	
	EA = 1;
}

void EEPROM_Load()
{
	uint8_t magic;
	
	EA = 0;
	
	magic = IAP_ReadByte(EEPROM_MAGIC_ADDR);
	
	if (magic == EEPROM_MAGIC)
	{
		work_time = IAP_ReadByte(EEPROM_WORK_ADDR);
		rest_time = IAP_ReadByte(EEPROM_REST_ADDR);
		
		if (work_time < TIME_MIN || work_time > TIME_MAX)
		{
			work_time = DEFAULT_WORK_MIN;
		}
		
		if (rest_time < TIME_MIN || rest_time > TIME_MAX)
		{
			rest_time = DEFAULT_REST_MIN;
		}
	}
	else 
	{
		IAP_EraseSector(EEPROM_MAGIC_ADDR);
		IAP_WriteByte(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);
		IAP_WriteByte(EEPROM_WORK_ADDR, DEFAULT_WORK_MIN);
		IAP_WriteByte(EEPROM_REST_ADDR, DEFAULT_REST_MIN);
	}
	
	EA = 1;
}