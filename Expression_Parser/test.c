#include "stdio.h"
#include "expression_parser.h"

void main()
{
    char *exp1 = "10*5(10+30))";
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
        printf("result = %li\n", Evaluate_Expression2(exp1));
    }
    else
    {
        printf("invalid expression!!\n");
    }

    printf("result = %li\n", Evaluate_Expression2(exp2));
    printf("result = %li\n", Evaluate_Expression2(exp3));
    printf("result = %li\n", Evaluate_Expression2(exp4));
    printf("result = %li\n", Evaluate_Expression2(exp5));
    printf("result = %li\n", Evaluate_Expression2(exp6));
    printf("result = %li\n", Evaluate_Expression2(exp7));
    printf("result = %li\n", Evaluate_Expression2(exp8));
    printf("result = %li\n", Evaluate_Expression2(exp9));
}