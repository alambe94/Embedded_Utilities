#include "cli.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

/*************************************************************************/
void Sprint_Ok_Helper(const char *in, char *out, uint16_t out_max)
{
    snprintf(out, out_max, "\r\nOk:%s\r\n", in);
}

void Sprint_Error_Helper(const char *in, char *out, uint16_t out_max)
{
    snprintf(out, out_max, "\r\nError:CMD invalid->%s\r\n", in);
}

void Sprint_Ok_Done_Helper(const char *in, char *out, uint16_t out_max)
{
    snprintf(out, out_max, "\r\nOk:%s\r\nDone\r\n", in);
}

uint8_t Parse_Integer(const char *param, int32_t *value)
{
    char str_to_int[11] = "0";
    uint8_t i = 0;
    uint8_t ok_flag = 1;
    int8_t sign = 0;

    if (*param == '-')
    {
        param++;
        sign = -1; //negative
    }
    else
    {
        sign = 1; //positive
    }

    while (*param != '\0' && *param != ' ')
    {

        if (*param < '0' || *param > '9' || i >= 10)
        {
            ok_flag = 0; //not number
            break;
        }
        str_to_int[i++] = *param++;
    }

    if (ok_flag)
    {
        *value = sign * atoi(str_to_int);
    }

    return ok_flag;
}

/************CLI Callback Prototypes starts*********************************/
static uint8_t Add_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max);

static uint8_t Sub_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max);

static uint8_t Mul_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max);

static uint8_t Div_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max);

static uint8_t Pow_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max);

static uint8_t Sqrt_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max);
/************CLI Callback Prototypes Ends***********************************/

/************CLI Commands Definitions starts********************************/
CLI_Command_t Add_Definition =
    {
        .CLI_Command = "add",
        .CLI_Command_Description = "addition",
        .CLI_Callback = Add_Callback};

CLI_Command_t Sub_Definition =
    {
        .CLI_Command = "sub",
        .CLI_Command_Description = "subtraction",
        .CLI_Callback = Sub_Callback};

CLI_Command_t Mul_Definition =
    {
        .CLI_Command = "mul",
        .CLI_Command_Description = "multiplication",
        .CLI_Callback = Mul_Callback};

CLI_Command_t Div_Definition =
    {
        .CLI_Command = "div",
        .CLI_Command_Description = "division",
        .CLI_Callback = Div_Callback};

CLI_Command_t Pow_Definition =
    {
        .CLI_Command = "pow",
        .CLI_Command_Description = "raise power",
        .CLI_Callback = Pow_Callback};

CLI_Command_t Sqrt_Definition =
    {
        .CLI_Command = "sqrt",
        .CLI_Command_Description = "square root",
        .CLI_Callback = Sqrt_Callback};
/************CLI Commands Definitions ends***********************************/

/************CLI Callback Implementation starts******************************/
static uint8_t Add_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    rslt += tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%i\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Sub_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    rslt -= tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%i\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Mul_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    rslt *= tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%i\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Div_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    if (tmp == 0)
                    {
                        snprintf(out_buffer, out_max,
                                 "divide by zero error, %d argument is zero",
                                 i - 1);
                        return 0;
                    }
                    rslt *= tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%i\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Pow_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc == 3)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            if (Parse_Integer(argv[2], &tmp))
            {
                rslt = pow(rslt, tmp);
            }
            else
            {
                is_command_valid = 0;
            }
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid && argc)
    {
        snprintf(out_buffer, out_max, "\r\n%i\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Sqrt_Callback(uint8_t argc, const char *argv[], char *out_buffer, uint16_t out_max)
{
    int32_t tmp = 0;

    if (Parse_Integer(argv[1], &tmp) && argc > 1)
    {
        int32_t rslt = sqrt(tmp);
        snprintf(out_buffer, out_max, "\r\n%i\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

void CLI_Add_Test_Commands()
{
    CLI_Add_Command(&Add_Definition);
    CLI_Add_Command(&Sub_Definition);
    CLI_Add_Command(&Mul_Definition);
    CLI_Add_Command(&Div_Definition);
    CLI_Add_Command(&Pow_Definition);
    CLI_Add_Command(&Sqrt_Definition);
}