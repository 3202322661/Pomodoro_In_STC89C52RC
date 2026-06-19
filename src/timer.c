/**
 * timer.c - Timer0 中断服务模块
 *
 * Timer0 Mode 1 (16-bit), 1ms 精确定时 @ 11.0592MHz。
 * ISR 完成两项核心任务:
 *   1. 动态 4 位共阴数码管扫描 (每 1ms 切换一位)
 *   2. 毫秒累加 → 500ms 闪烁标志 + 1s 秒标志
 *
 * 定时器初值计算:
 *   机器周期 = 12/11059200 ≈ 1.085 us
 *   1ms 计数值 = 1000/1.085 ≈ 922
 *   初值 = 65536 - 922 = 64614 = 0xFC66
 */

#include "config.h"
#include "timer.h"

unsigned char code SEG_TABLE[] = {
	0x3F,
	0x06,
	0x5B,
	0x4F,
	0x66,
	0x6D,
	0x7D,
	0x07,
	0x7F,
	0x6F,
	0x00
};

unsigned char display_buffer[4] = {0,0,0,0};

unsigned char mode = MODE_IDLE;
unsigned char work_time = DEFAULT_WORK_MIN;
unsigned char rest_time = DEFAULT_REST_MIN;
unsigned char min = DEFAULT_WORK_MIN;
unsigned char sec = 0;

unsigned int ms_count = 0;
bit half_sec_flag = 0;
bit sec_flag = 0;

void Timer0_Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = TIMER0_TH;
	TL0 = TIMER0_TL;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
}

void Timer0_ISR() interrupt 1
{
	static unsigned char digit = 0;
	
	TH0 = TIMER0_TH;
	TL0 = TIMER0_TL;
	
	P2 |= 0x0F;
	P0 = 0x00;
	
	switch(digit)
	{
		case 0: 
			P0 = SEG_TABLE[display_buffer[0]];
			P2 &= ~0x01;
		break;
		case 1:
			P0 = SEG_TABLE[display_buffer[1]];
			if ((mode == MODE_WORK_RUN || mode == MODE_REST_RUN) &&
					half_sec_flag)
			{
				P0 |= 0x80;
			} else if (mode == MODE_IDLE || mode == MODE_WORK_PAUSE ||
									mode == MODE_WORK_DONE || mode == MODE_REST_PAUSE)
			{
				P0 |= 0x80;
			}
			P2 &= ~0x02;
		break;
		case 2:
			P0 = SEG_TABLE[display_buffer[2]];
			P2 &= ~0x04;
		break;
		case 3:
			P0 = SEG_TABLE[display_buffer[3]];
			P2 &= ~0x08;
		break;
	}
	
	digit++;
	if (digit > 3)
	{
		digit = 0;
	}
	
	ms_count++;
	if (ms_count == 500)
	{
		half_sec_flag = ~half_sec_flag;
	}
	
	if (ms_count >= MS_PER_SEC)
	{
		ms_count = 0;
		sec_flag = 1;
	}
}