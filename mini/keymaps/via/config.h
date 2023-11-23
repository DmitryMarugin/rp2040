//Leader Key
#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 1000

#define VERBOSE_DEBUG 0

//for VIA
// #define EEPROM_SIZE 4096
// #define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 2047


#ifdef AUDIO_ENABLE
    #define AUDIO_PIN GP7
    #define AUDIO_PWM_DRIVER PWMD3
    #define AUDIO_PWM_CHANNEL RP2040_PWM_CHANNEL_B
    #define AUDIO_INIT_DELAY 50
    #define STARTUP_SONG SONG(NO_SOUND)
    #define AUDIO_CLICKY
    #define NO_MUSIC_MODE

    // ARM Audio Volume
    #define AUDIO_DAC_SAMPLE_MAX 0U //4095U
#endif


//Display OLED SSD1306

#define OLED_BRIGHTNESS 128
#define OLED_IC OLED_IC_SSD1306
#define OLED_SCROLL_TIMEOUT 0
#define OLED_TIMEOUT 60000
// settings for the oled 128x64 0.96
#define OLED_DISPLAY_128X64
#define REAL_OLED_DISPLAY_WIDTH 128
#define REAL_OLED_DISPLAY_HEIGHT 64
#define REAL_OLED_COLUMN_OFFSET (28 - OLED_COLUMN_OFFSET)
#define REAL_OLED_CURSOL_COL_OFFSET (REAL_OLED_COLUMN_OFFSET / OLED_FONT_WIDTH)
#define OLED_UPDATE_INTERVAL 100

// RP2040 I2C I2C0
// settings for 0.96" OLED display
#define I2C_DRIVER I2CD0
#define I2C1_SDA_PIN GP28
#define I2C1_SCL_PIN GP29

//#define UNICODE_SELECTED_MODES UNICODE_MODE_WINDOWS

// #define WS2812_DI_PIN GP16
// #define BACKLIGHT_PIN GP16
// #define BACKLIGHT_PWM_DRIVER PWMD0
// #define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A
// #define WS2812_PIO_USE_PIO0
// #define WS2812_DMA_CHANNEL 2

// #define WS2812_PWM_DRIVER PWMD0  // default: PWMD2
// #define WS2812_PWM_CHANNEL RP2040_PWM_CHANNEL_A  // default: 2
// #define WS2812_PWM_PAL_MODE 2  // Pin "alternate function", see the respective datasheet for the appropriate values for your MCU. default: 2
// #define NOP_FUDGE 0.4
// //#define WS2812_PWM_COMPLEMENTARY_OUTPUT // Define for a complementary timer output (TIMx_CHyN); omit for a normal timer output (TIMx_CHy).
// #define WS2812_DMA_STREAM STM32_DMA1_STREAM2  // DMA Stream for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.
// #define WS2812_DMA_CHANNEL 2  // DMA Channel for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.
// #define WS2812_DMAMUX_ID STM32_DMAMUX1_TIM2_UP // DMAMUX configuration for TIMx_UP -- only required if your MCU has a DMAMUX peripheral, see the respective reference manual for the appropriate values for your MCU.


//#define NOP_FUDGE 1
#ifdef WS2812_DI_PIN
    # define RGBLED_NUM 1
    # define RGBLIGHT_HUE_STEP 8
    # define RGBLIGHT_SAT_STEP 8
    # define RGBLIGHT_VAL_STEP 8
    # define RGBLIGHT_LIMIT_VAL 255
    # define RGBLIGHT_SLEEP
    // /*== all animations enable ==*/
    #define RGBLIGHT_ANIMATIONS
    // #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RGB_TEST
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#endif
