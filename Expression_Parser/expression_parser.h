#ifndef EXPRESSION_PARSER_H_
#define EXPRESSION_PARSER_H_

#include "stdint.h"


uint8_t Validate_Expression(const char *str);
int32_t Evaluate_Expression(const char *str);


#endif /* EXPRESSION_PARSER_H_ */