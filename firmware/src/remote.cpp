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
#include "remote.h"
#include <IRremote.h>

uint32_t time_last;

ir_cmd_t cmd_last = CMD_IDLE;

bool no_repeat_key(ir_cmd_t cmd);

// ----------------------------------------------------------------------------
void remote_init(void)
{
    IrReceiver.begin(IR_INPUT_PIN, false, USE_DEFAULT_FEEDBACK_LED_PIN);
}

// ----------------------------------------------------------------------------
ir_cmd_t remote_process(void)
{
    ir_cmd_t cmd = CMD_IDLE;

    if (IrReceiver.decode())
    {
        #ifdef  IR_DEBUG_OUT_ON
            IrReceiver.printIRResultShort(&Serial);
        
        if (IrReceiver.decodedIRData.protocol == UNKNOWN)
        {
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        #endif
        IrReceiver.resume(); // Enable receiving of the next value
        if (IrReceiver.decodedIRData.address == 0x07)
        {
            cmd = (ir_cmd_t)IrReceiver.decodedIRData.command;
            if (cmd == cmd_last && no_repeat_key(cmd) && IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) // ingore sunsequenc PWR presses
            {
                return CMD_IDLE;
            } 
            cmd_last = cmd;         
        }
    }
    return cmd;
}
// ----------------------------------------------------------------------------

bool no_repeat_key(ir_cmd_t cmd)
{
    return ((cmd == CMD_PWR_TGL) || (cmd == CMD_STORE_PRESET));
}
