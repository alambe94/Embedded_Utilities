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
