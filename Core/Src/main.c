/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "user_timer.h"
#include "user_lcd.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
int tt,tt1;
int second=0;
int k;
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
/////////////RTC/////////////
RTC_HandleTypeDef hrtc;
char lcd_data_1[100];
char lcd_data_2[100];
///////////////////////////
TIM_HandleTypeDef htim2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
////////////////LCD////////////////////
#define L0 0x80
#define L1 0xC0
#define L2 0x94
#define L3 0xD4
//////////////////////////////////////
unsigned char key_press(void);
unsigned char check_clm(unsigned char pin_var);
void LCD_init(void);
void LCD(uint8_t val_1, uint8_t cmd);
void LCD_str(unsigned char *str1);
void Disp_pass_key(unsigned char key_var);
/* USER CODE END PFP */
////////////////////LED7DOAN/////////////////////////////
#define	GPIO_DS_1 	    GPIOA
#define	PIN_DS_1 	    GPIO_PIN_12
#define	GPIO_SHCP_1 	GPIOA
#define	PIN_SHCP_1 	    GPIO_PIN_11
#define	GPIO_STCP_1 	GPIOA
#define	PIN_STCP_1 	    GPIO_PIN_15
#define __Get_Bit_1(__Value_, __bitN_) ((__Value_>>__bitN_) & 1)
/////////////////////LED7DOAN///////////////////////////////////
void Display_7SegLed_1(uint32_t Value_Display, uint8_t NumLed);
/* Private user code ---------------------------------------------------------*/
unsigned char var1;
int led_lr=0;
int button_lr;
int pre_lr=0;
////////////////////////////////////////////////////////
void LCD_START(){
    if(tt==6||tt1==6){
    	LCD(L1,0);
    	LCD_str("STATUS: WARNING");
    }else{
    	LCD(L1,0);
    	LCD_str("STATUS: AUTO");
    }
	   LCD(L2,0);
	   LCD_str("Enter password:");
}

///////////////////////CHECK KEYPAD///////////////
void check_key(){
	var1 = key_press();
	Disp_pass_key(var1);
	HAL_Delay(200);
}
///////////////////////RTC_REALTIME/////////////////////
void real_time(){
	RTC_TimeTypeDef sTime = {0};
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    RTC_DateTypeDef DateToUpdate = {0};
	HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
	if(k==1){
    sprintf(lcd_data_1,"TIME: %02d:%02d:%02d",sTime.Hours,sTime.Minutes,sTime.Seconds);
    LCD(L1,0);
    LCD_str(lcd_data_1);

    sprintf(lcd_data_2,"DATE: %02d/%02d/%04d",DateToUpdate.Date,DateToUpdate.Month,2000+DateToUpdate.Year);
	LCD(L0,0);
	LCD_str(lcd_data_2);
	}
    if(k==0){
    	if(sTime.Hours>=22||sTime.Hours<=4){
    		tt=6;
    		tt1=6;
    		second=-1;
        }
    }
}
void real_date(){
    RTC_DateTypeDef DateToUpdate = {0};
	HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
    sprintf(lcd_data_2,"DATE: %02d/%02d/%04d",DateToUpdate.Date,DateToUpdate.Month,2000+DateToUpdate.Year);
	LCD(L0,0);
	LCD_str(lcd_data_2);
}
/* USER CODE BEGIN 0 */
void auto_light_lr(){
	   /////////////button red.//////////////
	  if(k==1){
	     button_lr = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12); ///red
	    if(button_lr==1&&pre_lr==0){
	      if(led_lr==0){
	         tt=4;
	         tt1=5;
	         second=1;
	      }
	    }
        button_lr = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);///green
             if(button_lr==1&&pre_lr==0){
               if(led_lr==0){
                  tt=0;
                  tt1=0;
                  second=1;
               }
             }
        	  //////////////////////WARNING////////
        button_lr = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_14); ///red
        	 	    if(button_lr==1&&pre_lr==0){
        	 	      if(led_lr==0){
        	 	         tt=0;
        	 	         tt1=0;
        	 	         second=1;
        	 	      }
        	 	    }
           pre_lr=button_lr;
	  }

   switch(tt){
   case 0: //TT start
	tt=1;
   set_timer1(500);
   break;
   case 1: //red
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13,1);   //RED
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,0);   //YELLOW
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,0);  //GREEN

      //het time led red
      if(timer1_flag==1){
             tt=2;
          set_timer1(300);
      }
       ///button green
   break;
   case 2: //green
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13,0);   //RED
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,0);   //YELLOW
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,1);  //GREEN

      //het time led green
      if(timer1_flag==1){
            tt=3;
            set_timer1(200);
      }
     break;
   case 3: //yellow
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13,0);   //RED
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,1);   //YELLOW
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,0);  //GREEN

         //het time led yellow
         if(timer1_flag==1){
            tt=1;
            set_timer1(500);
         }
   break;
    ///////////control////////////
    case 4: //control red
       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13,1);   //RED
       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,0);   //YELLOW
       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,0);  //GREEN
   break;
    case 5: //control green
       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13,0);   //RED
       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,0);   //YELLOW
       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,1);  //GREEN
   break;
    case 6:
    	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);   //YELLOW
    break;
   default:
   break;
   }
}

