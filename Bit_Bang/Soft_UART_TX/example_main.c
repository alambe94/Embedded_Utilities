#include "soft_uart.h"

Soft_Uart_TX_t UART1;
char UART1_TX_Buffer[32];
char UART1_RX_Buffer[32];

Soft_Uart_TX_t UART2;
char UART2_TX_Buffer[32];
char UART2_RX_Buffer[32];

Soft_Uart_TX_t UART3;
char UART3_TX_Buffer[32];
char UART3_RX_Buffer[32];

Soft_Uart_TX_t UART4;
char UART4_TX_Buffer[32];
char UART4_RX_Buffer[32];

Soft_Uart_TX_t UART5;
char UART5_TX_Buffer[32];
char UART5_RX_Buffer[32];

Soft_Uart_TX_t UART6;
char UART6_TX_Buffer[32];
char UART6_RX_Buffer[32];

Soft_Uart_TX_t UART7;
char UART7_TX_Buffer[32];
char UART7_RX_Buffer[32];

Soft_Uart_TX_t UART8;
char UART8_TX_Buffer[32];
char UART8_RX_Buffer[32];

Soft_Uart_TX_t UART9;
char UART9_TX_Buffer[32];
char UART9_RX_Buffer[32];

Soft_Uart_TX_t UART10;
char UART10_TX_Buffer[32];
char UART10_RX_Buffer[32];

void example_main()
{

  UART1.GPIO_TX_Pin           = UART_TX1_Pin;
  UART1.GPIO_TX_Port          = UART_TX1_GPIO_Port;
  UART1.TX_Ring_Buffer.Buffer = UART1_TX_Buffer;
  UART1.TX_Ring_Buffer.Size   = sizeof(UART1_TX_Buffer);


  UART2.GPIO_TX_Pin           = UART_TX2_Pin;
  UART2.GPIO_TX_Port          = UART_TX2_GPIO_Port;
  UART2.TX_Ring_Buffer.Buffer = UART2_TX_Buffer;
  UART2.TX_Ring_Buffer.Size   = sizeof(UART2_TX_Buffer);


  UART3.GPIO_TX_Pin           = UART_TX3_Pin;
  UART3.GPIO_TX_Port          = UART_TX3_GPIO_Port;
  UART3.TX_Ring_Buffer.Buffer = UART3_TX_Buffer;
  UART3.TX_Ring_Buffer.Size   = sizeof(UART3_TX_Buffer);


  UART4.GPIO_TX_Pin           = UART_TX4_Pin;
  UART4.GPIO_TX_Port          = UART_TX4_GPIO_Port;
  UART4.TX_Ring_Buffer.Buffer = UART4_TX_Buffer;
  UART4.TX_Ring_Buffer.Size   = sizeof(UART4_TX_Buffer);


  UART5.GPIO_TX_Pin           = UART_TX5_Pin;
  UART5.GPIO_TX_Port          = UART_TX5_GPIO_Port;
  UART5.TX_Ring_Buffer.Buffer = UART5_TX_Buffer;
  UART5.TX_Ring_Buffer.Size   = sizeof(UART5_TX_Buffer);


  UART6.GPIO_TX_Pin           = UART_TX6_Pin;
  UART6.GPIO_TX_Port          = UART_TX6_GPIO_Port;
  UART6.TX_Ring_Buffer.Buffer = UART6_TX_Buffer;
  UART6.TX_Ring_Buffer.Size   = sizeof(UART6_TX_Buffer);


  UART7.GPIO_TX_Pin           = UART_TX7_Pin;
  UART7.GPIO_TX_Port          = UART_TX7_GPIO_Port;
  UART7.TX_Ring_Buffer.Buffer = UART7_TX_Buffer;
  UART7.TX_Ring_Buffer.Size   = sizeof(UART7_TX_Buffer);


  UART8.GPIO_TX_Pin           = UART_TX8_Pin;
  UART8.GPIO_TX_Port          = UART_TX8_GPIO_Port;
  UART8.TX_Ring_Buffer.Buffer = UART8_TX_Buffer;
  UART8.TX_Ring_Buffer.Size   = sizeof(UART8_TX_Buffer);


  UART9.GPIO_TX_Pin           = UART_TX9_Pin;
  UART9.GPIO_TX_Port          = UART_TX9_GPIO_Port;
  UART9.TX_Ring_Buffer.Buffer = UART9_TX_Buffer;
  UART9.TX_Ring_Buffer.Size   = sizeof(UART9_TX_Buffer);


  UART10.GPIO_TX_Pin           = UART_TX10_Pin;
  UART10.GPIO_TX_Port          = UART_TX10_GPIO_Port;
  UART10.TX_Ring_Buffer.Buffer = UART10_TX_Buffer;
  UART10.TX_Ring_Buffer.Size   = sizeof(UART10_TX_Buffer);

  Soft_Uart_TX_Add(&UART1);
  Soft_Uart_TX_Add(&UART2);
  Soft_Uart_TX_Add(&UART3);
  Soft_Uart_TX_Add(&UART4);
  Soft_Uart_TX_Add(&UART5);
  Soft_Uart_TX_Add(&UART6);
  Soft_Uart_TX_Add(&UART7);
  Soft_Uart_TX_Add(&UART8);
  Soft_Uart_TX_Add(&UART9);
  Soft_Uart_TX_Add(&UART10);

    while(1)
    {
      Soft_Uart_Send_String(&UART1, "Hello World! from UART1\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART2, "Hello World! from UART2\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART3, "Hello World! from UART3\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART4, "Hello World! from UART4\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART5, "Hello World! from UART5\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART6, "Hello World! from UART6\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART7, "Hello World! from UART7\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART8, "Hello World! from UART8\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART9, "Hello World! from UART9\r\n");
      //HAL_Delay(1);
      Soft_Uart_Send_String(&UART10, "Hello World! from UART10\r\n");

    /* wait for transmission to complete*/
      while(!Soft_Uart_Get_TC_Flag(&UART10))
          {
          }

      Soft_Uart_Clear_TC_Flag(&UART1);
      Soft_Uart_Clear_TC_Flag(&UART2);
      Soft_Uart_Clear_TC_Flag(&UART3);
      Soft_Uart_Clear_TC_Flag(&UART4);
      Soft_Uart_Clear_TC_Flag(&UART5);
      Soft_Uart_Clear_TC_Flag(&UART6);
      Soft_Uart_Clear_TC_Flag(&UART7);
      Soft_Uart_Clear_TC_Flag(&UART8);
      Soft_Uart_Clear_TC_Flag(&UART9);
      Soft_Uart_Clear_TC_Flag(&UART10);
    }
}