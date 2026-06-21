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

#define AUTO_SLEEP_SEC 300

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

#define IAP_CMD_READ 0x01
#define IAP_CMD_WRITE 0x02
#define IAP_CMD_ERASE 0x03

#define IAP_ENABLE 0x83

/* EEPROM 地址分配 (使用扇区 0, 地址 0x0000~0x01FF)
 *   0x0000: 魔数 (0xA5 = 已初始化)
 *   0x0001: work_time (工作时长, 1~60 分钟)
 *   0x0002: rest_time (休息时长, 1~60 分钟)
 *   0x0003~0x01FF: 保留
 */
#define EEPROM_MAGIC_ADDR 0x0000
#define EEPROM_WORK_ADDR 0x0001
#define EEPROM_REST_ADDR 0x0002
#define EEPROM_MAGIC 0xA5

/* ============================================================
 * STC89C52RC 内部 EEPROM (IAP 操作寄存器)
 *
 * STC89C52RC 内置 Data Flash (EEPROM), 通过 IAP 方式读写,
 * 断电数据不丢失。用于保存用户预设的工作/休息时长。
 *
 * IAP 寄存器地址:
 *   IAP_DATA  (0xE2): 数据寄存器
 *   IAP_ADDRH (0xE3): 目标地址高字节
 *   IAP_ADDRL (0xE4): 目标地址低字节
 *   IAP_CMD   (0xE5): 命令寄存器 (0x01=读, 0x02=写, 0x03=扇区擦除)
 *   IAP_TRIG  (0xE6): 触发寄存器 (依次写 0x46, 0xB9 触发操作)
 *   IAP_CONTR (0xE7): 控制寄存器 (bit7=IAPEN, bit[4:0]=等待时间)
 * ============================================================ */
sfr IAP_DATA = 0xE2;
sfr IAP_ADDRH = 0xE3;
sfr IAP_ADDRL = 0xE4;
sfr IAP_CMD = 0xE5;
sfr IAP_TRIG = 0xE6;
sfr IAP_CONTR = 0xE7;

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
extern uint16_t idle_seconds;

extern bit half_sec_flag;
extern bit sec_flag;

extern void EEPROM_Save();
extern void EEPROM_Load();

#endif