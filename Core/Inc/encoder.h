#ifndef ENCODER_H
#define ENCODER_H

#include "main.h"
#include "tim.h"

// Definições de constantes
#define FREQUENCIA_CLOCK   170000000   // Frequência do clock
#define PRESCALAR_TIMER    1700        // Fator de prescaler
#define CONTADOR_MAX       50000       // Valor máximo do contador
#define FATOR_MULTIPLICACAO_VELOCIDADE 20.4f // Fator para conversão de velocidade

extern int overflow1;
extern int overflow2;
extern float velocidadeRodaDireita;
extern float velocidadeRodaEsquerda;

// Funções para inicializar os encoders
void inicializarEncoders(TIM_HandleTypeDef *timer16, TIM_HandleTypeDef *timer17);

// Callback chamado quando uma captura de entrada é realizada
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

// Função para processar a captura de pulsos do encoder
void processarCaptura(TIM_HandleTypeDef *htim, float *frequencia, float *velocidade);

void processarCaptura2(TIM_HandleTypeDef *htim, float *frequencia, float *velocidade);


float fGetVelocidadeRodaEsquerda(void);
float fGetVelocidadeRodaDireita(void);
__weak void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim);

#endif // ENCODER_H
