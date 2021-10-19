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
#include "leds.h"
#include <EEPROM.h>

#define WARM_PIN PIN_PB0
#define COLD_PIN PIN_PB1

#define EEP_FACT_MARKER         (0xA512)
#define AUTOSAVE_DELAY_MS       (10000l)    // 10 sec delay to save last used preset in eeprom

// EEPROM addresses
uint8_t EEMEM eep_preset_table[2*10];
uint8_t EEMEM eep_last_preset;
uint16_t EEMEM eep_fact_marker;


const uint8_t fact_preset_table[2*10] = 
{
    0x10, 0x7F,         // Preset 0 = 10% bright, cold+warm white
    0xFF, 0x00,         // Preset 1 = 100% bright, cold white
    0x7F, 0x00,         // Preset 2 = 50% bright, cold white
    0x33, 0x00,         // Preset 3 = 20% bright, cold white
    0xFF, 0xFF,         // Preset 4 = 50% bright, warm white
    0x7F, 0xFF,         // Preset 5 = 20% bright, warm white
    0x33, 0xFF,         // Preset 6 = 100% bright, cold+warm white
    0xFF, 0x7F,         // Preset 7 = 100% bright, cold+warm white
    0x7F, 0x7F,         // Preset 8 = 100% bright, cold+warm white
    0x33, 0x7F          // Preset 9 =  bright, cold+warm white
};

// ----------------------------------------------------------------------------
// lastly used settings, stored in EEPROM and read on start
uint8_t last_brightness;
uint8_t last_colortemp;
uint8_t current_preset = 1;

leds_state_t state = LEDS_PWR_OFF;
bool mark_for_save = false;
uint32_t save_timestamp;
uint32_t wakeup_timestamp;

void leds_set(uint8_t bright, uint8_t color, bool store);

// ----------------------------------------------------------------------------
/**
 * @brief Initialize leds related stuff, 
 *          populate eeprom with default values on 1st boot
 * 
 */
void leds_init(void)
{

    uint16_t marker = eeprom_read_word(&eep_fact_marker);
    Serial.println(marker, HEX);
    // factory reset on 1st boot
    if (marker != EEP_FACT_MARKER)
    {
        Serial.println(F("Factory reset"));
        eeprom_write_block(fact_preset_table, eep_preset_table, sizeof(eep_preset_table)); // write the factory data to eeprom
        eeprom_busy_wait();
        eeprom_write_byte(&eep_last_preset, 0x01);                                         // default preset = 1
        eeprom_busy_wait();
        current_preset = 1;
        eeprom_write_word(&eep_fact_marker, EEP_FACT_MARKER);
        eeprom_busy_wait();
    }
    current_preset = eeprom_read_byte(&eep_last_preset);
    last_brightness = eeprom_read_byte(&eep_preset_table[current_preset*2]);
    last_colortemp = eeprom_read_byte(&eep_preset_table[(current_preset*2)+1]);
    // congif the pins:
    pinMode(WARM_PIN, OUTPUT);
    pinMode(COLD_PIN, OUTPUT);
    state = LEDS_PWR_OFF;
    leds_set(0x00, 0X00, false);
}
// ----------------------------------------------------------------------------
/**
 * @brief Increase brightness in steps defined in leds.h
 * 
 */
void leds_brightness_inc(void)
{
    state = LEDS_PWR_ON;
    if (last_brightness <= 0xFF-LEDS_BRIGHTNESS_STEP)    last_brightness += LEDS_BRIGHTNESS_STEP;
    else                                                last_brightness = 0xFF;
    leds_set(last_brightness, last_colortemp, true);
}
// ----------------------------------------------------------------------------
/**
 * @brief Decrease brightness in steps defined in leds.h
 * 
 */
void leds_brightness_dec(void)
{
    state = LEDS_PWR_ON;
    if (last_brightness >= LEDS_BRIGHTNESS_STEP)        last_brightness -= LEDS_BRIGHTNESS_STEP;
    else                                                last_brightness = 0x00;
    leds_set(last_brightness, last_colortemp, true);
}
// ----------------------------------------------------------------------------
/**
 * @brief Change color temperature cold--->warm
 * 
 */
