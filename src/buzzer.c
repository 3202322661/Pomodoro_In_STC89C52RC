/**
 * buzzer.c - 蜂鸣器驱动模块
 *
 * 有源蜂鸣器 (P1.5, 低电平触发)。
 * 阻塞式告警音: 5 次短鸣 (每鸣约 300 周期高低翻转), 间隔 100ms。
 *
 * 注意: Beep_Alarm() 在倒计时归零时调用, 会阻塞主循环约 1 秒,
 *       在此时间内 Timer0 ISR 仍继续运行 (数码管扫描不中断)。
 */
#include "config.h"
#include "buzzer.h"

static void DelayMS(uint16_t ms)
{
	uint16_t i, j;
	for (i = ms; i > 0; i--)
	{
		for (j = 110; j> 0; j--);
	}
}

void Beep_Click()
{
	uint8_t i;
	for (i = 0; i < 30; i++)
	{
		BUZZER = 1;
    DelayMS(1);
    BUZZER = 0;
    DelayMS(1);
	}
	
	BUZZER = 0;
}

void Beep_Alarm() 
{
	uint8_t i, j;
	for (j = 0; j < 5; j++) 
	{
		for (i = 0; i < 255; i++) 
		{
			BUZZER = 1;
			DelayMS(1);
			BUZZER = 0;
			DelayMS(1);
		}
		
		DelayMS(100);
	}
	
	BUZZER = 0;
}