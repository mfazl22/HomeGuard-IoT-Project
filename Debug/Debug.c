#include "Debug.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx.h"
/*
#include<stdio.h>
#include<string.h>
*/

char receivedDebug[200];
unsigned char receivedDebugLen;

/*
 * Configure and enable USART1
 */
void debugInit(void)
{

	//configure_debug_gpio

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	/* GPIO configuration for USART1 signals */
	/* (1) Select AF mode (10) on PA9 and PA10 */
	/* (2) AF1 for USART1 signals */
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER9|GPIO_MODER_MODER10))\
					 | (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1); /* (1) */
	GPIOA->AFR[1] = (GPIOA->AFR[1] &~ (GPIO_AFRH_AFRH1 | GPIO_AFRH_AFRH2))\
				  | (1 << (1 * 4)) | (1 << (2 * 4)); /* (2) */

	/*
        	USART 1
            TX - PA9
           	RX - PA10
    */

    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;   //115200;//Add baud rate
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_Init(USART1, &USART_InitStruct);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/*
 * Send data with USART
 */
void debugSend(char StringToSend[])
{
    uint16_t Length = strlen(StringToSend);
    uint16_t i;
    for (i=0; i<Length; i++ )
    {
        USART_SendData(USART1, StringToSend[i]);
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
    }
}

void debugSend2(char StringToSend[], int len)
{
    uint16_t i;
    for (i=0; i<len; i++ )
    {
        USART_SendData(USART1, StringToSend[i]);
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
    }
}

void debugFlushRx(void)
{
    uint16_t i;
    for(i=0; i<50; i++) receivedDebug[i]=0;//flush buffer
    receivedDebugLen=0;
}

void debugReceive()
{

}

void USART1_IRQHandler (void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET)
    {
        receivedDebug[receivedDebugLen++] = USART_ReceiveData(USART1);
    }

}

/**
 * \brief debug printf to serial of a 16 bit unsigned int
 *
 * \param text to send before the value
 * \param the 16 bit unsigned value
 *
 */
void _printfU(char* text ,uint16_t x)
{
    #if DEBUG
    char debugString[100]="";
    sprintf(debugString, "%s %u\n",text, x);
    debugSend(debugString);
    #endif
}

/** \brief debug printf to serial of a 16 bit signed int
 *
 * \param text to send before the value
 * \param the 16 bit signed value
 *
 */
void _printfS(char* text ,int16_t x)
{
    #if DEBUG
    char debugString[100]="";
    sprintf(debugString, "%s %d\n",text, x);
    debugSend(debugString);
    #endif
}


/** \brief debug printf to serial of a 32 bit unsigned int
 *
 * \param text to send before the value
 * \param the 16 bit unsigned value
 *
 */
void _printfLngU(char* text ,uint32_t x)
{
    #if DEBUG
    char debugString[100]="";
    sprintf(debugString, "%s %u\n",text, x);
    debugSend(debugString);
    #endif
}

/** \brief debug printf to serial of a 32 bit signed int
 *
 * \param text to send before the value
 * \param the 16 bit signed value
 *
 */
void _printfLngS(char* text ,int32_t x)
{
    #if DEBUG
    char debugString[100]="";
    sprintf(debugString, "%s %d\n",text, x);
    debugSend(debugString);
    #endif
}
