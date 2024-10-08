#include "main.h"
#include "tim.h"

#define CONTADOR_MAX 50000
#define FATOR_MULTIPLICACAO_VELOCIDADE 15.0f
#define FREQUENCIA_CLOCK   170000000
#define PRESCALAR_TIMER  1700

static uint32_t valorCaptura1 = 0;
static uint32_t valorCaptura2 = 0;
static uint32_t diferencaTempo = 0;
static int flagPrimeiraCaptura = 0;

static float frequenciaRodaEsquerda = 0;
static float frequenciaRodaDireita = 0;
float velocidadeRodaEsquerda = 0;
float velocidadeRodaDireita = 0;

void inicializarEncoders(TIM_HandleTypeDef *timer16, TIM_HandleTypeDef *timer17) {
    HAL_TIM_IC_Start_IT(timer16, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(timer17, TIM_CHANNEL_1);
}

void processarCaptura(TIM_HandleTypeDef *htim, float *frequencia, float *velocidade) {
    if (flagPrimeiraCaptura == 0) {
        valorCaptura1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        flagPrimeiraCaptura = 1;
    } else {
        valorCaptura2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        diferencaTempo = (valorCaptura2 >= valorCaptura1) ? (valorCaptura2 - valorCaptura1) : (CONTADOR_MAX - valorCaptura1 + valorCaptura2);

        float clockReferencia = FREQUENCIA_CLOCK / PRESCALAR_TIMER;
        *frequencia = clockReferencia / diferencaTempo;
        *velocidade = (*frequencia / 20) * FATOR_MULTIPLICACAO_VELOCIDADE;

        __HAL_TIM_SET_COUNTER(htim, 0);
        flagPrimeiraCaptura = 0;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim16) {
        processarCaptura(htim, &frequenciaRodaEsquerda, &velocidadeRodaEsquerda);
    } else if (htim == &htim17) {
        processarCaptura(htim, &frequenciaRodaDireita, &velocidadeRodaDireita);
    }
}
