#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h"
#include "bak.h"


//ALIENTEKս��STM32������ʵ��48
//IAPʵ�� Bootloader V1.0 ����
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
#define BLOCK (2048)

int main(void)
{
    u16 oldcount = 0; //�ϵĴ��ڽ�������ֵ
    u16 applenth = 0; //���յ���app���볤��

    uart_init(115200);  //���ڳ�ʼ��Ϊ115200
    delay_init();       //��ʱ��ʼ��
    printf("bootloader\r\n");
    if (USART_RX_CNT > 1)
    {
        Data_Save(1);
    }
    while (1)
    {
        int r = Data_Read();
        while (r == 1)
        {
            static int erase = 1;
            if (erase)
            {
                STMFLASH_Erase(FLASH_APP1_ADDR);
                delay_ms(30);
                erase = 0;
            }
            static int part1ok = 0;
            if (USART_RX_CNT)
            {
                extern volatile u8 *pUSART_RX_BUF;
                extern volatile u8 *plastUSART_RX_BUF;
                if (oldcount == USART_RX_CNT) //��������,û���յ��κ�����,��Ϊ�������ݽ������.
                {
                    applenth = USART_RX_CNT;
                    oldcount = 0;
                    USART_RX_CNT = 0;
                    printf("�û�����������!\r\n");
                    printf("���볤��:%dBytes\r\n", applenth);
                    if (applenth)
                    {
                        if (applenth <= BLOCK)
                        {
                            if (((*(vu32 *)(pUSART_RX_BUF + 4)) & 0xFF000000) == 0x08000000) //�ж��Ƿ�Ϊ0X08XXXXXX.
                            {
                                iap_write_appbin(FLASH_APP1_ADDR, pUSART_RX_BUF, applenth); //����FLASH����
                                delay_ms(100);
                                printf("Copy APP Successed1!!\r\n");
                                Data_Save(0);
                                r = 0;
                            }
                            else
                            {
                                printf("Illegal Receive APP1!\r\n");
                            }
                        }
                        else if (applenth > BLOCK && part1ok)
                        {
                            part1ok = 0;
                            iap_write_appbin(FLASH_APP1_ADDR + (applenth / BLOCK)*BLOCK, pUSART_RX_BUF, applenth % BLOCK); //����FLASH����
                            delay_ms(100);
                            printf("Copy APP Successed2!!%x %x\r\n", *(vs16 *)pUSART_RX_BUF, FLASH_APP1_ADDR + (applenth / BLOCK)*BLOCK);
                            Data_Save(0);
                            r = 0;
                        }
                    }
                    else
                    {
                        printf("No Reserive APP!\r\n");
                    }
                }
                else
                {
                    oldcount = USART_RX_CNT;
                    static int oldblock = 0;
                    if (oldcount / BLOCK > oldblock)
                    {
                        oldblock++;
                        if (((*(vu32 *)(plastUSART_RX_BUF + 4)) & 0xFF000000) == 0x08000000) //�ж��Ƿ�Ϊ0X08XXXXXX.
                        {
                            printf("Copying APP2FLASH...\r\n");
                            iap_write_appbin(FLASH_APP1_ADDR, plastUSART_RX_BUF, BLOCK); //����FLASH����
                            printf("Copy APP PART 1 Successed!!%x %x\r\n", *(vs16 *)plastUSART_RX_BUF, FLASH_APP1_ADDR);
                            part1ok = 1;
                        }
                        else if ((part1ok) && (oldcount / BLOCK > 1))
                        {
                            iap_write_appbin(FLASH_APP1_ADDR + (oldcount / BLOCK - 1) * BLOCK, plastUSART_RX_BUF, BLOCK); //����FLASH����
                            printf("Copy APP PART %d Successed!!%x %x\r\n", oldcount / BLOCK, *(vs16 *)plastUSART_RX_BUF, FLASH_APP1_ADDR + (oldcount / BLOCK - 1) * BLOCK);
                        }
                        else
                        {
                            printf("Illegal Receive APP2! id:%x %x\r\n", part1ok, oldcount / BLOCK);
                            part1ok = 0;
                        }
                    }
                }
            }
            delay_ms(10);
        }
        if (((*(vu32 *)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000) //�ж��Ƿ�Ϊ0X08XXXXXX.
        {
            printf("Run FLASH APP!!\r\n");
            iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
        }
        else
        {
            printf("Illegal FLASH APP!\r\n");
            Data_Save(1);
        }
    }
}
