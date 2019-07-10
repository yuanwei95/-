/*!
    \file  uart.h
    \brief the header file of main 
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#ifndef __UART_H
#define __UART_H

#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define USART0_TDATA_ADDRESS      ((uint32_t)0x40013828)  
#define USART0_RDATA_ADDRESS      ((uint32_t)0x40013824)  

#define DMA_UART_RX DMA_CH2
#define DMA_UART_TX DMA_CH1

/* eval board low layer COM */
#define COMn                             1U
/* definition for COM1, connected to USART0 */
#define EVAL_COM1                        USART0
#define EVAL_COM1_CLK                    RCU_USART0
#define EVAL_COM1_TX_PIN                 GPIO_PIN_9
#define EVAL_COM1_RX_PIN                 GPIO_PIN_10
#define EVAL_COM_GPIO_PORT               GPIOA
#define EVAL_COM_GPIO_CLK                RCU_GPIOA
#define EVAL_COM_AF                      GPIO_AF_1


#define LengthOfUartRxBuffer1 100
#define LengthOfUartTxBuffer1 100

#define MessageStartFlag 0xA5
#define MessageStartFlagEx 0x5A
#define MessageCmdStopDebug 0x00

#define RxCmdSize1   0x3E
#define RxCmdNum1   0x20
#define CmdParaMaxNum1 8

#define RXCmd_EndFlag '\r'
#define RXCmd_EndFlag2 '\n'

typedef enum
{
  BaudRate_128000  = 0,
  BaudRate_115200 = 1, 
  BaudRate_153600 = 2,   
  BaudRate_230400 = 3,
  BaudRate_Tail,
}BaudRate_enum;

typedef struct
{
  u8 CmdValid;
  u8 CmdLength; 
  char CmdPara[RxCmdSize1];
}Cmd_Type;

typedef struct
{
  u8 CmdParaNumber; 
  long CmdValuePara[CmdParaMaxNum1+2];
}CmdPara_Type;

extern Cmd_Type RxCmd1[];
extern u8 RxCmdCounter1;
extern const u32 BaudRateList[];
extern BaudRate_enum SET_BaudRate;
extern BaudRate_enum Cur_BaudRate;
extern u32 Cur_BaudRate_Value;

void uart_initial(uint32_t baudval);

bool VsnprintfCheckReady(void);
void  VsnprintfForDma( const char *format, ... );
void  VsnprintfForDmaNoFlag( const char *format, ... );
void  VsnprintfHexForDma(u32 PtrData, u16 Length);

void UART_RxDMAHandler(void);

void RecCmdFormat(u8 temp);

void PrintUartRxBuffer(void);


#endif /* __UART_H */
