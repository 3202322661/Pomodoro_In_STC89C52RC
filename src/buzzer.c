#include "config.h"
#include "buzzer.h"

static void DelayMS(unsigned int ms)
{
	unsigned int i, j;
	for (i = ms; i > 0; i--)
	{
		for (j = 110; j> 0; j--);
	}
}

void Beep_Alarm() 
{
	unsigned int i, j;
	for (j = 0; j < 5; j++) 
	{
		for (i = 0; i < 300; i++) 
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