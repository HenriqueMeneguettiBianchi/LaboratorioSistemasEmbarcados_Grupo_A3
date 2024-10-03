/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "motor.h"
#include "lcd.h"
#include <light_printf.h>
#include <stdio.h>
#include "sensorLinha.h"
#include "pid.h"
#include "encoder.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
float fLinhaPosicaoSensor;
extern float velocidadeRodaEsquerda;
extern float velocidadeRodaDireita;
float fRodaEsquerdaDC = 0.34;
float fRodaDireitaDC = 0.34;
float kp = 1.0;       // Proporcional
float ki = 0.1;       // Integral
float kd = 0.01;      // Derivativo
extern cAdressLCD;

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  MX_ADC3_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC4_Init();
  MX_ADC5_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();

  inicializarEncoders(&htim16, &htim17);
  vMotorInit(&htim1);
  vLcdInitLcd(&hi2c2, 0x27);
  vLcdWriteString("Teste de LCD!");

  SensorInit(&hadc1);
  SensorInit(&hadc2);
  SensorInit(&hadc3);
  SensorInit(&hadc4);
  SensorInit(&hadc5);
  PidInit(kp, ki, kd, 1);

  /* Infinite loop */
  while (1)
  {
	  fLinhaPosicaoSensor = fSensorLinhaLeituraSensor();

	    // Chama a função do PID e atualiza as velocidades das rodas
	  fPidUpdateData(fLinhaPosicaoSensor, &fRodaEsquerdaDC, &fRodaDireitaDC);

	  // Garantir que os valores de duty cycle estão dentro de limites
	  if (fRodaEsquerdaDC > 1.0) fRodaEsquerdaDC = 1.0;
	  if (fRodaEsquerdaDC < 0.0) fRodaEsquerdaDC = 0.0;
	  if (fRodaDireitaDC > 1.0) fRodaDireitaDC = 1.0;
	  if (fRodaDireitaDC < 0.0) fRodaDireitaDC = 0.0;

	  vSetRodasDC(fRodaEsquerdaDC, fRodaDireitaDC);

	  HAL_Delay(100);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  // Código de configuração do clock (sem alterações necessárias)
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif /* USE_FULL_ASSERT */
