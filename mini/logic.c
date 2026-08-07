#include QMK_KEYBOARD_H
#include "my_keycodes.h"
#include "display.h"
#include "musical_notes.h"
extern audio_config_t audio_config;

void layer_move_step(bool);
void SAPtime_confirm(uint8_t);
bool show_help = false;
float leadstart[][2] = SONG(CAPS_LOCK_ON_SOUND );
float kc_no_beep[][2] = SONG(M__NOTE(_C5,20),M__NOTE(_CS5,20));
float leader_fail_song[][2] = SONG(NUM_LOCK_OFF_SOUND);
float base_layer_song[][2] = SONG(MUSIC_ON_SOUND);
float other_layer_song[][2] = SONG(MUSIC_OFF_SOUND);
float macro_run_beep[][2] = SONG(TERMINAL_SOUND);
float macro_end_beep[][2] = SONG(MAJOR_SOUND);

static uint16_t blink_timer;


// #include "eeprom_driver.h"


#define LAYER_CYCLE_START 0
#define LAYER_CYCLE_END   3


/*-----------------------------------------
            Lighting Layers
-----------------------------------------*/
const rgblight_segment_t PROGMEM rgb_color1[] = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_GREEN});
const rgblight_segment_t PROGMEM rgb_color2[] = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_BLUE});
const rgblight_segment_t PROGMEM rgb_color3[] = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_CYAN});
const rgblight_segment_t PROGMEM rgb_color4[] = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_YELLOW});
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(rgb_color1,rgb_color2,rgb_color3,rgb_color4);

void keyboard_post_init_user(void)
{
    clicky_off();
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(HSV_CYAN);
    //rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
    //rgblight_mode_noeeprom(RGBLIGHT_MODE_RGB_TEST);
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
    // oled_write_P(PSTR("Default\n"), false);
    // PLAY_SONG(macro_end_beep); 
}

void matrix_scan_user(void)
{
    if (leader_sequence_active())
    {
        //Подсвечиваем нашу Leader Key
        if (timer_elapsed(blink_timer) < 50)
        {
            rgblight_enable();
            SEND_STRING("\a");
        }
        else if (timer_elapsed(blink_timer) < 100)
        {
            rgblight_disable();
        }
        else
        {
            blink_timer = timer_read();
        }
    }
}

layer_state_t layer_state_set_user(layer_state_t state)
{
    rgblight_set_layer_state(0, layer_state_cmp(state, _ONE));
    rgblight_set_layer_state(1, layer_state_cmp(state, _TWO));
    rgblight_set_layer_state(2, layer_state_cmp(state, _THREE));
    rgblight_set_layer_state(3, layer_state_cmp(state, _FOUR));


    switch (get_highest_layer(state))
    {
        case _ONE:
            PLAY_SONG(base_layer_song); 
            break;
        case _TWO:
            PLAY_SONG(other_layer_song);
            break;
        case _THREE:
            PLAY_SONG(other_layer_song);
            break;
        case _FOUR:
            PLAY_SONG(other_layer_song);
            break;
        default: //  for any other layers, or the default layer
            break;
    }
    return state;
}



bool jump_active = false;
uint32_t jump_timer = 0;


bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode)
    {
        // ---Leader key
        case KC_SCLN:
            if (record->event.pressed){
                if (get_mods() & MOD_BIT(KC_LALT) && get_mods() & MOD_BIT(KC_LCTL) && get_mods() & MOD_BIT(KC_LSFT)) //KC_LALT|KC_LCTL|
                {
                    del_mods(MOD_MASK_CSA);
                    leader_start();
                    return false; // Not send key
                }
                else{
                    return true; // if not all mods, then process as usual
                }
            }
            else{
                return true; //if they were released, we process it
            }
        // ---Help to OLED
        case KC_SLASH:
        {
            if (record->event.pressed){
                if (get_mods() & MOD_BIT(KC_LALT) && get_mods() & MOD_BIT(KC_LCTL) && get_mods() & MOD_BIT(KC_LSFT)){
                    del_mods(MOD_MASK_CSA);
                    show_help = true;
                    return false; // Not send key
                }
                else{
                    return true; // if not all mods, then process as usual
                }
            }
            else{
                return true; //if they were released, we process it
            }
        }
        // ---Close help
        case KC_ESC:
        {
            if (record->event.pressed){
                if (show_help){
                    show_help=!show_help;
                    // oled_clear();
                    return false;
                }
                else return true;
            }
            else{
                return true; //if they were released, we process it
            }
        }
        // ---SAP macro keys
        case SAP_1:
        case SAP_2:
        case SAP_3:
        case SAP_4:
        case SAP_5:
        case SAP_6:
        case SAP_7:
        case SAP_8:
        case SAP_9:
        case SAP_10:
        {
            if (record->event.pressed){
                uint8_t cnt = keycode - SAFE_RANGE + 1;
                SAPtime_confirm(cnt);
            }
            return false;
        }
        case CS_Jump:
        {
            jump_active = !jump_active;
            if (record->event.pressed){
                jump_timer = timer_read32(); // Reset spam timer
                SEND_STRING("\a" SS_TAP(X_W) SS_TAP(X_SPC) SS_DELAY(350));
                return true;
            }else{

                SEND_STRING("\a");
                return true;
            }

        }

        case KC_NO:
        {
            PLAY_SONG(kc_no_beep);
            return true;
        }
        default:
            return true;
    }
}


