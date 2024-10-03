#ifndef SENSORLINHA_H
#define SENSORLINHA_H

#include "main.h"
#include "adc.h"

// Declaração dos manipuladores ADC
extern ADC_HandleTypeDef hadc1, hadc2, hadc3, hadc4, hadc5;

// Variáveis para os sensores
extern float fSensor1, fSensor2, fSensor3, fSensor4, fSensor5;
extern unsigned int uiSensor1, uiSensor2, uiSensor3, uiSensor4, uiSensor5;

// Função para inicializar o sensor
void SensorInit(ADC_HandleTypeDef *hadc);

// Função para ler os valores dos sensores de linha e calcular a posição
float fSensorLinhaLeituraSensor(void);

#endif // SENSORLINHA_H
