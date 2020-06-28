#include <stdio.h>
#include "expression_parser.h"

void main()
{
    char *exp1 = "10*5(10+30))";

    char *exp2 = "10*10/30";
    char *exp3 = "10.147+10*30";
    char *exp4 = "10-10+30";
    char *exp5 = "10-10*30";

    char *exp6 = "10.2-10-30";
    char *exp7 = "10/10.27/30";
    char *exp8 = "10+10-30";
    char *exp9 = "10/10.36*30.5";

    char *exp10 = "1u*1K";
    char *exp11 = "1m*1K";
    char *exp12 = "1M*1K";

    char *exp13 = "1u*1M";
    char *exp14 = "1m*1M";

    char *exp15 = "10e2/1E4+5.5";
    char *exp16 = "1e-5+74";

    if (Validate_Expression(exp1))
    {
        printf("exp1 = %.4f\n", Evaluate_Expression(exp1));
    }
    else
    {
        printf("exp1 invalid expression!!\n");
    }

    printf("exp2 = %.4f\n", Evaluate_Expression(exp2));
    printf("exp3 = %.4f\n", Evaluate_Expression(exp3));
    printf("exp4 = %.4f\n", Evaluate_Expression(exp4));
    printf("exp5 = %.4f\n", Evaluate_Expression(exp5));
    printf("exp6 = %.4f\n", Evaluate_Expression(exp6));
    printf("exp7 = %.4f\n", Evaluate_Expression(exp7));
    printf("exp8 = %.4f\n", Evaluate_Expression(exp8));
    printf("exp9 = %.4f\n", Evaluate_Expression(exp9));

    printf("exp10 = %.4f\n", Evaluate_Expression(exp10));
    printf("exp11 = %.4f\n", Evaluate_Expression(exp11));
    printf("exp12 = %.4f\n", Evaluate_Expression(exp12));
    printf("exp13 = %.4f\n", Evaluate_Expression(exp13));
    printf("exp14 = %.4f\n", Evaluate_Expression(exp14));

    printf("exp15 = %.5f\n", Evaluate_Expression(exp15));
    printf("exp16 = %.5f\n", Evaluate_Expression(exp16));
}
