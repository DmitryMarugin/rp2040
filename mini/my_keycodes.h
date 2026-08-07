#pragma once
#include QMK_KEYBOARD_H
// Кастомные клавиши
enum custom_keycodes {
    SAP_1 = SAFE_RANGE,
    SAP_2,
    SAP_3,
    SAP_4,
    SAP_5,
    SAP_6,
    SAP_7,
    SAP_8,
    SAP_9,
    SAP_10,
    CS_Jump
};

// Названия слоев
enum layers {
    _ONE,
    _TWO,
    _THREE,
    _FOUR
};