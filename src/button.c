#include "config.h"
#include "button.h"

static void DelayMS(unsigned int ms)
{
	unsigned int i, j;
	for (i = ms; i > 0; i--)
	{
		for (j = 110; j> 0; j--);
	}
}

unsigned char Button_Scan()
{
	if (KEY_SET == 0)
	{
		DelayMS(10);
		if (KEY_SET == 0)
		{
			while (KEY_SET == 0);
			return BTN_SET;
		}
	}
	
	if (KEY_ADD == 0)
	{
		DelayMS(10);
		if (KEY_ADD == 0)
		{
			while (KEY_ADD == 0);
			return BTN_ADD;
		}
	}
	
	if (KEY_START_STOP == 0)
	{
		DelayMS(10);
		if (KEY_START_STOP == 0)
		{
			while (KEY_START_STOP == 0);
			return BTN_START_STOP;
		}
	}
	
	return BTN_NONE;
}