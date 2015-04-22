#ifndef __BAK_H_
#define __BAK_H_
#ifdef __cplusplus
       extern "C" {
#endif
#include "sys.h"
#include "stm32f10x_bkp.h"

void Data_Save(u8);
u16 Data_Read(void);
void StmFlash_Read(void);
void StmFlash_Save(void);
#ifdef __cplusplus
        }
#endif
#endif


