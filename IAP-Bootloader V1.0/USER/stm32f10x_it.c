/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "usart.h" 

u8 USART_RX_BUF2[4096];
volatile u8 *plastUSART_RX_BUF;
volatile u8 *pUSART_RX_BUF;
volatile u8 p = 1;
//����״̬
//bit15��  ������ɱ�־
//bit14��  ���յ�0x0d
//bit13~0��    ���յ�����Ч�ֽ���Ŀ
u32 USART_RX_STA = 0;       //����״̬���
volatile u32 USART_RX_CNT = 0;           //���յ��ֽ���
void USART1_IRQHandler(void)
{
    u8 res;
#ifdef OS_CRITICAL_METHOD   //���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
    OSIntEnter();
#endif
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
    {
        res = USART_ReceiveData(USART1);
        {					
					static int oldblock=0;
					if (USART_RX_CNT / 2048 >= oldblock)
          {
								oldblock++;
                if (p)
                {
                    p = 0;
                    plastUSART_RX_BUF = &USART_RX_BUF2[2048];
										pUSART_RX_BUF = &USART_RX_BUF2[0];
                }
                else
                {
                    p = 1;
                    plastUSART_RX_BUF = &USART_RX_BUF2[0];
									  pUSART_RX_BUF = &USART_RX_BUF2[2048];
									
                }
            }
            USART_RX_BUF2[USART_RX_CNT % 2048 + 2048 * p] = res;
            USART_RX_CNT++;
        }
    }
#ifdef OS_CRITICAL_METHOD   //���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
    OSIntExit();
#endif
}

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