void auto_light_ud(){

           /////////////button red.//////////////
   if(k==1){
          button_lr = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
           if(button_lr==1&&pre_lr==0){
             if(led_lr==0){
                tt=5;
                tt1=4;
                second=0;
             }
           }

   ///////////////////button green//////////////////
          button_lr = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
                 if(button_lr==1&&pre_lr==0){
                   if(led_lr==0){
                      tt1=0;
                      tt=0;

                   }
                 }
                 pre_lr=button_lr;
         }

   switch(tt1){
   case 0: //TT start
   tt1=2;
   set_timer2(300);
   break;
   case 1: //red
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1);     //RED
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,0);   //YELLOW
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,0);  //GREEN

      //het time led red
      if(timer2_flag==1){
             tt1=2;
            set_timer2(300);
            }
   break;
   case 2: //green
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,0);     //RED
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,0);   //YELLOW
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,1);  //GREEN

      //het time led green
      if(timer2_flag==1){
            tt1=3;
            set_timer2(200);
      }

     break;
   case 3: //yellow
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,0);   //RED
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,1);   //YELLOW
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,0);  //GREEN
         //het time led yellow
         if(timer2_flag==1){
            tt1=1;
            set_timer2(500);
         }
      break;
      ////////////control//////////
     case 4: //control red
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1);   //RED
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,0);   //YELLOW
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,0);  //GREEN
      break;
       case 5: //control GREEN
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,0);   //RED
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,0);   //YELLOW
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,1);  //GREEN
      break;
       case 6:
          HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);   //YELLOW
      break;
    default:
    break;
   }
}
void clock(){
   if(timer3_flag==1){
       second++;
       set_timer3(100);
      if(second>5){
         second=1;
      }
   }
   Display_7SegLed_1(second,1);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	   k=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);
  ////////////////TIMER////////////////////////
   MX_TIM2_Init();
   HAL_TIM_Base_Start_IT(&htim2); // Start timer ở CHE DO NGAT
   MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE LCD BEGIN///*/
   LCD_init();
   real_date();
   LCD_START();
   //////////////////////////////////////////
   set_timer1(3);
   set_timer2(2);
   set_timer3(1);
   Display_7SegLed_1(0,1);
   //////////////////////////////////////////
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
            clock();
	 	  	auto_light_lr();
	 	    auto_light_ud();
	        check_key();
	        real_time();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x12;
  sTime.Minutes = 0x00;
  sTime.Seconds = 0x00;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }


  DateToUpdate.WeekDay = RTC_WEEKDAY_FRIDAY;
  DateToUpdate.Month =0x10;
  DateToUpdate.Date = 0x29;
  DateToUpdate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A*/
  sAlarm.AlarmTime.Hours = 0x15;
  sAlarm.AlarmTime.Minutes = 0x23;
  sAlarm.AlarmTime.Seconds = 0x01;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 PA10 PA11
                           PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB10 PB11
                           PB3 PB4 PB5 PB6
                           PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    timer_run();
 }

////////////////////////LED7DOAN///////////////////////////////////////////

