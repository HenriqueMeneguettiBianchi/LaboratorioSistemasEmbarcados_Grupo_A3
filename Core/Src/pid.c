#include "pid.h"

float Kp, Ki, Kd;
float setpoint, integral, previous_error;

// Inicializa o PID
void PidInit(float kp, float ki, float kd, float initial_setpoint) {
    Kp = kp;
    Ki = ki;
    Kd = kd;
    setpoint = initial_setpoint; // Posição desejada (idealmente o centro da linha)
    integral = 0;
    previous_error = 0;
}

// Atualiza os dados do PID e calcula os ajustes para as rodas
void fPidUpdateData(float fLinhaPosicaoSensor, float *fRodaEsquerdaDC, float *fRodaDireitaDC) {
    float error = setpoint - fLinhaPosicaoSensor; // Erro de posição
    integral += error;
    float derivative = error - previous_error;

    float output = Kp * error + Ki * integral + Kd * derivative; // Saída do PID
    previous_error = error;

    // Saturação do output
    if (output > 1.0) output = 1.0;
    if (output < -1.0) output = -1.0;

    // Ajustar a velocidade das rodas com base na saída do PID
    float base_speed = 0.34; // Velocidade base das rodas
    *fRodaEsquerdaDC = base_speed - output;  // Roda esquerda ajusta para o erro
    *fRodaDireitaDC = base_speed + output; // Roda direita ajusta para o erro

    // Saturação das velocidades das rodas
    if (*fRodaEsquerdaDC < 0) *fRodaEsquerdaDC = 0;
    if (*fRodaDireitaDC < 0) *fRodaDireitaDC = 0;
}
