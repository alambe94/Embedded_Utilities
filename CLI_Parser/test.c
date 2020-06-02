#include "stdio.h"
#include "string.h"
#include "cli.h"

void main()
{
    uint8_t in_buffer[128];
    uint8_t out_buffer[128];
    uint8_t call_again = 0;

    CLI_Init();

    extern void CLI_Add_Test_Commands();
    CLI_Add_Test_Commands();

    while (1)
    {
        memset(in_buffer, 0x00, sizeof(in_buffer));
        fgets(in_buffer, sizeof(in_buffer), stdin);
        in_buffer[strnlen(in_buffer, sizeof(in_buffer)) - 1] = '\0';

        do
        {
            call_again = CLI_Process_Command(in_buffer, out_buffer, sizeof(out_buffer));
            printf("%s", out_buffer);
        } while (call_again);

        printf("%s", "->");
    }
}