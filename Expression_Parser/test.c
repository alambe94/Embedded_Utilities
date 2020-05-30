#include "stdio.h"
#include "expression_parser.h"

void main()
{
    char *exp1 = "10*5(10+30)1";
    char *exp2 = "10*10/30";
    char *exp3 = "10+10*30";
    char *exp4 = "10-10+30";
    char *exp5 = "10-10*30";
    char *exp6 = "10-10-30";
    char *exp7 = "10/10/30";
    char *exp8 = "10+10-30";
    char *exp9 = "10/10*30";

    if (Validate_Expression(exp1))
    {
        printf("result = %li\n", Evaluate_Expression(exp1));
    }
    else
    {
        printf("invalid expression!!\n");
    }

    printf("result = %li\n", Evaluate_Expression(exp2));
    printf("result = %li\n", Evaluate_Expression(exp3));
    printf("result = %li\n", Evaluate_Expression(exp4));
    printf("result = %li\n", Evaluate_Expression(exp5));
    printf("result = %li\n", Evaluate_Expression(exp6));
    printf("result = %li\n", Evaluate_Expression(exp7));
    printf("result = %li\n", Evaluate_Expression(exp8));
    printf("result = %li\n", Evaluate_Expression(exp9));
}