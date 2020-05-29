#ifndef EXPRESSION_PARSER_H_
#define EXPRESSION_PARSER_H_

#include "stm32XX_hal.h"

/* pointer wrapped in structure instead of pointer to pointer or double */
typedef struct EXP_Handle_t
{
    char *EXP;
} EXP_Handle_t;

int32_t Parse_Expression(EXP_Handle_t *handle);


#endif /* EXPRESSION_PARSER_H_ */