// *****************
// File name: sensorLinha.c
// File description: Controle do sensor de linha e PID
//
// Author name:
// Creation date:
// Revision date:
//
// *****************

#include "sensorLinha.h"
#include "motor.h"
#include "main.h" // Inclui os headers necessários do HAL

ADC_HandleTypeDef *sensor1ADC;
ADC_HandleTypeDef *sensor2ADC;
ADC_HandleTypeDef *sensor3ADC;
ADC_HandleTypeDef *sensor4ADC;
ADC_HandleTypeDef *sensor5ADC;

uint32_t lineSensor1Value;
uint32_t lineSensor2Value;
uint32_t lineSensor3Value;
uint32_t lineSensor4Value;
uint32_t lineSensor5Value;

// Variáveis do controlador PID
float Kp = 1.2f;
float Ki = 0.1f;
float Kd = 0.1f;

float error = 0.0f;
float previous_error = 0.0f;
float integral_error = 0.0f;
float derivative_error = 0.0f;

float control_output = 0.0f;

// Variáveis de tempo
float delta_time = 0.01f; // Assume um loop de 10ms
uint32_t previous_time = 0;

// Velocidade base dos motores
float base_speed = 0.15f; // Valor entre 0.0f e 1.0f
float max_duty_cycle = 0.15f; // Limite máximo do duty cycle

// Função para inicializar o sensor 1
void vLineSensor1Init(ADC_HandleTypeDef *hadc1) {
    sensor1ADC = hadc1;
    HAL_ADCEx_Calibration_Start(sensor1ADC, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(sensor1ADC, (uint32_t *)&lineSensor1Value, 1);
}

// Função para inicializar o sensor 2
void vLineSensor2Init(ADC_HandleTypeDef *hadc2) {
    sensor2ADC = hadc2;
    HAL_ADCEx_Calibration_Start(sensor2ADC, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(sensor2ADC, (uint32_t *)&lineSensor2Value, 1);
}

// Função para inicializar o sensor 3
void vLineSensor3Init(ADC_HandleTypeDef *hadc3) {
    sensor3ADC = hadc3;
    HAL_ADCEx_Calibration_Start(sensor3ADC, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(sensor3ADC, (uint32_t *)&lineSensor3Value, 1);
}

// Função para inicializar o sensor 4
void vLineSensor4Init(ADC_HandleTypeDef *hadc4) {
    sensor4ADC = hadc4;
    HAL_ADCEx_Calibration_Start(sensor4ADC, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(sensor4ADC, (uint32_t *)&lineSensor4Value, 1);
}

// Função para inicializar o sensor 5
void vLineSensor5Init(ADC_HandleTypeDef *hadc5) {
    sensor5ADC = hadc5;
    HAL_ADCEx_Calibration_Start(sensor5ADC, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(sensor5ADC, (uint32_t *)&lineSensor5Value, 1);
}

// Função para calcular a posição do carrinho com base nos sensores
float fLineSensorCalculatePosition(void) {
    // Pesos para cada sensor (da esquerda para a direita)
    int pesos[5] = {-2, -1, 0, 1, 2};
    float totalWeightedPosition = 0.0f;
    float totalWeight = 0.0f;

    // Array com os valores dos sensores
    uint32_t sensorValues[5] = {
        lineSensor1Value,
        lineSensor2Value,
        lineSensor3Value,
        lineSensor4Value,
        lineSensor5Value
    };

    for (int i = 0; i < 5; i++) {
        if (sensorValues[i] >= 300 && sensorValues[i] <= 500) {
            float weight = 1.0f / (sensorValues[i] - 300 + 1);
            totalWeightedPosition += pesos[i] * weight;
            totalWeight += weight;
        }
    }

    if (totalWeight == 0) {
        return 99; // Nenhum sensor detectou a linha
    }

    float posicao = totalWeightedPosition / totalWeight;
    return posicao;
}

void vLineSensorPIDControl(float left_encoder_speed, float right_encoder_speed) {
    // Obtém o tempo atual
    uint32_t current_time = HAL_GetTick();
    delta_time = (current_time - previous_time) / 1000.0f;

    if (delta_time <= 0) {
        delta_time = 0.01f; // Evita divisão por zero
    }

    float position = fLineSensorCalculatePosition();

    // Se nenhum sensor detectar a linha
    if (position == 99) {
        vSetRodasDC(0, 0);
        return;
    }

    // Calcula o erro (posição desejada é 0)
    error = 0 - position;

    // Calcula os termos integral e derivativo
    integral_error += error * delta_time;
    derivative_error = (error - previous_error) / delta_time;

    // Calcula a saída do controlador PID
    control_output = -(Kp * error + Ki * integral_error + Kd * derivative_error);

    // Limita a saída do controlador para evitar saturação
    if (control_output > max_duty_cycle) control_output = max_duty_cycle;
    if (control_output < -max_duty_cycle) control_output = -max_duty_cycle;

    previous_error = error;
    previous_time = current_time;

    // Ajusta velocidades dos motores com base nos sensores
    float left_speed = left_encoder_speed;
    float right_speed = right_encoder_speed;

    // Ajuste de velocidade para os sensores
    if (lineSensor4Value >= 380 && lineSensor4Value <= 550) {
        left_speed += left_speed * 0.05f; // Incremento de 5% na roda esquerda
        right_speed -= right_speed * 0.05f; // Decremento de 5% na roda direita
    }
    if (lineSensor5Value >= 380 && lineSensor5Value <= 550) {
        left_speed += left_speed * 0.15f; // Incremento de 10% na roda esquerda
        right_speed -= right_speed * 0.15f; // Decremento de 10% na roda direita
    }
    if (lineSensor2Value >= 380 && lineSensor2Value <= 500) {
        right_speed += right_speed * 0.05f; // Incremento de 5% na roda direita
        left_speed -= left_speed * 0.05f; // Decremento de 5% na roda esquerda
    }
    if (lineSensor1Value >= 380 && lineSensor1Value <= 500) {
        right_speed += right_speed * 0.15f; // Incremento de 10% na roda direita
        left_speed -= left_speed * 0.15f; // Decremento de 10% na roda esquerda
    }

    // Garante que as velocidades estão entre 0 e max_duty_cycle
    if (left_speed > max_duty_cycle) left_speed = max_duty_cycle;
    if (left_speed < 0.0f) left_speed = 0.0f;
    if (right_speed > max_duty_cycle) right_speed = max_duty_cycle;
    if (right_speed < 0.0f) right_speed = 0.0f;

    // Ajusta as velocidades dos motores
    vSetRodasDC(left_speed, right_speed);
}
