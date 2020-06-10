#include "soft_i2c_master.h"
#include "stddef.h"

static void Soft_I2C_Master_State_Machine(Soft_I2C_Master_t *i2c_handle);

#if (I2C_USE_INTERRUPT_MODE)
static Soft_I2C_Master_t *Soft_I2C_Master_List[MAX_SOFT_I2C_MASTER];
static uint8_t Soft_I2C_Master_Count = 0;
extern void Soft_I2C_Master_TIM_Init();
extern void Soft_I2C_Master_TIM_Start();
extern void Soft_I2C_Master_TIM_Stop();

/* add a new instance of soft i2c master to list */
void Soft_I2C_Master_Add(Soft_I2C_Master_t *i2c_handle)
{
    if (Soft_I2C_Master_Count < MAX_SOFT_I2C_MASTER)
    {
        i2c_handle->Active_Flag = 0;
        i2c_handle->State = Generate_Start;
        i2c_handle->Soft_I2C_GPIO_Init();
        Soft_I2C_Master_List[Soft_I2C_Master_Count] = i2c_handle;
        Soft_I2C_Master_Count++;
    }
}

/** call in timer ISR */
void Soft_I2C_Master_TIM_ISR()
{
    Soft_I2C_Master_t *i2c_handle = NULL;

    uint8_t i2c_active_count = 0;

    for (uint8_t i = 0; i < Soft_I2C_Master_Count; i++)
    {
        /* grab i2c_handle from list*/
        i2c_handle = Soft_I2C_Master_List[i];

        if (i2c_handle->Active_Flag)
        {
            i2c_active_count++;
            Soft_I2C_Master_State_Machine(i2c_handle);
        }
    }
    /** if all soft_i2c are idle, disable tim interrupt */
    if (!i2c_active_count)
    {
        Soft_I2C_Master_TIM_Stop();
    }
}
#else
/** adjust i2c clock frequency */
extern void Delay_Micros();
#define Soft_I2C_Delay() Delay_Micros(5)
void Soft_I2C_Master_Init(Soft_I2C_Master_t *i2c_handle)
{
    i2c_handle->Soft_I2C_GPIO_Init();
}
#endif

static uint8_t Soft_I2C_ACK_Read(Soft_I2C_Master_t *i2c_handle)
{
    return i2c_handle->Soft_I2C_SDA_Read() ? 0 : 1;
}

static void Soft_I2C_Master_State_Machine(Soft_I2C_Master_t *i2c_handle)
{
    if (i2c_handle->CLK_State == 0)
    {
        i2c_handle->CLK_State  = 1;

        i2c_handle->Soft_I2C_SCL_High();

        /** things to do when clock is high */
        if (i2c_handle->State == Generate_Start)
        {
            /*generate start condition*/
            i2c_handle->Soft_I2C_SDA_Low();

            i2c_handle->Current_Byte = i2c_handle->Address_RW;

            i2c_handle->State = Data_Out;

            i2c_handle->Bit_Count = 0;
        }
        else if (i2c_handle->State == Data_Out)
        {
            if (i2c_handle->Bit_Count == 8)
            {
                /* Current_Byte sent*/
                i2c_handle->Bit_Count = 0;
                i2c_handle->Current_Byte = 0;
                i2c_handle->State = Read_ACK;
            }
        }
        else if (i2c_handle->State == Read_ACK)
        {
            /*check for ACK*/
            if (Soft_I2C_ACK_Read(i2c_handle))
            {
                /*ACK Received*/
                if ((i2c_handle->Address_RW) & 0x01)
                {
                    /*read from slave*/
                    i2c_handle->State = Data_In;
                }
                else
                {
                    /*write to slave*/
                    i2c_handle->State = Data_Out;

                    /*grab new Current_Byte if available*/
                    if (Ring_Buffer_Get_Count(i2c_handle->I2C_TX_Ring_Buffer))
                    {
                        Ring_Buffer_Get_Char(i2c_handle->I2C_TX_Ring_Buffer, &i2c_handle->Current_Byte);
                    }
                    else
                    {
                        /*all bytes transferred generate stop*/
                        i2c_handle->State = Generate_Stop;
                    }
                }
            }
            else
            {
                /*NACK Received*/
                i2c_handle->State = Generate_Stop;
                i2c_handle->Status_Flag = I2C_Error;
            }
        }
        else if (i2c_handle->State == Data_In)
        {
            i2c_handle->Current_Byte <<= 1;

            if (i2c_handle->Soft_I2C_SDA_Read())
            {
                i2c_handle->Current_Byte |= 0x01;
            }

            i2c_handle->Bit_Count++;

            if (i2c_handle->Bit_Count == 8)
            {
                i2c_handle->Bit_Count = 0;

                Ring_Buffer_Put_Char(i2c_handle->I2C_RX_Ring_Buffer, i2c_handle->Current_Byte);

                i2c_handle->Current_Byte = 0;

                i2c_handle->Bytes_To_Receive--;

                if (i2c_handle->Bytes_To_Receive == 0)
                {
                    /*all bytes received, generate stop*/
                    /*or give NACK then stop? */
                    i2c_handle->State = Generate_Stop;
                }
                else
                {
                    /*give ack to receive next byte*/
                    i2c_handle->State = Give_ACK;
                }
            }
        }
        else if (i2c_handle->State == Give_ACK)
        {
            /*next stage*/
            i2c_handle->State = Data_In;
        }
        else if (i2c_handle->State == Generate_Stop)
        {
            /*generate stop contion*/
            i2c_handle->Soft_I2C_SDA_High();

            i2c_handle->Active_Flag = 0;
            i2c_handle->State = Generate_Start;

            /** so next CLK_State wil be high */
            i2c_handle->CLK_State = 0;

            if (i2c_handle->Status_Flag != I2C_Error)
            {
                i2c_handle->Status_Flag = I2C_Ok;
            }

            /** if callback is defined call it */
            if (i2c_handle->callback != NULL)
            {
                i2c_handle->callback(i2c_handle->Status_Flag);
            }
        }
    }

    else if (i2c_handle->CLK_State == 1) /*i2c clock frequency is half of interrupt frequency but symmetric clock waveform*/
    //if (i2c_handle->CLK_State == 1) /*i2c clock frequency is same as interrupt frequency but asymmetric clock waveform*/
    {
        i2c_handle->CLK_State = 0;

        i2c_handle->Soft_I2C_SCL_Low();

        /*things to do when clock is low*/
        if (i2c_handle->State == Generate_Start)
        {
            /** nothing to do here */
            (void)0;
        }
        else if (i2c_handle->State == Data_Out)
        {
            /*send Current_Byte*/
            if (i2c_handle->Current_Byte & 0x80)
            {
                i2c_handle->Soft_I2C_SDA_High();
            }
            else
            {
                i2c_handle->Soft_I2C_SDA_Low();
            }

            i2c_handle->Current_Byte <<= 1;

            i2c_handle->Bit_Count++;
        }
        else if (i2c_handle->State == Read_ACK || i2c_handle->State == Data_In)
        {
            /* release sda line to read*/
            i2c_handle->Soft_I2C_SDA_High();
        }
        else if (i2c_handle->State == Give_ACK || i2c_handle->State == Generate_Stop)
        {
            /* set sda line to low*/
            i2c_handle->Soft_I2C_SDA_Low();
        }
    }
}

