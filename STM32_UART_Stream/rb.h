#include <stdint.h>

typedef struct RB_t
{
    uint8_t *RB_Storage;
    uint8_t RB_Full_Flag;
    uint32_t RB_Read_Index;
    uint32_t RB_Write_Index;
    uint32_t RB_Size;
}RB_t;

void RB_Init(RB_t *handle, uint8_t *buffer, uint32_t size);
void RB_Flush(RB_t *handle);

uint8_t RB_Is_Full(RB_t *handle);
uint8_t RB_Is_Empty(RB_t *handle);

uint32_t RB_Get_Count(RB_t *handle);
uint32_t RB_Get_Free(RB_t *handle);

int RB_Get_Char(RB_t *handle);
void RB_Put_Char(RB_t *handle, uint8_t chr);

uint32_t RB_Put_Chars(RB_t *handle, char *buff, uint32_t cnt, uint32_t timeout);
uint32_t RB_Get_Chars(RB_t *handle, char *buff, uint32_t cnt, uint32_t timeout);