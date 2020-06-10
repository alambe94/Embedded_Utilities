/*
 * file version V0.0.0
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
 */

#ifndef SOFT_OW_H_
#define SOFT_OW_H_

#include "stdint.h"

typedef struct Soft_OW_t
{
    /** enable clock, configure gpio pin as input */
    void (*Pin_Init)(void);

    /** return if pin is high or not */
    uint8_t (*Pin_Read)(void);

    /** set pin high */
    void (*Pin_High)();

    /** set pin low */
    void (*Pin_Low)();

} Soft_OW_t;

void Soft_OW_Init(Soft_OW_t *ow_handle);
uint8_t Soft_OW_Reset(Soft_OW_t *ow_handle);
void Soft_OW_Send_Byte(Soft_OW_t *ow_handle, uint8_t byte);
uint8_t Soft_OW_Receive_Byte(Soft_OW_t *ow_handle);

#endif /* SOFT_OW_H_ */
