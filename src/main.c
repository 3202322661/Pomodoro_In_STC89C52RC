/*
	番茄钟主程序入口
	编译环境：Keil 5
	主控芯片: STC89C52RC@11.0592MHz
	
	Author: LMY
	Date: 2026-6-20
*/
#include "config.h"
#include "timer.h"
#include "display.h"
#include "button.h"
#include "buzzer.h"
#include "pomodoro.h"
#include "sleep.h"

void main()
{
	uint8_t btn;
	/* ---- 硬件初始化 ---- */
	Timer0_Init();
	Pomodoro_Init();
	Sleep_Init();
	
	Update_Display_Buffer();
	
	while (1)
	{
		/* 数码管显示缓冲更新 */
		Update_Display_Buffer();
		
		/* 按键扫描与分发 */
		btn = Button_Scan();
		if (btn != BTN_NONE)
		{
			idle_seconds = 0;
			Pomodoro_HandleButton(btn);
		}
		
		/* 秒脉冲处理 */
		if (sec_flag == 1)
		{
			sec_flag = 0;
			Pomodoro_ProcessTick();
			
			if (Sleep_IsEligible())
			{
				idle_seconds++;
				if (idle_seconds >= AUTO_SLEEP_SEC)
				{
					Sleep_EnterPowerDown();
					
					Button_Scan();
				}
			}
			else 
			{
				idle_seconds = 0;
			}
		}
	}
}