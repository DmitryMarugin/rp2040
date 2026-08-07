#include QMK_KEYBOARD_H
#include "my_keycodes.h"
#include "display.h"  // Добавьте эту строку
#ifdef OLED_ENABLE
// OLED RP2040
extern bool show_help;
extern uint8_t kbd_addr;
extern uint8_t kbd_instance;
extern uint8_t hid_instance;
extern uint8_t mouse_addr;
extern uint8_t mouse_instance;

void render_wpm(void);
void render_layer(void);
void render_led_status(void);
void render_main_display(void);
void render_help_display(void);
void render_icons(void);

static void print_logo(void)
{
    oled_write_raw_P(qmk_my_logo, sizeof(qmk_my_logo));
}

oled_rotation_t oled_init_user(oled_rotation_t rotation)
{
    //oled_scroll_set_area(0, 7);
    //oled_scroll_set_speed(scrolling_speed);
    return OLED_ROTATION_0;
}



static int iii = 0;
    bool oled_task_user(void)
    {
        if (iii == 5)
        {
            oled_set_cursor(0 , 1);
            print_logo();
        }
        if (iii == 40)
        {
            oled_clear();
        }
        if (iii >= 41)
        {
            if (!show_help){
                render_main_display();
            }
            else{
                render_help_display();
            }
        }
        if (++iii > 55) iii = 50;
        return false;
    }

    void render_main_display()
    {
        /*-------------------------------------
        |   Words per minutes (1 row)         |
        -------------------------------------*/
        render_wpm();
        render_icons();
        /*-------------------------------------
        |   Layer state (3 rows, 128x24 image)|
        -------------------------------------*/
        render_layer();
        oled_set_cursor(0, 5);

        /*-------------------------------------|
        |   Help row (1 row)                   |
        |-------------------------------------*/
        oled_set_cursor(0, 6);
        oled_write_raw_P(raw_help_str, sizeof(raw_help_str));

        /*------------------------------------|
        |   Host Keyboard LED Status(1 row)   |
        |------------------------------------*/
        render_led_status();
    }

    void render_help_display()
    {
        oled_clear();
        oled_set_cursor(0, 0);
        oled_write_raw_P(raw_help_leader1, sizeof(raw_help_leader1));
    }
    void render_wpm()
    {
        oled_set_cursor(0, 0);
        char tmp[64];
        memset(tmp,0, sizeof(tmp));
        uint8_t n = get_current_wpm();
        char    wpm_counter[4];
        wpm_counter[3] = '\0';
        wpm_counter[2] = '0' + n % 10;
        wpm_counter[1] = (n /= 10) % 10 ? '0' + (n) % 10 : (n / 10) % 10 ? '0' : ' ';
        wpm_counter[0] = n / 10 ? '0' + n / 10 : ' ';
        oled_write_P(PSTR("WPM: "), false);
        oled_write(wpm_counter, false);
        oled_write_P(PSTR("  "), false);
    }

    void render_icons()
    {
         //kb and mouse icons
        if (kbd_addr>0){
            oled_write_raw_P(kb_icon,sizeof(kb_icon));
        }
        else{
            oled_write_raw_P(blank_16x8,sizeof(blank_16x8));
        }
        oled_set_cursor(OLED_FONT_WIDTH*2+1, 0);
        if (mouse_addr>0){
            oled_write_raw_P(mouse_icon,sizeof(mouse_icon));
        }
        else{
            oled_write_raw_P(blank_16x8,sizeof(blank_16x8));
        }
        //Цифирку с номером слоя в уголок
        oled_set_cursor((OLED_DISPLAY_WIDTH / OLED_FONT_WIDTH)-1, 0);
        char buff[4];
        sprintf(buff,"%d",get_highest_layer(layer_state));
        oled_write_P(buff,false);
    }

    void render_layer()
    {
        oled_set_cursor(0, 2);
        switch (get_highest_layer(layer_state))
        {
            case _ONE:
                //oled_write_P(PSTR("BASE\n"), false);
                oled_write_raw_P(raw_base_layer, sizeof(raw_base_layer));
                break;
            case _TWO:
                oled_write_raw_P(raw_sap_layer, sizeof(raw_sap_layer));
                break;
            case _THREE:
                oled_write_raw_P(raw_dota_layer, sizeof(raw_dota_layer));
                break;
            case _FOUR:
                oled_write_raw_P(raw_cs_layer, sizeof(raw_cs_layer));
                break;
            default:
                // Or use the write_ln shortcut over adding '\n' to the end of your string
                oled_write_ln_P(PSTR("Undefined"), false);
        }
    }

    void render_led_status()
    {
        oled_set_cursor(0 + REAL_OLED_CURSOL_COL_OFFSET, 7);
        led_t led_state = host_keyboard_led_state();
        //oled_write_raw_P(led_state.num_lock ? num_logo : num_logo_active,sizeof(num_logo));
        oled_write_P(led_state.num_lock ? PSTR("Num") : PSTR("Num"), led_state.num_lock);
        oled_write(" ", false);
        oled_write_P(led_state.caps_lock ? PSTR("Caps") : PSTR("Caps"), led_state.caps_lock);
        oled_write(" ", false);
        oled_write_P(led_state.scroll_lock ? PSTR("Sc") : PSTR("Sc"), led_state.scroll_lock);
        // oled_set_cursor(0, 7);
        // char buff[64];
        // sprintf(buff,"A:%d I:%d | A:%d I:%d",kbd_addr,kbd_instance,mouse_addr,mouse_instance);
        // oled_write_ln_P(buff,false);
    }

#endif
