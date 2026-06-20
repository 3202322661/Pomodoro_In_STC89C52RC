#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <reg52.h>

/*============================================================
 * 重定义所需类型
 *============================================================*/
 typedef unsigned char uint8_t;
 typedef unsigned int uint16_t;

/* ============================================================
 * 定时器参数 (Timer0, Mode 1, 16-bit)
 *
 * 目标中断周期: 1 ms
 *   机器周期 = 12 / FOSC = 12/11059200 ≈ 1.085 us
 *   1ms 计数值 = 1000 / 1.085 ≈ 921.6
 *   实际计数值 = 922 (使 1ms 误差最小)
 *   初值 = 65536 - 922 = 64614 = 0xFC66
 *   TH0 = 0xFC, TL0 = 0x66
 * ============================================================ */
#define TIMER0_TH 0xFC
#define TIMER0_TL 0x66

#define MS_PER_SEC 1000

#define DEFAULT_WORK_MIN 25
#define DEFAULT_REST_MIN 5

#define TIME_MAX 60
#define TIME_MIN 1

/* ============================================================
 * 运行模式定义
 *
 *   0  MODE_IDLE        空闲, 等待启动
 *   1  MODE_WORK_RUN    工作倒计时运行中
 *   2  MODE_WORK_PAUSE   工作倒计时暂停
 *   3  MODE_WORK_DONE    工作完成, 等待启动休息
 *   4  MODE_REST_RUN     休息倒计时运行中
 *   5  MODE_REST_PAUSE   休息倒计时暂停
 *   6  MODE_SET_WORK     设置工作时长
 *   7  MODE_SET_REST     设置休息时长
 * ============================================================ */
#define MODE_IDLE 0
#define MODE_WORK_RUN 1
#define MODE_WORK_PAUSE 2
#define MODE_WORK_DONE 3
#define MODE_REST_RUN 4
#define MODE_REST_PAUSE 5
#define MODE_SET_WORK 6
#define MODE_SET_REST 7

/* ============================================================
 * 按键引脚定义 (独立按键, 低电平有效)
 * ============================================================ */
sbit KEY_SET = P3^2;
sbit KEY_START_STOP = P3^3;
sbit KEY_ADD = P3^4;

/* ============================================================
 * 蜂鸣器引脚定义 (低电平触发)
 * ============================================================ */
sbit BUZZER = P1^5;

/* ============================================================
 * 数码管段码定义
 *   共阴数码管, P0 输出段码
 *   8 段: a(P0.0) b(P0.1) c(P0.2) d(P0.3) e(P0.4) f(P0.5) g(P0.6) dp(P0.7)
 *   段码表: 0x3F=0, 0x06=1, 0x5B=2, 0x4F=3, 0x66=4,
 *           0x6D=5, 0x7D=6, 0x07=7, 0x7F=8, 0x6F=9, 0x00=blank
 * ============================================================ */
extern uint8_t code SEG_TABLE[];

/* ============================================================
 * 数码管显示缓冲 (4 位)
 *   display_buffer[0]: 分钟十位
 *   display_buffer[1]: 分钟个位 (小数点位在此)
 *   display_buffer[2]: 秒数十位
 *   display_buffer[3]: 秒数个位
 * ============================================================ */
extern uint8_t display_buffer[4];

extern uint8_t mode;
extern uint8_t work_time;
extern uint8_t rest_time;
extern uint8_t min;
extern uint8_t sec;

extern uint16_t ms_count;

extern bit half_sec_flag;
extern bit sec_flag;

#endif