void Display_7SegLed_1(uint32_t Value_Display, uint8_t NumLed) {
    const unsigned char ma7doan_CA[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 };
   goto Label_Display_7SegLed;
   void ShiftOut(uint8_t Value_Shift) {
      for (int i = 7; i >= 0; i--) {
         HAL_GPIO_WritePin(GPIO_DS_1, PIN_DS_1, __Get_Bit_1(Value_Shift, i));
         __NOP();
         ///////////////////tao CLK////////////////////////////
         HAL_GPIO_WritePin(GPIO_SHCP_1, PIN_SHCP_1, GPIO_PIN_RESET);
         __NOP();
         HAL_GPIO_WritePin(GPIO_SHCP_1, PIN_SHCP_1, GPIO_PIN_SET);
         __NOP();
         HAL_GPIO_WritePin(GPIO_SHCP_1, PIN_SHCP_1, GPIO_PIN_RESET);
         __NOP();
      }
   }
   Label_Display_7SegLed: //start Display_7SegLed
   for (int i = 0; i < NumLed; i++) {
      ShiftOut(ma7doan_CA[Value_Display % 10]);
      Value_Display = Value_Display / 10;
   }
   ///////////////////////tao CLK//////////////////
   HAL_GPIO_WritePin(GPIO_STCP_1, PIN_STCP_1, GPIO_PIN_RESET);
   __NOP();
   HAL_GPIO_WritePin(GPIO_STCP_1, PIN_STCP_1, GPIO_PIN_SET);
   __NOP();
   HAL_GPIO_WritePin(GPIO_STCP_1, PIN_STCP_1, GPIO_PIN_RESET);
   __NOP();
}

/* USER CODE BEGIN 4 */
unsigned char key_press(void)
{
   unsigned char clm_var;
   unsigned char key_arr[] = {7, 8, 9, 15, 4, 5, 6, 14, 1, 2, 3, 13, 10, 0, 11, 12};

   if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0)
   {
      clm_var = check_clm(GPIO_PIN_0);
      return key_arr[clm_var-1];
   }
   else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
   {
      clm_var = check_clm(GPIO_PIN_1);
      return key_arr[clm_var+3];
   }
   else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 0)
   {
      clm_var = check_clm(GPIO_PIN_3);
      return key_arr[clm_var+7];
   }
   else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
   {
      clm_var = check_clm(GPIO_PIN_4);
      return key_arr[clm_var+11];
   }
   else
      return 16;
}

unsigned char check_clm(unsigned char pin_var)
{
   unsigned char clm;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_SET);      //Set all columns to logic 1 ie connected to Vcc

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);                  //set first column to Gnd
   if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
      clm = 1;

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);                  //set second column to Gnd
   if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
      clm = 2;

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                  //set third column to Gnd
   if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
      clm = 3;

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);                  //set fourth column to Gnd
   if(HAL_GPIO_ReadPin(GPIOB, pin_var) == 0)
      clm = 4;

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);      //Set all columns to logic 0 ie connected to GND
   return clm;
}

void Disp_pass_key(unsigned char key_var)
{
   static unsigned char pass_var1 = 0;
   static unsigned char key_arr[5];
   unsigned char enter_key[5] = {"12345"};
   unsigned char i;

   if(key_var != 16)
   {
      key_arr[pass_var1] = key_var + 48;
      LCD(key_var + 48, 1);
      pass_var1++;
      if(pass_var1 == 5)
      {
         LCD(L3, 0);
         for(i=0;i<5;i++)
         {
            if(key_arr[i] != enter_key[i])
            {
               LCD(0x01, 0);
               LCD(L0+5,0);
               LCD_str("Re-enter pw");
               pass_var1 = 0;
               break;
            }
         }
         if(i == 5)
         {
     	   LCD(0x01, 0);
           LCD(L0+5,0);
           LCD_str("Welcome!!!!");
     	   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
     	   k=1;
         }
         HAL_Delay(500);
         pass_var1 = 0;
         LCD(0x01, 0);
         real_time();
         real_date();
         LCD(L2, 0);
         LCD_str("STATUS: CONTROL");
         LCD(L3,0);
 		 LCD_str("Enter password:");
      }
   }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
