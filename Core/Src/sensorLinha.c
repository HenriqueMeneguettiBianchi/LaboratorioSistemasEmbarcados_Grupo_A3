// *****************
// File name: sensorLinha.c
// File description: Biblioteca para sensor de faixa
//
// Author name:
// Creation date:
// Revision date:
// *****************

#include "sensorLinha.h"
#include "motor.h"
#include "main.h" // Inclui os headers necessários do HAL
#include <stdbool.h> // Inclua isso para usar o tipo bool
#include "tim.h"

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
float fsaidaControladorLinha;
float fsaidaDutyEsquerda;
float fsaidaDutyDireita;
float posicao3;
float EsquMaiorDir;

int minThresholds[5] = {350, 350, 350, 350, 350};
int maxThresholds[5] = {700, 700, 700, 700, 700};
//int maxThresholds[5] = {640, 650, 550, 620, 620};
//int minThresholds[5] = {1000, 1000, 1000, 1000, 1000};
//int maxThresholds[5] = {3000, 3000, 3000, 3000, 3000};
float velocidadeEsquerda, velocidadeDireita;
// Variáveis do controlador PID
float kp = 4.0;  // Constante Proporcional
float ki = 0.0;  // Constante Integral
float kd = 1.2;  // Constante Derivativa

// Definição da velocidade base
int velocidadeBase = 25;

float error = 0.0f;
float previous_error = 0.0f;
float integral_error = 0.0f;
float derivative_error = 0.0f;
float control_output = 0.0f;

// Variáveis de tempo
float delta_time = 0.07f; // Assume um loop de 10ms
uint32_t previous_time = 0;

// Velocidade base dos motores
float base_speed = 0.34f; // Valor entre 0.0f e 1.0f
float max_duty_cycle = 0.45f; // Limite máximo do duty cycle

/* Defina o limite para detecção de linha (ajustável) */

// ******************* //
// Method name: vLineSensorInit                          //
// Method description: Inicialização do ponteiro ADC,    //
//     calibração e start ADC                            //
// Input params: ADC_HandleTypeDef *hadc                 //
// Output params: Void                                   //
//*******************//
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
        // Verifica se o sensor está detectando a linha branca com base em intervalos individuais
        if (sensorValues[i] >= minThresholds[i] && sensorValues[i] <= maxThresholds[i]) {
            // Calcular o peso inverso para priorizar o sensor com menor valor
            float weight = 1.0f / (sensorValues[i] - minThresholds[i] + 1); // Adiciona 1 para evitar divisão por zero
            totalWeightedPosition += pesos[i] * weight;
            totalWeight += weight;
        }
    }


    // Verifica se algum sensor detectou a linha
    if (totalWeight == 0) {
        // Nenhum sensor detectou a linha
        return 99;
    }

    // Calcula a posição ponderada
    float posicao = totalWeightedPosition / totalWeight;
    posicao3 = posicao;
    return posicao;
}

// Definição das constantes do PID


// Função para ajustar a velocidade das rodas com base na saída do PID
// Função para ajustar a velocidade das rodas com base na saída do PID
void vLineSensorPIDControl(void) {
    // Obtém o tempo atual
    uint32_t current_time = HAL_GetTick();
    delta_time = (current_time - previous_time) / 1000.0f; // Converte ms para segundos

    if (delta_time <= 0) {
        delta_time = 0.01f; // Evita divisão por zero
    }

    // Calcula a posição atual a partir dos sensores
    float position = fLineSensorCalculatePosition();

    // Verifica se a linha foi detectada
    if (position == 99) {
        // Se nenhum sensor detectar a linha, o carrinho pode parar ou continuar em frente
        // Aqui, vamos continuar em frente com velocidade base
        vSetRodasDC(0, 0);
    	//vSetRodaDireitaDC(0.5);
    	   // vSetRodaEsquerdaDC(0.5);
        return;
    }

    // Calcula o erro (posição desejada é 0)
    error = 0-position; // Inverte o sinal para correção na direção oposta

    // Calcula os termos integral e derivativo
    integral_error += error * delta_time;
    derivative_error = (error - previous_error) / delta_time;

    // Calcula a saída do controlador PID
    control_output = -(kp * error + ki * integral_error + kd * derivative_error);
    fsaidaControladorLinha=control_output;
    if (control_output == 0) {
            velocidadeEsquerda = velocidadeBase;
            velocidadeDireita = velocidadeBase;
        } else {
            // Caso contrário, ajusta a velocidade de cada roda com base no output do PID
            // Se o erro for positivo, roda esquerda acelera e a direita desacelera e vice-versa
            velocidadeEsquerda = velocidadeBase + control_output;
            velocidadeDireita = velocidadeBase - control_output;

            // Limitar a velocidade mínima para 0 (evita valores negativos nas rodas)
            if (velocidadeEsquerda < 0) {
                velocidadeEsquerda = 0;
            }
            if (velocidadeDireita < 0) {
                velocidadeDireita = 0;
            }
        }
}
