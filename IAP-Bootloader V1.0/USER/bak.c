#include "./bak.h"
void Data_Save(u8 data)
{
    RCC->APB1ENR |= 1 << 28;//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC->APB1ENR |= 1 << 27;//RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    PWR->CR |= 1 << 8;      //DISENABLE写保护
    BKP_WriteBackupRegister(BKP_DR1, data);
}
u16 Data_Read(void)
{
    return BKP_ReadBackupRegister(BKP_DR1);
};
