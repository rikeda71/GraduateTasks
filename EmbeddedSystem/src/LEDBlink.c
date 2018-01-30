/*
===============================================================================
 Name        : LEDBlink.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC8xx.h"
#endif

#include <cr_section_macros.h>
#include "type.h"
void SwitchMatrix_Init();
int main(void) {
  SwitchMatrix_Init();
  // 信号機初期化
  LPC_GPIO_PORT->DIR0 |= (1<<2);
  LPC_GPIO_PORT->DIR0 |= (1<<4);
  // Force the counter to be placed into memory
  volatile static int i = 0 ;
  volatile static int j = 0 ;
  // Enter an infinite loop, just incrementing a counter
  while(1) {
    // 青点灯
    LPC_GPIO_PORT->NOT0 = 1<<2;
    for (i=0; i<500000; i++);
    // 青点滅
    for (i=0; i<11; i++) {
    	for (j=0; j<100000; j++);
    	LPC_GPIO_PORT->NOT0 = 1<<2;
    }
    // 赤点灯
    LPC_GPIO_PORT->NOT0 = 1<<4;
    for (i=0; i<700000; i++);
    // 赤消灯
    LPC_GPIO_PORT->NOT0 = 1<<4;
  }
  return 0;
}

