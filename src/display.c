/**
 * display.c - 数码管显示缓冲更新模块
 *
 * 根据当前模式和闪烁标志更新 display_buffer[4]。
 * 设置模式 (MODE_SET_WORK / MODE_SET_REST) 下对应数字位以
 * 0.5s 周期闪烁 (显示/隐藏交替)。
 *
 * 显示缓冲:
 *   buffer[0]: 分钟十位
 *   buffer[1]: 分钟个位
 *   buffer[2]: 秒数十位
 *   buffer[3]: 秒数个位
 */
#include "config.h"
#include "display.h"

/**
 * Update_Display_Buffer - 更新 4 位显示缓冲
 *
 * 闪烁逻辑:
 *   MODE_SET_WORK: half_sec_flag=1 时分钟位显示空白 (闪烁)
 *   MODE_SET_REST: half_sec_flag=1 时秒位显示空白 (闪烁)
 *   其他模式: 正常显示
 */
void Update_Display_Buffer()
{
    if (mode == MODE_SET_WORK && half_sec_flag) {
        display_buffer[0] = SEG_BLANK;
        display_buffer[1] = SEG_BLANK;
    } else {
        display_buffer[0] = min / 10;
        display_buffer[1] = min % 10;
    }

    if (mode == MODE_SET_REST && half_sec_flag) {
        display_buffer[2] = SEG_BLANK;
        display_buffer[3] = SEG_BLANK;
    } else {
        display_buffer[2] = sec / 10;
        display_buffer[3] = sec % 10;
    }
}