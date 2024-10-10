// *****************
// File name: sensorLinha.c
// File description: Biblioteca para sensor de faixa
//
// Author name:      Maria Julia Braz, Paulo Roberto
// Creation date:    15/05/2024
// Revision date:    02/10/2024
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
float fsaidaControladorLinha;

int minThresholds[5] = {300, 300, 300, 300, 300};
int maxThresholds[5] = {500, 500, 500, 550, 550};

// Variáveis do controlador PID
float Kp = 1.2f; // Ajuste conforme necessário
float Ki = 0.0f; // Pode iniciar com 0 e ajustar depois
float Kd = 0.5f; // Pode iniciar com 0 e ajustar depois

float error = 0.0f;
float previous_error = 0.0f;
float integral_error = 0.0f;
float derivative_error = 0.0f;

float control_output = 0.0f;

// Variáveis de tempo
float delta_time = 0.01f; // Assume um loop de 10ms
uint32_t previous_time = 0;

// Velocidade base dos motores
float base_speed = 0.5f; // Valor entre 0.0f e 1.0f


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


// ******************* //
// Method name: bLineSensorIsOnLine                      //
// Method description: Verifica se o sensor detecta linha//
// Input params: void                                    //
// Output params: uint8_t (1 = linha detectada, 0 = não) //
//*******************//
uint8_t bLineSensorIsOnLine(){
    HAL_Delay(100);  // Atraso para estabilização

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
    return posicao;
}

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
       // vSetRodasDC(0, 0);
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
    control_output = -(Kp * error + Ki * integral_error + Kd * derivative_error);
    fsaidaControladorLinha=control_output;
    // Limita a saída do controlador para evitar saturação
    if (control_output > base_speed) control_output = base_speed;
    if (control_output < -base_speed) control_output = -base_speed;

    // Atualiza o erro e tempo anteriores
    previous_error = error;
    previous_time = current_time;

    // Calcula as velocidades dos motores
    float left_speed = base_speed + control_output;
    float right_speed = base_speed - control_output;

    // Garante que as velocidades estão entre 0 e 1
    if (left_speed > 1.0f) left_speed = 1.0f;
    if (left_speed < 0.0f) left_speed = 0.0f;
    if (right_speed > 1.0f) right_speed = 1.0f;
    if (right_speed < 0.0f) right_speed = 0.0f;
    if (position == 0) {
        left_speed = 0.5f;
        right_speed = 0.5f;
    }
    // Ajusta as velocidades dos motores
   //vSetRodasDC(left_speed, right_speed);
    //vSetRodaDireitaDC(right_speed);
    //vSetRodaEsquerdaDC(left_speed);
}

