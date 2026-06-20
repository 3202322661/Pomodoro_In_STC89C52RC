/**
 * button.c - 按键扫描模块
 *
 * 三个独立按键, 10ms 消抖, 按下后等待释放 (阻塞式)。
 * 按键优先级: KEY_SET > KEY_ADD > KEY_START_STOP
 *
 * 硬件: P3.2(设置), P3.3(启停), P3.4(递加), 均为低电平有效。
 */

#include "config.h"
#include "button.h"
#include "buzzer.h"

/**
 * DelayMS - 毫秒级延时 (基于指令周期粗略估算)
 *   适用于 11.0592MHz / 12T 模式。
 *   精确度 ≈ ±10%, 对按键消抖而言足够。
 */
static void DelayMS(uint16_t ms)
{
	uint16_t i, j;
	for (i = ms; i > 0; i--)
	{
		for (j = 110; j> 0; j--);
	}
}

/**
 * Button_Scan - 扫描三个按键 (阻塞式消抖)
 *
 * 消抖流程:
 *   1. 检测低电平
 *   2. 延时 10ms
 *   3. 复检低电平 (消抖确认)
 *   4. 等待按键释放 (防止连发)
 *   5. 返回按键 ID
 *
 * 返回值:
 *   BTN_SET   — KEY_SET 按下
 *   BTN_START — KEY_START_STOP 按下
 *   BTN_ADD   — KEY_ADD 按下
 *   BTN_NONE  — 无按键按下
 */
uint8_t Button_Scan()
{
	if (KEY_SET == 0)
	{
		DelayMS(10);
		if (KEY_SET == 0)
		{
			Beep_Click();//新增按键提示音
			while (KEY_SET == 0);
			return BTN_SET;
		}
	}
	
	if (KEY_ADD == 0)
	{
		DelayMS(10);
		if (KEY_ADD == 0)
		{
			Beep_Click();
			while (KEY_ADD == 0);
			return BTN_ADD;
		}
	}
	
	if (KEY_START_STOP == 0)
	{
		DelayMS(10);
		if (KEY_START_STOP == 0)
		{
			Beep_Click();
			while (KEY_START_STOP == 0);
			return BTN_START_STOP;
		}
	}
	
	return BTN_NONE;
}