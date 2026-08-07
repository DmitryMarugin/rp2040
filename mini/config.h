// Copyright 2023 sekigon-gonnoc (@sekigon-gonnoc)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// override backing_store_lock/unlock to control core1
#pragma weak backing_store_lock
#pragma weak backing_store_unlock

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET // Activates the double-tap behavior
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
