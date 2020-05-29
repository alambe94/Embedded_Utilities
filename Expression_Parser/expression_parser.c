/*
 *      https://stackoverflow.com/questions/9329406/evaluating-arithmetic-expressions-from-string-in-c
 *      https://en.wikipedia.org/wiki/Recursive_descent_parser
 */

#include "expression_parser.h"

static char peek(EXP_Handle_t *handle)
{
    if (*(handle->EXP) == ' ')
    {
        return *++(handle->EXP);
    }
    return *(handle->EXP);
}

static char get(EXP_Handle_t *handle)
{
    return *(handle->EXP)++;
}

static int32_t number(EXP_Handle_t *handle)
{
    int32_t result = get(handle) - '0';
    while (peek(handle) >= '0' && peek(handle) <= '9')
    {
        result = 10 * result + get(handle) - '0';
    }
    return result;
}

static int32_t factor(EXP_Handle_t *handle)
{
    if (peek(handle) >= '0' && peek(handle) <= '9')
    {
        return number(handle);
    }
    else if (peek(handle) == '(')
    {
        get(handle); // '('
        int32_t result = Parse_Expression(handle);
        get(handle); // ')'
        return result;
    }
    else if (peek(handle) == '-')
    {
        get(handle); // '-'
        return -factor(handle);
    }
    return 0; // error
}

static int32_t term(EXP_Handle_t *handle)
{
    int32_t result = factor(handle);

    while (peek(handle) == '*' || peek(handle) == '/')
    {
        if (get(handle) == '*')
        {
            result *= factor(handle);
        }
        else
        {
            result /= factor(handle);
        }
    }
    return result;
}

int32_t Parse_Expression(EXP_Handle_t *handle)
{
    int32_t result = term(handle);

    while (peek(handle) == '+' || peek(handle) == '-')
    {
        if (get(handle) == '+')
        {
            result += term(handle);
        }
        else
        {
            result -= term(handle);
        }
    }
    return result;
}
