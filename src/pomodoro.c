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

void Pomodoro_HandleButton(unsigned char btn)
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