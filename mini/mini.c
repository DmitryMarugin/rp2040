// Copyright 2023 sekigon-gonnoc
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keyboard.h"
#include "pico/stdlib.h"
#include "bootloader.h"
#include "debug.h"
#include "c1.h"

void keyboard_pre_init_kb(void)
{
    set_sys_clock_khz(120000, true);
    keyboard_pre_init_user();
}

__attribute__((weak)) void virtser_recv(uint8_t c)
{
    if (c == 'b' || c == 'B')
    {
        bootloader_jump();
    }
    else if (c == 'd')
    {
        debug_enable = !debug_enable;
        uprintf("Debug %s\n", debug_enable ? "enabled" : "disabled");
    }

    // Можно добавить коды остальных клавишь подставив их из:
    // https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2
    else if (c=='h')
    {
        uint8_t bits[32] = {0};
        uint8_t keycode = 0x04;
        uprintf("Show matrix for scan code 0x%.2x",keycode);
        print("\nr/c 01234567\n");
        bits[keycode >> 3] |= (1 << (keycode & 0x07));
        for (uint8_t row = 0; row < MATRIX_ROWS; row++)
        {
            xprintf("%02d: ", row);
            print_bin_reverse8(bits[row]);
            printf("\n");
        }
        for (uint8_t row = 0; row < MATRIX_ROWS; row++)
        {
            if (bits[row]>0)
            {
                uprintf("matrix [%d, ",row);
                for(int i=7;i>=0;i--)
                {
                    if(bits[row]&(1<<i))
                    {
                        uprintf("%d]",i);
                        break;
                    }
                }
            }
        }
    }
}

bool backing_store_lock(void)
{
    c1_after_flash_operation();
    return true;
}

bool backing_store_unlock(void)
{
    c1_before_flash_operation();
    return true;
}
