#ifndef __LCD_H__
#define __LCD_H__
#include <stdint.h>
extern void LCDInit(void);
void LCDSendString(char *str,uint8_t row, uint8_t col);

#endif