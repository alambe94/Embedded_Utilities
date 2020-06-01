/*
 * FreeRTOS+CLI V1.0.4
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef _CLI_H_
#define _CLI_H_

#include "stdint.h"

typedef struct
{
    const char *CLI_Command;
    const char *CLI_Command_Description;
    uint16_t CLI_Command_Length;
    uint8_t (*CLI_Callback)(uint8_t argc,
                            const char *argv[],
                            char *cli_out_buffer,
                            uint16_t cli_out_max);
} CLI_Command_t;

void CLI_Init();
uint8_t CLI_Add_Command(CLI_Command_t *command_def);
uint8_t CLI_Process_Command(const char *cli_in_buffer, char *cli_tx_out_buffer, uint16_t max_buffer_len);
void CLI_Parse_Arguments(const char *cli_in_buffer, uint8_t *argc, const char *argv[]);
uint8_t CLI_Get_Argument_Length(const char *arg);

#endif /* _CLI_H_ */
