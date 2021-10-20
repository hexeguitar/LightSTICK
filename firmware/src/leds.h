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

#ifndef _LEDS_H
#define _LEDS_H

#include<Arduino.h>

#define LEDS_BRIGHTNESS_STEPS   16
#define LEDS_BRIGHTNESS_STEP   (256/LEDS_BRIGHTNESS_STEPS)

#define LEDS_COLORTEMP_STEPS   16
#define LEDS_COLORTEMP_STEP   (256/LEDS_BRIGHTNESS_STEPS)


typedef enum
{
    LEDS_PWR_OFF,
    LEDS_WAKE_UP,
    LEDS_PWR_ON
}leds_state_t;

void leds_init(void);
void leds_process(void);

void leds_set_max(void);
void leds_brightness_inc(void);
void leds_brightness_dec(void);
void leds_colortemp_warm(void);
void leds_colortemp_cold(void);

void leds_set_state(leds_state_t s);
leds_state_t leds_get_state(void);
void leds_set_preset(uint8_t preset);

#endif // _LEDS_H
