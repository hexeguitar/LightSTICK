/*
 * Copyright (c) 2021 by Piotr Zapart / www.hexefx.com
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list 
 * of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this 
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *  
 * Neither the name of the Piotr Zapart or HEXE FX nor the names of its 
 * contributors may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 */
#ifndef _REMOTE_H
#define _REMOTE_H
#include <Arduino.h>

//#define IR_DEBUG_OUT_ON         // uncomment to print out the irremote output

#define DECODE_RC5              // IR remote protocol
#define IR_INPUT_PIN PIN_PB4    // IR receive pin


/**
 * @brief IR remote command table. 
 * 
 */
typedef enum
{
    CMD_IDLE            = 0x00,
    CMD_PWR_TGL         = 0x39,     // key POWER
    CMD_BRIGHT_INC      = 0x03,     // key +
    CMD_BRIGHT_DEC      = 0x09,     // key -
    CMD_BRIGHT_MAX      = 0x30,     // key PLAY
    CMD_COLOR_WARMER    = 0x06,     // key UP
    CMD_COLOR_COLDER    = 0x0C,     // key DOWN
    CMD_COLOR_WARM      = 0x33,     // key FWD
    CMD_COLOR_COLD      = 0x2D,     // key REW
    CMD_PRESET_0        = 0x27,     // key 0
    CMD_PRESET_1        = 0x0F,     // key 1
    CMD_PRESET_2        = 0x15,     // key 2
    CMD_PRESET_3        = 0x10,     // key 3
    CMD_PRESET_4        = 0x18,     // key 4
    CMD_PRESET_5        = 0x1B,     // key 5
    CMD_PRESET_6        = 0x1E,     // key 6
    CMD_PRESET_7        = 0x11,     // key 7
    CMD_PRESET_8        = 0x21,     // key 8
    CMD_PRESET_9        = 0x12,     // key 9
    CMD_RESET_MCU       = 0x3C,     // key STOP
    CMD_STORE_PRESET    = 0x36      // key REC
}ir_cmd_t;

void remote_init(void);
ir_cmd_t remote_process(void);

#endif // _REMOTE_H
