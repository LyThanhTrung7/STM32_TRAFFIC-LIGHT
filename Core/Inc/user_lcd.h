/*
 * user_lcd.h
 *
 *  Created on: Oct 28, 2023
 *      Author: lytha
 */

#ifndef INC_USER_LCD_H_
#define INC_USER_LCD_H_
#include "stm32f1xx_hal.h"
#define L0 0x80
#define L1 0xC0
#define L2 0x94
#define L3 0xD4

void LCD_init(void);
void LCD(uint8_t val_1, uint8_t cmd);
void LCD_str(unsigned char *str1);


#endif /* INC_USER_LCD_H_ */
