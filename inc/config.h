#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <reg52.h>

#define FOSC 11059200UL

#define TIMER0_TH 0xFC
#define TIMER0_TL 0x66

#define MS_PER_SEC 1000

#define DEFAULT_WORK_MIN 25
#define DEFAULT_REST_MIN 5

#define TIME_MAX 60
#define TIME_MIN 1

#define DIGIT_0 0xFE
#define DIGIT_1 0xFD
#define DIGIT_2 0xFB
#define DIGIT_3 0xF7
#define DIGIT_ALL_OFF 0x0F

#define MODE_IDLE 0
#define MODE_WORK_RUN 1
#define MODE_WORK_PAUSE 2
#define MODE_WORK_DONE 3
#define MODE_REST_RUN 4
#define MODE_REST_PAUSE 5
#define MODE_SET_WORK 6
#define MODE_SET_REST 7

sbit KEY_SET = P3^2;
sbit KEY_START_STOP = P3^3;
sbit KEY_ADD = P3^4;

sbit BUZZER = P1^5;

extern unsigned char code SEG_TABLE[];
extern unsigned char display_buffer[4];

extern unsigned char mode;
extern unsigned char work_time;
extern unsigned char rest_time;
extern unsigned char min;
extern unsigned char sec;

extern unsigned int ms_count;

extern bit half_sec_flag;
extern bit sec_flag;

#endif