/*-----------------------------------------
            Leader Key
-----------------------------------------*/
void leader_start_user(void)
{
        PLAY_SONG(leadstart);
        blink_timer = timer_read();
}

void leader_end_user(void)
{
    bool did_leader_succeed = false;

    if (leader_sequence_three_keys(KC_B, KC_I, KC_P))
    {
        did_leader_succeed = true;
        PLAY_SONG(kc_no_beep);
        clicky_toggle();
    }
    if (leader_sequence_one_key(KC_1))
    {
        did_leader_succeed = true;
        layer_move(_ONE);
    }
    if (leader_sequence_one_key(KC_2))
    {
        did_leader_succeed = true;
        layer_move(_TWO);
    }
    if (leader_sequence_one_key(KC_3))
    {
        did_leader_succeed = true;
        layer_move(_THREE);
    }
    if (leader_sequence_one_key(KC_4))
    {
        did_leader_succeed = true;
        layer_move(_FOUR);
    }
    if (leader_sequence_one_key(KC_RIGHT))
    {
        did_leader_succeed = true;
        layer_move_step(false);
    }

    if (leader_sequence_one_key(KC_LEFT))
    {
        did_leader_succeed = true;
        layer_move_step(true);
    }

    if (did_leader_succeed)
    {

    }
    else
    {
        PLAY_SONG(leader_fail_song);
    }
    rgblight_enable();
}

/*-----------------------------------------*
 *        Circle layers switch             *
 *-----------------------------------------*/
void layer_move_step(bool left)
{
    int8_t next_layer;
    uint8_t current_layer = get_highest_layer(layer_state);
    // Проверяем что мы не на каком-то слое вне границ
    if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
        return;
    }
    if (left){
        next_layer = current_layer - 1;
        if (next_layer < LAYER_CYCLE_START) {
            next_layer = LAYER_CYCLE_END;
        }
    }
    else{
        next_layer = current_layer + 1;
        if (next_layer > LAYER_CYCLE_END) {
            next_layer = LAYER_CYCLE_START;
        }
    }
    layer_move(next_layer);
}
/*-----------------------------------------*
 *        SAP Macro                        *
 *-----------------------------------------*/
void SAPtime_confirm(uint8_t cnt)
{
    for (uint8_t i = 0;i<cnt;i++)
    {
            PLAY_SONG(macro_run_beep);
            SEND_STRING(SS_DELAY(200));
    }
    for (uint8_t i = 0;i<cnt;i++)
    {
            SEND_STRING(SS_DOWN(X_LCTL) SS_DOWN(X_LSFT) SS_DOWN(X_F8)
                        SS_UP(X_LCTL) SS_UP(X_LSFT) SS_UP(X_F8)
                        SS_DELAY(1000)
                        SS_TAP(X_TAB) SS_TAP(X_TAB) SS_TAP(X_TAB) SS_TAP(X_TAB) SS_TAP(X_TAB) SS_TAP(X_TAB)
                        SS_DELAY(100)
                        SS_TAP(X_SPC)
                        SS_TAP(X_TAB)
                        SS_TAP(X_SPC)
                        SS_LCTL("s")
                        SS_DELAY(1000)
                        SS_TAP(X_DOWN)
                    );
    }
    PLAY_SONG(macro_end_beep);
}

