// *********************************************************************//
// File name: 		    motor.c                                         //
// File description: 	Esse codigo implementa o funcionamento do motor	//          										//
// Author name:			Gabriel Erick Matheus Imamura                   //
//						Guilherme Augusto Vieira Palma                  //
//                      Henrique Meneguetti Bianchi                     //
//                      João Vitor Gomes Do N. De Siqueira              //
//                      Paulo Roberto De Araújo Junior  				//
//						Suellen Ribeiro									//
// Creation date:		27ago2024                                       //
// Revision date:		29set2024                                       //
// *********************************************************************//

#include <motor.h>
#include <main.h>
#include "tim.h"

TIM_HandleTypeDef *htimRodaMotores;
// ********************************************************** //
// Method name:        vMotoresInit               			  //
// Method description: Inicializa o timer do PWM pros motores //
// Input params:       htimRodaMotores                 		  //
// Output params:      n/a                          		  //
// ********************************************************** //

void vMotorInit(TIM_HandleTypeDef *htim){
	htimRodaMotores = htim;
	HAL_TIM_PWM_Start(htimRodaMotores, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(htimRodaMotores, TIM_CHANNEL_2);
	HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port, Motor_Dir_IN1_Pin, 0);
	HAL_GPIO_WritePin(Motot_Dir_IN2_GPIO_Port, Motot_Dir_IN2_Pin, 1);
	HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port, Motor_Esq_IN3_Pin, 0);
	HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port, Motor_Esq_IN4_Pin, 1);
}

void vSetRodasDC(float fRodaEsquerdaDC, float fRodaDireitaDC)
{
  __HAL_TIM_SET_COMPARE(htimRodaMotores, TIM_CHANNEL_1, fRodaEsquerdaDC * 1000);
  __HAL_TIM_SET_COMPARE(htimRodaMotores, TIM_CHANNEL_2, fRodaDireitaDC * 1000);
}