void leds_colortemp_warm(void)
{
    state = LEDS_PWR_ON;
    if (last_colortemp <= 0xFF-LEDS_COLORTEMP_STEP)      last_colortemp += LEDS_COLORTEMP_STEP;
    else                                                last_colortemp = 0xFF;
    leds_set(last_brightness, last_colortemp, true);
}
// ----------------------------------------------------------------------------
/**
 * @brief Change color temperature warm--->cold
 * 
 */
void leds_colortemp_cold(void)
{
    state = LEDS_PWR_ON;
    if (last_colortemp >= LEDS_COLORTEMP_STEP)          last_colortemp -= LEDS_BRIGHTNESS_STEP;
    else                                                last_colortemp = 0x00;
    leds_set(last_brightness, last_colortemp, true);
}

// ----------------------------------------------------------------------------
/**
 * @brief set new leds engine state
 * 
 * @param s 
 */
void leds_set_state(leds_state_t s)
{
    state = s;
    switch (state)
    {
        case LEDS_PWR_OFF:
            leds_set(0x00, last_colortemp, false);
            break;
        case LEDS_WAKE_UP:
            wakeup_timestamp = millis();
            leds_set_preset(current_preset);
            break;
        case LEDS_PWR_ON:
            leds_set(last_brightness, last_colortemp, false);
            break;        
        default:
            break;
    }       
}
// ----------------------------------------------------------------------------
leds_state_t leds_get_state(void)
{
    return state;
}
// ----------------------------------------------------------------------------
/**
 * @brief Set the brightness and color of the leds
 * 8 bit values
 * 
 * @param bright    overall brightness level, 8 bit value
 * @param color     mix between cold and warm white, 8 bit value
 */
void leds_set(uint8_t bright, uint8_t color, bool store)
{
    uint16_t t16;
    uint8_t cold, warm;

    if (bright == 0)
    {
        analogWrite(WARM_PIN, 0x00);
        analogWrite(COLD_PIN, 0x00);
        return;
    }

    t16 = ((uint16_t) color * color) >> 6;      // 0-1023

    if (t16 > 0xFF) warm = 0xFF;                 // clamp to 0xFF
    else            warm = t16;
    
    t16 = 511 - (t16>>1);
    if (t16 > 0xFF) cold = 0xFF;
    else            cold = t16;

    warm = ((uint16_t) warm * bright)>>8;       // scale by brightness
    cold = ((uint16_t) cold * bright)>>8; 

    analogWrite(WARM_PIN, warm);
    analogWrite(COLD_PIN, cold);
    if (store)
    {
        last_brightness = bright;
        last_colortemp = color;
    }

}
// ----------------------------------------------------------------------------
/**
 * @brief Both leds at 100% brightness
 * 
 */
void leds_set_max(void)
{
    state = LEDS_PWR_ON;
    leds_set(0xFF, 0x7F, true);
}

/**
 * @brief Sets the new preset and marks the "store last preset flag"
 *          last preset is saved in eeprom after 10s wait to reduce 
 *          number of eeprom write cycles
 * 
 * @param preset preset number, 0-9
 */
void leds_set_preset(uint8_t preset)
{
    if (preset > 9) return;
    current_preset = preset;
    last_brightness = eeprom_read_byte(&eep_preset_table[current_preset*2]);
    last_colortemp = eeprom_read_byte(&eep_preset_table[(current_preset*2)+1]);
    leds_set(last_brightness, last_colortemp, false);

    mark_for_save = true;
    save_timestamp = millis();
}
// ----------------------------------------------------------------------------
/**
 * @brief Invoked in the main loop
 * 
 */
void leds_process(void)
{
    uint32_t time_now = millis();
    if (time_now - wakeup_timestamp > 100 && state == LEDS_WAKE_UP)
    {
        state = LEDS_PWR_ON;
    }
    // ### Last preset save after 10s wait
    if (!mark_for_save) return;
    
    if (time_now - save_timestamp > AUTOSAVE_DELAY_MS)
    {
        mark_for_save = false;
        eeprom_update_byte(&eep_last_preset, current_preset);
    }
}
