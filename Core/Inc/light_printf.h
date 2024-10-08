/// File name: light_printf.c                      //
// File description: Arquivo fornecido para nomear//
//                   as funcoes de print          //
// Authors name: Maria Eduarda Vieira Rangel      //
//               Pedro Henrique dos Santos        //
// Creation date: 21/04/2024                      //
// Revision date: 23/04/2024                      //
// ********************************************** //

#ifndef INC_LIGHT_PRINTF_H_
#define INC_LIGHT_PRINTF_H_

#include <stdarg.h>

int printf_(const char *format, ...);

int sprintf_(char *out, const char *format, ...);

int snprintf_( char *buf, unsigned int count, const char *format, ... );

#endif /* INC_LIGHT_PRINTF_H_ */
