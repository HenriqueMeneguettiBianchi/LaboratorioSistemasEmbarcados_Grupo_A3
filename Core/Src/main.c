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
#include "stdio.h"
#include "sensorLinha.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "lcd_hd44780_i2c.h"
#include "sensorUltrassonico.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
float kp1, ki1, kd1,d,e,kp2, ki2, kd2;
//float dutyCycleE= base_speed;
//float dutyCycleD= base_speed;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rxData;
volatile uint8_t rxData_M= 0;

int iColisao = 0;

int iCarroParado = 0;

int iLCDcounter = 0;

float fVelocidadeEsquerdaAntiga, fVelocidadeDireitaAntiga;

float fDistancia;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
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
  MX_TIM6_Init();
  MX_TIM15_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_TIM8_Init();
  MX_TIM20_Init();
  /* USER CODE BEGIN 2 */
  vMotorInit(&htim1);
  inicializarEncoders(&htim16, &htim17);
  vLineSensor1Init(&hadc1);
  vLineSensor2Init(&hadc2);
  vLineSensor3Init(&hadc3);
  vLineSensor4Init(&hadc4);
  vLineSensor5Init(&hadc5);
  lcdInit(&hi2c2,(uint8_t)0x27,(uint8_t)2,(uint8_t)16);
  HAL_TIM_Base_Start_IT(&htim15);
  HAL_TIM_Base_Start_IT(&htim3);
  vPidInit(3.2, 0.22, 0, 1000, 100);
  vPidInit2(3.5, 0.5, 0, 1000, 100);

  //Buzzer
  vDistanceSensorInitSensors(&htim20, TIM_CHANNEL_1, &htim3, TIM_CHANNEL_1);

  //Bluetooth
  HAL_UART_Receive_IT(&huart3,&rxData,1);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  	//vPrintMotorSpeed(0, 0);
    while (1)
    {
    	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
//    	iColisao = 0;


    	if((iCarroParado == 1) && (iColisao == 0)){
    		vPrintString("      MODO      ", "     MANUAL     ");
    	}


    	if ((5 <= iLCDcounter) && (rxData_M == 0) && (iColisao == 0))

	    {
			if ((fVelocidadeEsquerdaAntiga == velocidadeRodaEsquerda) && (fVelocidadeDireitaAntiga != velocidadeRodaDireita))
			  {
				vPrintMotorSpeed(0, velocidadeRodaDireita);
			  }
			  else if ((fVelocidadeEsquerdaAntiga != velocidadeRodaEsquerda) && (fVelocidadeDireitaAntiga == velocidadeRodaDireita))
			  {
				vPrintMotorSpeed(velocidadeRodaEsquerda, 0);
			  }
			  else if ((fVelocidadeEsquerdaAntiga != velocidadeRodaEsquerda) && (fVelocidadeDireitaAntiga != velocidadeRodaDireita))
			  {
				vPrintMotorSpeed(velocidadeRodaEsquerda, velocidadeRodaDireita);
			  }
			  else if ((fVelocidadeEsquerdaAntiga == velocidadeRodaEsquerda) && (fVelocidadeDireitaAntiga == velocidadeRodaDireita))
			  {
				vPrintMotorSpeed(0, 0);
			  }

			  fVelocidadeEsquerdaAntiga = velocidadeRodaEsquerda;
		      fVelocidadeDireitaAntiga = velocidadeRodaDireita;
		      iLCDcounter=0;
	    }



    	if(HAL_GPIO_ReadPin(Colisao_GPIO_Port, Colisao_Pin) == 1){

    		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 999);
    		vPrintString("    ATENCAO  !! ", "  HOUVE COLISAO ");
    		vSetRodasDC(0,0);
    		iColisao = 1;

    	}
    	else{
    		iColisao = 0;
    	}
    	fDistancia = fDistanceSensorGetRightDistance();
    	if (fDistancia < 20)
    	{
    		vPrintString("    ATENCAO  !! ", "  HAVERA COLISAO ");
    		vSetRodasDC(0,0);
			iColisao = 1;
    	}


