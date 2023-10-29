/*
 * user_lcd.c
 *
 *  Created on: Oct 28, 2023
 *      Author: lytha
 */
#include "user_lcd.h"

void LCD_init(void)
{
   LCD(0x38, 0);         //2 lines, 5*7 matrix
   LCD(0x0C, 0);         //Display on, cursor off
   //LCD(0x0E, 0);         //Display on, cursor blinking
   LCD(0x06, 0);         //Increment cursor (shift cursor to right)
   LCD(0x01, 0);         //Clear display screen
   LCD(0x80, 0);         //Force cursor to beginning ( 1st line)
}

void LCD(uint8_t val_1, uint8_t cmd)
{
   uint8_t data1;

   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, cmd);         //set RS = cmd; (cmd=0) => Command; (cmd=1) => Data

   data1 = val_1 & 0x01;                                          //mask bit and assign it to GPIO 0
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, data1);

   data1 = (val_1>>1) & 0x01;                                 //mask bit and assign it to GPIO 1
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, data1);

   data1 = (val_1>>2) & 0x01;                                 //mask bit and assign it to GPIO 2
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, data1);

   data1 = (val_1>>3) & 0x01;                                 //mask bit and assign it to GPIO 3
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, data1);

   data1 = (val_1>>4) & 0x01;                                 //mask bit and assign it to GPIO 4
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, data1);

   data1 = (val_1>>5) & 0x01;                                 //mask bit and assign it to GPIO 5
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, data1);

   data1 = (val_1>>6) & 0x01;                                 //mask bit and assign it to GPIO 6
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, data1);

   data1 = (val_1>>7) & 0x01;                                 //mask bit and assign it to GPIO 7
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, data1);

   //---------------------------------------------//
   //      Enable Pulse      //
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
   HAL_Delay(5);
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}

void LCD_str(unsigned char *str1)
{
   while(*str1 != 0)
   {
      LCD(*str1, 1);
      str1++;
   }
}


