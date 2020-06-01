#ifndef EXPRESSION_PARSER_H_
#define EXPRESSION_PARSER_H_

#include "stdint.h"


uint8_t Validate_Expression(const char *str);
int32_t Evaluate_Expression(const char *str);
uint8_t Evaluate_Expression2(const char *str, int32_t *value, uint8_t *sign);

#endif /* EXPRESSION_PARSER_H_ */