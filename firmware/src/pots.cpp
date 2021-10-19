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
#include "pots.h"

static volatile int8_t adc_brightness, adc_color;
const int8_t pot_ch_dlt = 6;                        // pot change detection delta
int8_t brg, color;


uint8_t detect_pot_change(void)
{
    static int8_t brg_old = 0, color_old = 0;
    
    uint8_t result = 0;
    noInterrupts();
    brg = adc_brightness;
    color = adc_color;
    interrupts();

    if (brg > (brg_old + pot_ch_dlt) || brg < (brg_old - pot_ch_dlt))
    {
        result |= 0x01;
        brg_old = brg;
    }
    if (color > (color_old + pot_ch_dlt) || brg < (color_old - pot_ch_dlt))
    {
        result |= 0x02;
        color_old = color;
    }
}

uint8_t pots_get_brightness(void)
{
    return (uint8_t) brg;
}

uint8_t pots_get_color(void)
{
    return (uint8_t) color;
}




/**
 * @brief ADC0 Result read interrupt
 *      ADC0 accumulates 64 samples, thus making the result 13bit value
 *          
 * 
 */
ISR(ADC0_RESRDY_vect)
{
    switch (ADC0.MUXPOS)
    {
        case ADC_MUX_BRIGHT:
            adc_brightness = ADC0.RES >> (13-7);        // oversampled result is 13 bit, shiftR for 7 bit value
            ADC0.MUXPOS = ADC_MUX_COLOR;                // switch to the other channel
            ADC0.COMMAND = ADC_STCONV_bm;               // start new conversion
            break;
        case ADC_MUX_COLOR:
            adc_color = ADC0.RES >> (13-8);             // oversampled result is 13 bit, shiftR for 7 bit value
            ADC0.MUXPOS = ADC_MUX_BRIGHT;               // switch to the other channel
            ADC0.COMMAND = ADC_STCONV_bm;               // start new conversion       
            break;    
        default:
            break;
    }

}
