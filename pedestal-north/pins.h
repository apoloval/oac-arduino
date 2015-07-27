/*
 * Open Airbus Cockpit - Arduino North Pedestal Sketch
 * Copyright (c) 2012-2015 Alvaro Polo
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// This is the pin assignment file. 
// Use it to configure the cable layout of your Arduino board. 

// Radio Management Panel #1
#define RMP1_DISPLAY_DIN_PIN     6
#define RMP1_DISPLAY_CLK_PIN     7
#define RMP1_DISPLAY_LOAD_PIN    8
#define RMP1_POWER_PIN           27
#define RMP1_ENC_IA_PIN          50
#define RMP1_ENC_IB_PIN          51
#define RMP1_ENC_OA_PIN          52
#define RMP1_ENC_OB_PIN          53
#define RMP1_KEYPAD_DAV_PIN      9
#define RMP1_KEYPAD_DATAA_PIN    10
#define RMP1_KEYPAD_DATAB_PIN    11
#define RMP1_KEYPAD_DATAC_PIN    12
#define RMP1_KEYPAD_DATAD_PIN    13

// ECAM panel
#define ECAM_UPPER_ROTARY_PIN    0
#define ECAM_LOWER_ROTARY_PIN    1
#define ECAM_EXP0_PINS           28, 30, 32, 34, 36, 38
#define ECAM_EXP1_PINS           29, 31, 33, 35, 37, 39

// Left MCDU
#define MCDU_BUS_PINS            2, 3, 4, 5
#define MCDU_CTRL0_PINS          40, 41
#define MCDU_CTRL1_PINS          42, 43
#define MCDU_CTRL2_PINS          44, 45
#define MCDU_CTRL3_PINS          46, 47
#define MCDU_CTRL4_PINS          48, 49
