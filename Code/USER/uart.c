/*!
    \file  main.c
    \brief USART DMA transmitter receiver
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "gd32f3x0.h"
#include <stdio.h>

const u32 BaudRateList[BaudRate_Tail] = {128000, 115200, 153600, 230400};

static rcu_periph_enum COM_CLK[COMn]  = {EVAL_COM1_CLK};
static u32 COM_TX_PIN[COMn]      = {EVAL_COM1_TX_PIN};
static u32 COM_RX_PIN[COMn]      = {EVAL_COM1_RX_PIN};

u8 rxbuffer[LengthOfUartRxBuffer1];
u8 txbuffer[LengthOfUartTxBuffer1];
u8 UartRxCounter1 = 0;
u8 MasterFlag[] = "[Master]: ";

Cmd_Type RxCmd1[RxCmdNum1];

u8 RxCounter1 = 0x00; 
u8 RxCmdCounter1 = 0x00;

u32 CmdError = 0;

BaudRate_enum SET_BaudRate;
BaudRate_enum Cur_BaudRate;
u32 Cur_BaudRate_Value=0;

/*******************************************************************************
* Function Name  : gd_eval_com_init
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void gd_eval_com_init(uint32_t com, uint32_t baudval)
{
    uint32_t COM_ID;
    
    if(EVAL_COM1 == com){
        COM_ID = 0U;
    }else{
    }

    /* enable COM GPIO clock */
    rcu_periph_clock_enable(EVAL_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);

    /* connect port to USARTx_Tx */
    gpio_af_set(EVAL_COM_GPIO_PORT, EVAL_COM_AF, COM_TX_PIN[COM_ID]);

    /* connect port to USARTx_Rx */
    gpio_af_set(EVAL_COM_GPIO_PORT, EVAL_COM_AF, COM_RX_PIN[COM_ID]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_TX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_TX_PIN[COM_ID]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_RX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, baudval);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);

    usart_enable(com);
}

/*******************************************************************************
* Function Name  : VsnprintfForDma
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void  VsnprintfForDmaPreDeal(void)
{
   memcpy(txbuffer, MasterFlag, sizeof(MasterFlag) -1);
}
/*******************************************************************************
* Function Name  : uart_initial
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void uart_initial(uint32_t baudval)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);

    gd_eval_com_init(EVAL_COM1, baudval);

    /* deinitialize DMA channel1 */
    dma_deinit(DMA_UART_TX);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = 0; //ARRAYNUM(txbuffer);
    dma_init_struct.periph_addr = USART0_TDATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_UART_TX,dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_UART_TX);
    dma_memory_to_memory_disable(DMA_UART_TX);
    /* enable DMA channel1 */
    dma_channel_enable(DMA_UART_TX);
    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(USART0, USART_DENT_ENABLE);

    /* deinitialize DMA channel2 */
    dma_deinit(DMA_UART_RX);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = LengthOfUartRxBuffer1;
    dma_init_struct.periph_addr = USART0_RDATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_UART_RX, dma_init_struct);
    
    dma_circulation_enable(DMA_UART_RX);
    dma_memory_to_memory_disable(DMA_UART_RX);

    dma_channel_enable(DMA_UART_RX);
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);

    VsnprintfForDmaPreDeal();

    //VsnprintfForDma("Welcome to RayTouch!\r\n");
    //VsnprintfForDma("USART_BaudRate is %d!\r\n", baudval);
}
/*******************************************************************************
* Function Name  : HexCmdToTEXTCmd
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void HexCmdToTEXTCmd(uint8_t temp)
{
  RxCmd1[RxCmdCounter1].CmdPara[RxCounter1%RxCmdSize1] = RayTouch_Cmd_Mode_Hex;
  RxCounter1++;  
  RxCmd1[RxCmdCounter1].CmdPara[RxCounter1%RxCmdSize1] = ' ';
  RxCounter1++;                                
  RxCmd1[RxCmdCounter1].CmdPara[RxCounter1%RxCmdSize1] = '0' + (temp/100)%10;
  RxCounter1++;
  RxCmd1[RxCmdCounter1].CmdPara[RxCounter1%RxCmdSize1] = '0' + (temp/10)%10;
  RxCounter1++;               
  RxCmd1[RxCmdCounter1].CmdPara[RxCounter1%RxCmdSize1] = '0' + (temp/1)%10;
  RxCounter1++;  
  
  //只支持一个字节命令，无负载
  RxCmd1[RxCmdCounter1].CmdLength = RxCounter1;
  RxCmd1[RxCmdCounter1].CmdValid = ENABLE;
  
  RxCmdCounter1++;
  RxCounter1 = 0;
  if(RxCmdCounter1 >= RxCmdNum1)
  {
      RxCmdCounter1 = 0;
  } 
}
/*******************************************************************************
* Function Name  : RecCmdFormat
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void RecCmdFormat(uint8_t temp)
{
    static int CmdTranslateBegin = TRUE;
    static int CmdTranslateBegin_Hex = FALSE;
    //VsnprintfForDma("%X\r", temp);
    
    if(Cmd_Mode == Cmd_Mode_Text)
    {
        if(temp != RXCmd_EndFlag2)  //丢弃'\n'
        {
            if(temp == RXCmd_EndFlag)
            {
                CmdTranslateBegin = TRUE;
            }

            if( (RxCmd1[RxCmdCounter1].CmdValid != ENABLE) && (CmdTranslateBegin == TRUE) )
            {
                //命令长度超出后丢弃
                if(RxCounter1 < RxCmdSize1) 
                {                
                    RxCmd1[RxCmdCounter1].CmdPara[RxCounter1%RxCmdSize1] = temp;
                    if(temp == RXCmd_EndFlag)
                    {
                        RxCmd1[RxCmdCounter1].CmdLength = RxCounter1+1;
                        RxCmd1[RxCmdCounter1].CmdValid = ENABLE;
                        RxCmdCounter1++;
                        RxCounter1 = 0;
                        if(RxCmdCounter1 >= RxCmdNum1)
                        {
                            RxCmdCounter1 = 0;
                        }        
                    }
                    else
                    {
                        RxCounter1++; 
                    }
                }
                else if(RxCounter1 >= RxCmdSize1)
                {
                    CmdTranslateBegin = FALSE;
                    RxCounter1 = 0;
                }
            }
            else
            {
                CmdTranslateBegin = FALSE;
                RxCounter1 = 0;
            }
        }
    }
    else if(Cmd_Mode == Cmd_Mode_Hex)
    {
        if(temp == MessageStartFlag )
        {
            CmdTranslateBegin_Hex = TRUE;
        }
        else if(( (RxCmd1[RxCmdCounter1].CmdValid != ENABLE) && (CmdTranslateBegin_Hex == TRUE) ))
        {
            HexCmdToTEXTCmd(temp); 
            CmdTranslateBegin_Hex = FALSE;
        }
        else
        {
            CmdError++;
        }
    }
}
/*******************************************************************************
* Function Name  : UART_RxDMAHandler
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void UART_RxDMAHandler(void)
{
    u16 Temp, PositonRx;
    PositonRx = LengthOfUartRxBuffer1 - dma_transfer_number_get(DMA_UART_RX);

    if( UartRxCounter1 < PositonRx )
    {
        for(Temp = UartRxCounter1; Temp < PositonRx; Temp++)
        {
            RecCmdFormat(rxbuffer[Temp]);
        }
        UartRxCounter1 = PositonRx;
    }
    else if(UartRxCounter1 > PositonRx)
    {
        for(Temp = UartRxCounter1; Temp < LengthOfUartRxBuffer1; Temp++)
        {
            RecCmdFormat(rxbuffer[Temp]);
        }
        for(Temp = 0; Temp < PositonRx; Temp++)
        {
            RecCmdFormat(rxbuffer[Temp]);
        }
        UartRxCounter1 = PositonRx;
    }
}

/*******************************************************************************
* Function Name  : VsnprintfCheckReady
* Description    : 
* Input          : 
* Output         : bool
* Return         : 
*******************************************************************************/
bool VsnprintfCheckReady(void)
{
    if(dma_transfer_number_get(DMA_UART_TX) != 0)
    {
        return FALSE;
    } 
    else
    {
        return TRUE;
    }
}

