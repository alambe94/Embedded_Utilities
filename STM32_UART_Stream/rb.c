#include"rb.h"

/**
 * @brief Initialize ring buffer
 */
void RB_Init(RB_t *handle, uint8_t *buffer, uint16_t size)
{
    handle->RB_Storage = buffer;
    handle->RB_Size = size;
    handle->RB_Read_Index = 0;
    handle->RB_Write_Index = 0;
    handle->RB_Full_Flag = 0;
}

/**
 * @brief flush ring buffer
 */
void RB_Flush(RB_t *handle)
{
    handle->RB_Read_Index = handle->RB_Write_Index;
}

/**
 * @brief check if ring buffer is full
 * @retval return 1 if ring buffer is full
 */
uint8_t RB_Is_Full(RB_t *handle)
{
    return handle->RB_Full_Flag;
}

/**
 * @brief check if buffer is empty
 * @retval return 1 if ring buffer is empty
 */
uint8_t RB_Is_Empty(RB_t *handle)
{
    return (RB_Get_Count(handle) == 0);
}

/**
 * @brief check number of chars in ring buffer
 * @retval return number of chars in ring buffer
 */
uint16_t RB_Get_Count(RB_t *handle)
{
    if (handle->RB_Full_Flag)
        return handle->RB_Size;
    if (handle->RB_Write_Index >= handle->RB_Read_Index)
        return (handle->RB_Write_Index - handle->RB_Read_Index);
    return (handle->RB_Size - (handle->RB_Read_Index - handle->RB_Write_Index));
}

/**
 * @brief check number of free slots ring buffer
 * @retval return number of free slots in ring buffer
 */
uint16_t RB_Get_Free(RB_t *handle)
{
    handle->RB_Size-RB_Get_Count(handle);
}

/**
 * @brief return a char from from ring buffer
 * @retval char return
 */
int RB_Get_Char(RB_t *handle)
{
    if (RB_Is_Empty(handle))
    {
        /** exception */
        return -1;
    }

    uint8_t temp = handle->RB_Storage[handle->RB_Read_Index++];
    handle->RB_Full_Flag = 0;

    if (handle->RB_Read_Index == handle->RB_Size)
        handle->RB_Read_Index = 0;

    return temp;
}

/**
 * @brief write single char into ring buffer
 * @chr char to write
 */
void RB_Put_Char(RB_t *handle, uint8_t chr)
{
    if (RB_Is_Full(handle))
    {
        /** exception */
        return;
    }

    handle->RB_Storage[handle->RB_Write_Index++] = chr;

    if (handle->RB_Write_Index == handle->RB_Size)
        handle->RB_Write_Index = 0;

    if (handle->RB_Write_Index == handle->RB_Read_Index)
        handle->RB_Full_Flag = 1;
}

/**
 * @brief write number of chars in ring buffer
 *        if timeout occurs write only in available slots
 * @param buff buffer to write
 * @param cnt  number of char to write
 * @param timeout max wait time in milliseconds
 * @retval number of chars written
 */
uint16_t RB_Put_Chars(RB_t *handle, char *buff, uint16_t cnt, uint32_t timeout)
{
    uint32_t tick_now = HAL_GetTick();
    uint32_t tick_timeout = tick_now + timeout;
    uint16_t count = cnt;

    while (RB_Get_Free(handle) < cnt && (tick_now < tick_timeout))
    {
        tick_now = HAL_GetTick();
    }

    if (tick_now >= tick_timeout)
    {
        /** get available slots within timeout */
        count = RB_Get_Free(handle);
    }

    for (uint16_t i = 0; i < count; i++)
    {
        RB_Put_Char(handle, buff[i]);
    }

    return count;
}

/**
 * @brief return number of requested char from ring buffer
 *        if timeout occurs return only available chars
 * @param buff buffer where to retrieve
 * @param cnt  number of char to retrieve
 * @param timeout max wait time in milliseconds
 */
uint16_t RB_Get_Chars(RB_t *handle, char *buff, uint16_t cnt, uint32_t timeout)
{
    uint32_t tick_now = HAL_GetTick();
    uint32_t tick_timeout = tick_now + timeout;
    uint16_t count = cnt;

    while (RB_Get_Count(handle) < cnt && (tick_now < tick_timeout))
    {
        tick_now = HAL_GetTick();
    }

    if (tick_now >= tick_timeout)
    {
        /** get bytes available chars within timeout */
        count = RB_Get_Count(handle);
    }

    for (uint16_t i = 0; i < count; i++)
    {
        buff[i] = RB_Get_Char(handle);
    }

    return count;
}