/* available bytes to read in rx buffer*/
uint16_t Soft_I2C_Master_Available(Soft_I2C_Master_t *i2c_handle)
{
    return Ring_Buffer_Get_Count(i2c_handle->I2C_RX_Ring_Buffer);
}

/* read byte from rx buffer*/
uint8_t Soft_I2C_Master_Read_Byte(Soft_I2C_Master_t *i2c_handle)
{
    uint8_t temp;
    Ring_Buffer_Get_Char(i2c_handle->I2C_RX_Ring_Buffer, &temp);
    return temp;
}

Soft_I2C_Master_Flags_t Soft_I2C_Master_Get_Status(Soft_I2C_Master_t *i2c_handle)
{
    return i2c_handle->Status_Flag;
}

void Soft_I2C_Master_Transmit(Soft_I2C_Master_t *i2c_handle,
                              uint8_t slave_address,
                              uint16_t reg_addr,
                              uint8_t reg_addr_size,
                              uint8_t *bytes,
                              uint16_t len)
{

    i2c_handle->Address_RW = slave_address;

    if (reg_addr_size == 2)
    {
        /* msb of register address if address is of two bytes*/
        Ring_Buffer_Put_Char(i2c_handle->I2C_TX_Ring_Buffer, (reg_addr >> 8));
    }

    Ring_Buffer_Put_Char(i2c_handle->I2C_TX_Ring_Buffer, reg_addr);

    /* copy data to be sent in ring buffer*/
    while (len--)
    {
        Ring_Buffer_Put_Char(i2c_handle->I2C_TX_Ring_Buffer, *bytes++);
    }

    i2c_handle->Bit_Count = 0;

    i2c_handle->Current_Byte = 0;

    i2c_handle->State = Generate_Start;

    i2c_handle->Status_Flag = I2C_Busy;

    i2c_handle->Active_Flag = 1;

#if (I2C_USE_INTERRUPT_MODE)
    /*enable TIM to start transaction */
    Soft_I2C_Master_TIM_Start();
#else
    while (i2c_handle->Active_Flag)
    {
        Soft_I2C_Master_State_Machine(i2c_handle);
        Soft_I2C_Delay();
    }
#endif
}

void Soft_I2C_Master_Receive(Soft_I2C_Master_t *i2c_handle,
                             uint8_t slave_address,
                             uint16_t reg_addr,
                             uint8_t reg_addr_size,
                             uint16_t len)
{

    /* send and wait for register address to read from*/
    Soft_I2C_Master_Transmit(i2c_handle,
                             slave_address,
                             reg_addr,
                             reg_addr_size,
                             NULL,
                             0);

    while (i2c_handle->Active_Flag)
    {
    }

    i2c_handle->Address_RW = slave_address + 1;

    i2c_handle->Bytes_To_Receive = len;

    i2c_handle->Bit_Count = 0;

    i2c_handle->Current_Byte = 0;

    i2c_handle->State = Generate_Start;

    i2c_handle->Status_Flag = I2C_Busy;

    i2c_handle->Active_Flag = 1;

#if (I2C_USE_INTERRUPT_MODE)
    /*enable TIM to start transaction */
    Soft_I2C_Master_TIM_Start();
#else
    while (i2c_handle->Active_Flag)
    {
        Soft_I2C_Master_State_Machine(i2c_handle);
        Soft_I2C_Delay();
    }
#endif
}

/*check if slave responding*/
void Soft_I2C_Master_Scan(Soft_I2C_Master_t *i2c_handle, uint8_t slave_address)
{
    i2c_handle->Address_RW = slave_address;

    i2c_handle->Bit_Count = 0;

    i2c_handle->Current_Byte = 0;

    i2c_handle->State = Generate_Start;

    i2c_handle->Status_Flag = I2C_Busy;

    i2c_handle->Active_Flag = 1;

#if (I2C_USE_INTERRUPT_MODE)
    /*enable TIM to start transaction */
    Soft_I2C_Master_TIM_Start();
#else
    while (i2c_handle->Active_Flag)
    {
        Soft_I2C_Master_State_Machine(i2c_handle);
        Soft_I2C_Delay();
    }
#endif
}
