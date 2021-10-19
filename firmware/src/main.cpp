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
#include <Arduino.h>
#include "leds.h"
#include "pots.h"
#include "power.h"
#include "remote.h"
#include "Wire.h"


ir_cmd_t ir_cmd;

void setup()
{
    Serial.swap(1);         // use alternate pin mapping
    Serial.begin(115200);
    Serial.println(F("LightSTICK"));
    pwr_init();
    leds_init();
    remote_init();
    pwr_off();
}

void loop()
{

    ir_cmd = remote_process();
    switch (ir_cmd)
    {
    case CMD_IDLE:
        break;
    case CMD_PWR_TGL:
        switch (leds_get_state())
        {
            case LEDS_PWR_OFF:
            case LEDS_WAKE_UP:
                leds_set_state(LEDS_PWR_ON);
                break;
            case LEDS_PWR_ON:
                pwr_off();
                break;        
            default:
                break;
        }
        break;
    case CMD_BRIGHT_INC:
        leds_brightness_inc();
        break;
    case CMD_BRIGHT_DEC:
        leds_brightness_dec();
        break;
    case CMD_BRIGHT_MAX:
        leds_set_max();
        break;
    case CMD_COLOR_WARMER:
        leds_colortemp_warm();
        break;
    case CMD_COLOR_COLDER:
        leds_colortemp_cold();
        break;
    case CMD_PRESET_0:
        leds_set_preset(0);
        break;
    case CMD_PRESET_1:
        leds_set_preset(1);
        break;
    case CMD_PRESET_2:
        leds_set_preset(2);
        break;
    case CMD_PRESET_3:
        leds_set_preset(3);
        break;
    case CMD_PRESET_4:
        leds_set_preset(4);
        break;
    case CMD_PRESET_5:
        leds_set_preset(5);
        break;
    case CMD_PRESET_6:
        leds_set_preset(6);
        break;
    case CMD_PRESET_7:
        leds_set_preset(7);
        break;
    case CMD_PRESET_8:
        leds_set_preset(8);
        break;
    case CMD_PRESET_9:
        leds_set_preset(9);
        break;
    case CMD_RESET_MCU:
        _PROTECTED_WRITE(RSTCTRL.SWRR,1);
        break;
    default:
        break;
    }

    leds_process();


}