//    	if( iColisao == 1){
//    	    vPrintString("    ATENCAO  !! ", "  HOUVE COLISAO ");
//    	    vSetRodasDC(0,0);
//    	    rxData_M = 1;
//    	}


    	//vSetRodasDC(d,e);
    	//vPrintMotorSpeed(velocidadeRodaEsquerda, velocidadeRodaDireita);
    	//HAL_Delay(10);
        // Controla o PID para ajustar os motores
        //vLineSensorPIDControl(velocidadeRodaEsquerda, velocidadeRodaDireita);
    	//vSetRodasDC(dutyCycleD,dutyCycleE);
    	 //vLineSensorPIDControl();
        // Adiciona um pequeno atraso se necessário
        //HAL_Delay(10); e esse o da apresentaçao 2

    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim){
	// Chamada a cada 10 ms
	iCarroParado = 0;
	if (rxData_M == 0 && iColisao ==0){
		if (htim == &htim15){
			vLineSensorPIDControl();
		}
		if (htim == &htim3){
			ajustaVelocidadeRodas();
			}
		if (htim == &htim16){
			overflow1 +=1;
			if(overflow1 > 5){
				velocidadeRodaEsquerda = 0;
			}
			}
		if (htim == &htim17){
			overflow2 +=1;
			if(overflow2 > 5){
				velocidadeRodaDireita = 0;
			}
		}
	}
	else if((iColisao == 1)){
		vSetRodasDC(0,0);
	}
	else if((fDistancia < 20) && rxData_M == 0){
			vSetRodasDC(0,0);
		}
	if (htim == &htim15){
		iLCDcounter += 1;
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim == pDistanceSensorTIM)
	{
		vDistanceSensorInputCaptureCallback(htim);
	}
    if (htim == &htim16) {
        processarCapturaRodaEsquerda(htim);
    }
    if (htim == &htim17) {
        processarCapturaRodaDireita(htim);

    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if(huart->Instance==USART3)
  {

	if(rxData=='M') // Modo Manual
	{
		rxData_M = 1;
		iCarroParado = 1;

	}
	else if (rxData=='A') // Modo Automático
	{
	    rxData_M = 0;
	}
	else if(rxData=='1') // Move para frente
	{
		vSetRodasDC(1,1);
		iCarroParado = 1;
		while(HAL_GPIO_ReadPin(Colisao_GPIO_Port, Colisao_Pin) == 1){
			vSetRodasDC(0,0);
			iColisao = 1;
		}
	}
	else if (rxData=='2') // Gira para direita
	{
		vSetRodasDC(1,0);
		iCarroParado = 1;
		while(HAL_GPIO_ReadPin(Colisao_GPIO_Port, Colisao_Pin) == 1){
			vSetRodasDC(0,0);
			iColisao = 1;
		}
	}
	else if (rxData=='3') // Gira para esquerda
	{
		vSetRodasDC(0,1);
		iCarroParado = 1;
		while(HAL_GPIO_ReadPin(Colisao_GPIO_Port, Colisao_Pin) == 1){
			vSetRodasDC(0,0);
			iColisao = 1;
		}
	}
	else if (rxData=='4') // Para o veículo
	{
		vSetRodasDC(0,0);
	}

    else if (rxData=='6')
    {

    }
    else if (rxData=='7')
    {

    }
    else if (rxData=='8') // Apaga todos os LEDs
    {
    	HAL_GPIO_WritePin(LED_G_PWM_GPIO_Port, LED_G_PWM_Pin, 0);
    	HAL_GPIO_WritePin(LED_R_PWM_GPIO_Port, LED_R_PWM_Pin, 0);
    	HAL_GPIO_WritePin(LED_B_PWM_GPIO_Port, LED_B_PWM_Pin, 0);
    }
    else if (rxData=='9') // Acendo todos os LEDs
    {
    	HAL_GPIO_WritePin(LED_G_PWM_GPIO_Port, LED_G_PWM_Pin, 1);
    	HAL_GPIO_WritePin(LED_R_PWM_GPIO_Port, LED_R_PWM_Pin, 1);
    	HAL_GPIO_WritePin(LED_B_PWM_GPIO_Port, LED_B_PWM_Pin, 1);
    }
    HAL_UART_Receive_IT(&huart3,&rxData,1);
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
