#include "config.h"
#include "display.h"

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