/*******************************************************************************
* Function Name  : VsnprintfForDma
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void  VsnprintfForDma( const char *format, ... )
{
    int Count_format;
    va_list args;

    while(dma_transfer_number_get(DMA_UART_TX) != 0);
    va_start(args, format);
    Count_format = vsnprintf((char*)&txbuffer[sizeof(MasterFlag) -1], LengthOfUartTxBuffer1-sizeof(MasterFlag),format, args);
    va_end(args);
    
    if(Count_format < 0) 
    {
        return;
    }  
    else
    {
        /* Disable USART2 TX DMA1 Channel */
        dma_channel_disable(DMA_UART_TX);

        dma_memory_address_config(DMA_UART_TX, (u32)&txbuffer);
        dma_transfer_number_config(DMA_UART_TX, Count_format + sizeof(MasterFlag) -1);
        
        /* Enable USART2 TX DMA1 Channel */
        dma_channel_enable(DMA_UART_TX);
    }    
}

/*******************************************************************************
* Function Name  : VsnprintfForDma
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void  VsnprintfForDmaNoFlag( const char *format, ... )
{
    int Count_format;
    va_list args;

    while(dma_transfer_number_get(DMA_UART_TX) != 0);
    va_start(args, format);
    Count_format = vsnprintf((char*)&txbuffer[sizeof(MasterFlag) -1], LengthOfUartTxBuffer1-sizeof(MasterFlag),format, args);
    va_end(args);
    
    if(Count_format < 0) 
    {
        return;
    }  
    else
    {
        /* Disable USART2 TX DMA1 Channel */
        dma_channel_disable(DMA_UART_TX);

        dma_memory_address_config(DMA_UART_TX, (u32)&txbuffer[sizeof(MasterFlag) -1]);
        dma_transfer_number_config(DMA_UART_TX, Count_format);
        
        /* Enable USART2 TX DMA1 Channel */
        dma_channel_enable(DMA_UART_TX);
    }    
}

/*******************************************************************************
* Function Name  : VsnprintfHexForDma
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void  VsnprintfHexForDma(u32 PtrData, u16 Length)
{
    #if 1
    while(dma_transfer_number_get(DMA_UART_TX) != 0);
    #else
    if(dma_transfer_number_get(DMA_UART_TX) != 0)
    {
         return;
    }
    #endif   
    /* Disable USART1 TX DMA1 Channel */
    dma_channel_disable(DMA_UART_TX);

    dma_memory_address_config(DMA_UART_TX, PtrData);
    dma_transfer_number_config(DMA_UART_TX, Length);

    /* Enable USART1 TX DMA1 Channel */
    dma_channel_enable(DMA_UART_TX);
}
/*******************************************************************************
* Function Name  : PrintUartRxBuffer
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void PrintUartRxBuffer(void)
{
    VsnprintfHexForDma((u32)&UartRxCounter1, 1);
    VsnprintfHexForDma((u32)&rxbuffer, LengthOfUartRxBuffer1);
}

