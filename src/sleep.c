#include <intrins.h>
#include "config.h"
#include "sleep.h"
#include "timer.h"

void INT0_ISR() interrupt 0
{

}

void INT1_ISR() interrupt 2
{

}

void Sleep_Init()
{
	IT0 = 1;
	IT1 = 1;
	EX0 = 0;
	EX1 = 0;
}

bit Sleep_IsEligible()
{
	if (mode == MODE_IDLE ||
			mode == MODE_WORK_PAUSE ||
			mode == MODE_WORK_DONE ||
			mode == MODE_REST_PAUSE)
	{
		return 1;
	}
	
	return 0;
}

void Sleep_EnterPowerDown()
{
	P2 |= 0x0F;
	P0 |= 0x00;
	
	TR0 = 0;
	ET0 = 0;
	
	IE0 = 0;
	IE1 = 0;
	
	EX0 = 1;
	EX1 = 1;
	
	_nop_();
	_nop_();
	PCON |= 0x02;
	
	
	EX0 = 0;
	EX1 = 0;
	
	ms_count = 0;
	half_sec_flag = 0;
	sec_flag = 0;
	idle_seconds = 0;
	
	Timer0_Init();
}