/**
 * pomodoro.c - 番茄钟核心状态机
 *
 * 8 模式状态管理系统, 严格匹配以下模式规范:
 *
 *   MODE_IDLE        (0)  空闲, 等待启动工作
 *   MODE_WORK_RUN    (1)  工作倒计时运行中
 *   MODE_WORK_PAUSE  (2)  工作倒计时暂停
 *   MODE_WORK_DONE   (3)  工作完成, 等待启动休息
 *   MODE_REST_RUN    (4)  休息倒计时运行中
 *   MODE_REST_PAUSE  (5)  休息倒计时暂停
 *   MODE_SET_WORK    (6)  设置工作时长 (分钟位闪烁)
 *   MODE_SET_REST    (7)  设置休息时长 (秒位闪烁)
 *
 * 状态转移规则:
 *   KEY_SET:    0/1/2 → 6 → 7 → 0 (循环切换)
 *   KEY_START:  0/2 → 1,  1 → 2,  3/5 → 4,  4 → 5
 *   KEY_ADD:    仅 mode=6 时递增 work_time, mode=7 时递增 rest_time
 *               (范围 1~60, 超出回绕至 1)
 */

#include "config.h"
#include "pomodoro.h"
#include "button.h"
#include "buzzer.h"

void Pomodoro_Init()
{
	mode = MODE_IDLE;
	work_time = DEFAULT_WORK_MIN;
	rest_time = DEFAULT_REST_MIN;
	min = work_time;
	sec = 0;
	BUZZER = 0;
}

void Pomodoro_HandleButton(uint8_t btn)
{
	if (btn == BTN_NONE) return;
	
	if (btn == BTN_SET)
	{
	if (mode == MODE_IDLE || 
			mode == MODE_WORK_RUN || 
			mode == MODE_WORK_PAUSE) 
		{
			mode = MODE_SET_WORK;
			min  = work_time;
			sec  = 0;
		} else if (mode == MODE_SET_WORK) {
			mode = MODE_SET_REST;
			min  = rest_time;
			sec  = 0;
		} else if (mode == MODE_SET_REST) {
      mode = MODE_IDLE;
      min  = work_time;
      sec  = 0;
		}
		return;
	}
	
	if (btn == BTN_ADD)
	{
		if (mode == MODE_SET_WORK)
		{
			work_time++;
			if (work_time > TIME_MAX)
			{
				work_time = TIME_MIN;
			}
			min = work_time;
		}
		else if (mode == MODE_SET_REST)
		{
			rest_time++;
			if (rest_time > TIME_MAX)
			{
				rest_time = TIME_MIN;
			}
			min = rest_time;
		}
		return;
	}
	
	if (btn == BTN_START_STOP)
	{
		if (mode == MODE_IDLE || mode == MODE_WORK_PAUSE)
		{
			mode = MODE_WORK_RUN;
		}
		else if (mode == MODE_WORK_RUN)
		{
			mode = MODE_WORK_PAUSE;
		}
		else if (mode == MODE_WORK_DONE || mode == MODE_REST_PAUSE)
		{
			mode = MODE_REST_RUN;
		}
		else if (mode == MODE_REST_RUN)
		{
			mode = MODE_REST_PAUSE;
		}
		return;
	}	
}

void Pomodoro_ProcessTick()
{
	if (mode != MODE_WORK_RUN && mode != MODE_REST_RUN)
	{
		return;
	}
	
	if (sec == 0)
	{
		if (min == 0)
		{
			Beep_Alarm();
			
			if (mode == MODE_WORK_RUN)
			{
				mode = MODE_WORK_DONE;
				min = rest_time;
				sec = 0;
			} 
			else 
			{
				mode = MODE_IDLE;
				min = work_time;
				sec = 0;
			}
		}
		else
		{
			min--;
			sec = 59;
		}
	}
	else 
	{
		sec--;
	}
}