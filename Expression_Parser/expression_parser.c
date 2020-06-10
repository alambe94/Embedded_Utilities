/**
 *      https://stackoverflow.com/questions/9329406/evaluating-arithmetic-expressions-from-string-in-c
 *      https://en.wikipedia.org/wiki/Recursive_descent_parser
 */
/*
 * file version V0.0.0
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

#include "expression_parser.h"

/* pointer wrapped in struct instead of pointer to pointer or double pointer */
typedef struct EXP_Handle_t
{
    const char *EXP;
} EXP_Handle_t;

static int32_t Parse_Expression(EXP_Handle_t *handle);

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

static int32_t Parse_Expression(EXP_Handle_t *handle)
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

int32_t Evaluate_Expression(const char *str)
{
    /* skip '=' if any*/
    if (*str == '=')
    {
        str++;
    }
    return Parse_Expression((EXP_Handle_t *)&str);
}

uint8_t Evaluate_Expression2(const char *str, int32_t *value, uint8_t *sign)
{
    uint8_t xreturn = 0;

    if (Validate_Expression(str))
    {
        xreturn = 1;

        int32_t val = Evaluate_Expression(str);

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
