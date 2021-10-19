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
#include "power.h"
#include <avr/sleep.h>
#include "remote.h" // for
#include "leds.h"

#define V5_PWR_ON HIGH
#define V5_PWR_OFF LOW
#define V12_PWR_ON HIGH
#define V12_PWR_OFF LOW

const uint8_t V12_EN_PIN = PIN_PC0;
const uint8_t V5_EN_PIN = PIN_PA5;
static volatile uint8_t wakeup_trig = 0;

uint32_t t_start = 0, t_stop = 0;

// ----------------------------------------------------------------------------
void pwr_init(void)
{
    pinMode(V12_EN_PIN, OUTPUT);
    pinMode(V5_EN_PIN, OUTPUT);
    digitalWrite(V5_EN_PIN, V5_PWR_OFF);
    digitalWrite(V12_EN_PIN, V12_PWR_OFF);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
}
// ----------------------------------------------------------------------------
/**
 * @brief turns off the 12V and 5V power rails, enables the pin change
 *          interrrupt on the IR receive pin (used for wake up), enables
 *          the "power off" sleep mode on the MCU
 * 
 */
void pwr_off(void)
{
pwroff_start:
    leds_set_state(LEDS_PWR_OFF);
    digitalWrite(V12_EN_PIN, V12_PWR_OFF);
    digitalWrite(V5_EN_PIN, V5_PWR_OFF); // disable Aux power rails
    delay(300);
    PORTB.PIN4CTRL |= 0x01;             // enable PB4 change interrupt
    PORTB.INTFLAGS = (1<<4);            // clear the flag
    sleep_cpu();
    // 1st wake up - falling edge
    t_start = micros();
    wakeup_trig = 0;                    // clr the flag
    while (!wakeup_trig)               // wait for the falling edge
    {
        uint32_t timeout = micros();
        if (timeout - t_start > 1500u)
        {
            // falling edge taking too long, go to sleep
            goto pwroff_start;
        }
    }
    t_stop = micros();                  // falling edge timestamp
    if ((t_stop - t_start) < 300) goto pwroff_start;    // short pulse, not a valid decoded signal -> go back to sleep
    PORTB.INTFLAGS = (1<<4);            // clear the flag PB4 = IR receive pin
    PORTB.PIN4CTRL &= ~0x01;             // disable PB4 change interrupt
    digitalWrite(V5_EN_PIN, V5_PWR_ON);     // wake up complete, power on the 12V and 5V busses
    digitalWrite(V12_EN_PIN, V12_PWR_ON);
    leds_set_state(LEDS_WAKE_UP);
}

// ----------------------------------------------------------------------------
/**
 * @brief Pin change interrupt - MCU wakes up and disables the interrupt.
 *          The standard IRremote library uses timer B for pulse decoding - 
 *          No conflict here. 
 * 
 */
ISR(PORTB_PORT_vect)
{
    PORTB.INTFLAGS = (1<<4);            // clear the flag PB4 = IR receive pin
    //PORTB.PIN4CTRL &= ~0x01;             // disable PB4 change interrupt
    wakeup_trig = 1;
}
