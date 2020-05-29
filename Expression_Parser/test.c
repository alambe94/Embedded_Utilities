#include "stdio.h"
#include "expression_parser.h"

void main()
{
char *exp1 = "10+10+30";
char *exp2 = "10*10*30";
char *exp3 = "10+10*30";
char *exp4 = "10-10+30";
char *exp5 = "10-10*30";
char *exp6 = "10-10-30";
char *exp7 = "10/10/30";
char *exp8 = "10+10-30";
char *exp9 = "10/10*30";

printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp1));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp2));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp3));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp4));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp5));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp6));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp7));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp8));
printf("result = %li\n", Parse_Expression((EXP_Handle_t*)&exp9));

}