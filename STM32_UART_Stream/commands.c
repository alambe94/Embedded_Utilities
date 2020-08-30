#include <stdint.h>
#include <string.h>

#include "log_uart.h"

#define CMD_MAX_ARGS 10

typedef struct
{
    const char *CMD;
    const char *Help;
    uint8_t Len;
    void (*CMD_Callback)(uint8_t argc, char *argv[]);
} CMD_t;

void CMD_Parse_ARGS(char *cmd, uint8_t *argc, char *argv[]);

void CMD_Help_CB(uint8_t argc, char *argv[]);
void CMD_Time_CB(uint8_t argc, char *argv[]);
void CMD_Calibrate_ADC_CB(uint8_t argc, char *argv[]);

CMD_t Logger_CMDS[] =
    {{.CMD = "help", .Help = "list all resgistred commands", .Len = strlen("help"), .CMD_Callback = CMD_Help_CB},
     {.CMD = "time", .Help = "update date and time from prompt", .Len = strlen("time"), .CMD_Callback = CMD_Time_CB}};

void CMD_Process(char *cmd)
{
    uint8_t argc = 0;
    char *argv[CMD_MAX_ARGS];

    for (uint32_t i = 0; i < sizeof(Logger_CMDS) / sizeof(Logger_CMDS[0]); i++)
    {
        uint16_t cmd_len = Logger_CMDS[i].Len;

        if ((cmd[cmd_len] == ' ') || (cmd[cmd_len] == 0x00))
        {
            if (strncmp(cmd, Logger_CMDS[i].CMD, cmd_len) == 0)
            {
                if (Logger_CMDS[i].CMD_Callback)
                {
                    CMD_Parse_ARGS(cmd, &argc, argv);
                    Logger_CMDS[i].CMD_Callback(argc, argv);
                }
            }
        }
    }
}

void CMD_Parse_ARGS(char *cmd, uint8_t *argc, char *argv[])
{
    uint8_t argc_temp = 0;
    /** arg 0 is input command */
    argv[argc_temp++] = cmd;

    while (argc_temp < CMD_MAX_ARGS)
    {
        while (((*cmd) != 0x00) && ((*cmd) != ' '))
        {
            cmd++;
        }

        while ((*cmd) == ' ')
        {
            cmd++;
        }

        if (*cmd != 0x00)
        {
            argv[argc_temp++] = cmd;
        }
        else
        {
            break;
        }
    }

    *argc = argc_temp;
}

/************************************ callbacks *************************************/
void CMD_Help_CB(uint8_t argc, char *argv[])
{
    for (uint32_t i = 0; i < sizeof(Logger_CMDS) / sizeof(Logger_CMDS[0]); i++)
    {
        Log_UART_Printf("%s:%s\r\n", Logger_CMDS[i].CMD, Logger_CMDS[i].Help);
    }
}

void CMD_Time_CB(uint8_t argc, char *argv[])
{
	for(uint32_t i=0; i<argc; i++)
	{

	}
}

void CMD_Calibrate_ADC_CB(uint8_t argc, char *argv[])
{
}
