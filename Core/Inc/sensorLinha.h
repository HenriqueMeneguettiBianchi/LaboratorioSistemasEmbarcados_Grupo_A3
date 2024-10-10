// *****************
// File name: sensorLinha.h
// File description: Header file for line sensor control and PID
//
// Author name:
// Creation date:
// Revision date:

// *****************

#ifndef SENSORLINHA_H
#define SENSORLINHA_H

#include "main.h" // Para usar ADC_HandleTypeDef

// Funções de inicialização dos sensores
void vLineSensor1Init(ADC_HandleTypeDef *hadc1);
void vLineSensor2Init(ADC_HandleTypeDef *hadc2);
void vLineSensor3Init(ADC_HandleTypeDef *hadc3);
void vLineSensor4Init(ADC_HandleTypeDef *hadc4);
void vLineSensor5Init(ADC_HandleTypeDef *hadc5);
extern float fsaidaControladorLinha;

// Função para calcular a posição do carrinho com base nos sensores
float fLineSensorCalculatePosition(void);

// Função de controle PID
void vLineSensorPIDControl();

#endif // SENSORLINHA_H
