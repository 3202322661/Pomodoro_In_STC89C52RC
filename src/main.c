#include "config.h"
#include "timer.h"
#include "display.h"
#include "button.h"
#include "buzzer.h"
#include "pomodoro.h"

void main()
{
	unsigned char btn;
	
	Timer0_Init();
	Pomodoro_Init();
	
	Update_Display_Buffer();
	
	while (1)
	{
		Update_Display_Buffer();
		
		btn = Button_Scan();
		if (btn != BTN_NONE)
		{
			Pomodoro_HandleButton(btn);
		}
		
		if (sec_flag == 1)
		{
			sec_flag = 0;
			Pomodoro_ProcessTick();
		}
	}
}