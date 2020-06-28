/*
 * file version V0.0.4
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/**
 *      https://stackoverflow.com/questions/9329406/evaluating-arithmetic-expressions-from-string-in-c
 *      https://en.wikipedia.org/wiki/Recursive_descent_parser
 */

/**
 * Change Log
 *
 * ***V0.0.1***
 * 1. simplified original code
 * 2. removed double pointer, intead using wrapped struct
 *
 * ***V0.0.2***
 * 1. added Validate_Expression()
 * 2. added Evaluate_Expression2()
 *
 * ***V0.0.3***
 * 1. added floating point evaluation
 * 2. added suffix such as u, m, K, M (micro, millis, kilo, mega)
 *
 * ***V0.0.4***
 * 1. added e, E power of 10 exponential
 * 2. added Evaluate_Expression3(), Evaluate_Expression4() for truncated int32_t output
 *
 */

/** standard includes */

/** parser includes */
#include "expression_parser.h"

/* pointer wrapped in struct instead of pointer to pointer or double pointer */
typedef struct EXP_Handle_t
{
    const char *EXP;
} EXP_Handle_t;

static float power_of_10(int8_t power);
static char peek(EXP_Handle_t *handle);
static char get(EXP_Handle_t *handle);
static float number(EXP_Handle_t *handle);
static float factor(EXP_Handle_t *handle);
static float term(EXP_Handle_t *handle);
static float parse_expression(EXP_Handle_t *handle);

static float power_of_10(int8_t power)
{
    float result = 1;

    if (power > 0)
    {
        while (power--)
        {
            result *= 10;
        }
    }
    else
    {
        while (power++)
        {
            result /= 10;
        }
    }

    return result;
}

static char peek(EXP_Handle_t *handle)
{
    /** space is used as delimeter */
    if (*(handle->EXP) == ' ')
    {
        return '\0';
    }

    return *(handle->EXP);
}

static char get(EXP_Handle_t *handle)
{
    return *(handle->EXP)++;
}

static float number(EXP_Handle_t *handle)
{
    float result = get(handle) - '0';
    uint8_t dp_count = 0; /** decimal point */

    while ((peek(handle) >= '0' && peek(handle) <= '9') || peek(handle) == '.')
    {
        if (peek(handle) == '.')
        {
            get(handle);
            dp_count = 1;
        }
        if (dp_count)
        {
            result = result + (float)(get(handle) - '0') / power_of_10(dp_count);
            dp_count++;
        }
        else
        {
            result = 10 * result + get(handle) - '0';
        }
    }

    if (peek(handle) == 'u')
    {
        /** micro */
        get(handle);
        result /= 1000000;
    }
    else if (peek(handle) == 'm')
    {
        /** milli */
        get(handle);
        result /= 1000;
    }
    else if (peek(handle) == 'K')
    {
        /** Kilo */
        get(handle);
        result *= 1000;
    }
    else if (peek(handle) == 'M')
    {
        /** Mega */
        get(handle);
        result *= 1000000;
    }
    else if (peek(handle) == 'e' || peek(handle) == 'E')
    {
        get(handle);
        result *= power_of_10(factor(handle));
    }

    return result;
}

static float factor(EXP_Handle_t *handle)
{
    if (peek(handle) >= '0' && peek(handle) <= '9')
    {
        return number(handle);
    }
    else if (peek(handle) == '(')
    {
        get(handle); // '('
        float result = parse_expression(handle);
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

static float term(EXP_Handle_t *handle)
{
    float result = factor(handle);

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

static float parse_expression(EXP_Handle_t *handle)
{
    float result = term(handle);

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

uint8_t Validate_Expression(const char *str)
{
    uint8_t xreturn = 1;
    int8_t brace_count = 0;

    /* skip '=' if any*/
    if (*str == '=')
    {
        str++;
    }

    while (*str)
    {
        /** space is used as delimeter */
        if (*str == ' ')
        {
            break;
        }
        else if (*str == '(')
        {
            brace_count++;
        }
        else if (*str == ')')
        {
            brace_count--;
            if (brace_count < 0)
            {
                xreturn = 0;
                break;
            }
        }
        else if (*str >= '*' && *str <= '9')
        {
            // *+-./0123456789 valid characters. remove ','
            if (*str == ',')
            {
                xreturn = 0;
                break;
            }
        } /** add chars here to ignore theme */
        else if (*str == 'u' || *str == 'm' || *str == 'K' || *str == 'M' || *str == 'e' || *str == 'E')
        {
            // u->micro
            // m->milli
            // K->kilo
            // M->Mega
            // e, E power of 10
        }
        else
        {
            xreturn = 0;
            break;
        }

        str++;
    }

    if (brace_count)
    {
        xreturn = 0;
    }

    return xreturn;
}

/**
 * return evaluated expression in float
 */
float Evaluate_Expression(const char *str)
{
    /* skip '=' if any*/
    if (*str == '=')
    {
        str++;
    }

    return parse_expression((EXP_Handle_t *)&str);
}

/**
 * return evaluated expression in float, sign and magnitude separately
 */
uint8_t Evaluate_Expression2(const char *str, float *value, uint8_t *sign)
{
    uint8_t xreturn = 0;

    if (Validate_Expression(str))
    {
        xreturn = 1;

        float val = Evaluate_Expression(str);

        if (val >= 0)
        {
            *sign = 1;
        }
        else
        {
            *sign = 0;
            val *= -1;
        }

        *value = val;
    }

    return xreturn;
}

/**
 * return evaluated expression truncated to int32_t
 */
int32_t Evaluate_Expression3(const char *str)
{
    return (int32_t)Evaluate_Expression(str);
}

/**
 * return evaluated expression truncated to int32_t, sign and magnitude separately
 */
uint8_t Evaluate_Expression4(const char *str, int32_t *value, uint8_t *sign)
{
    uint8_t xreturn = 0;
    float tmp = 0;

    xreturn = Evaluate_Expression2(str, &tmp, sign);
    *value = (int32_t)tmp;

    return xreturn;
}