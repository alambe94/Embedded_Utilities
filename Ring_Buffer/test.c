#include "stdio.h"
#include "ring_buffer.h"
#include "sys/timeb.h"

void Ring_Buffer_Test_Char();
void Ring_Buffer_Test_Short();
void Ring_Buffer_Test_Custom();

uint64_t system_current_time_millis()
{
#if defined(_WIN32)
    struct _timeb timebuffer;
    _ftime(&timebuffer);
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
#endif

    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
}

void main()
{
    //Ring_Buffer_Test_Char();
    //Ring_Buffer_Test_Short();
    Ring_Buffer_Test_Custom();
}

uint8_t Storage_Char[100];
Ring_Buffer_t RB_Char;
void Ring_Buffer_Test_Char()
{
    uint8_t data = 0;
    uint64_t time_elapsed = system_current_time_millis();

    printf("elements = %zu\n", sizeof(Storage_Char) / sizeof(Storage_Char[0]));

    Ring_Buffer_Init(&RB_Char, (uint8_t *)Storage_Char, sizeof(Storage_Char[0]), sizeof(Storage_Char) / sizeof(Storage_Char[0]));

    for (uint32_t i = 0; i < sizeof(Storage_Char) / sizeof(Storage_Char[0]); i++)
    {
        Ring_Buffer_Put(&RB_Char, &data);
        data++;
    }

    while (Ring_Buffer_Get(&RB_Char, &data))
    {
        printf("%d\n", data);
    }

    time_elapsed = system_current_time_millis() - time_elapsed;

    printf("elapsed time %lu\n", time_elapsed);
}

uint16_t Storage_Short[10000];
Ring_Buffer_t RB_Short;
void Ring_Buffer_Test_Short()
{
    uint16_t data = 0;
    uint64_t time_elapsed = system_current_time_millis();

    printf("elements = %zu\n", sizeof(Storage_Short) / sizeof(Storage_Short[0]));

    Ring_Buffer_Init(&RB_Short, (uint8_t *)Storage_Short, sizeof(Storage_Short[0]), sizeof(Storage_Short) / sizeof(Storage_Short[0]));

    for (uint32_t i = 0; i < sizeof(Storage_Short) / sizeof(Storage_Short[0]); i++)
    {
        Ring_Buffer_Put(&RB_Short, &data);
        data++;
    }

    while (Ring_Buffer_Get(&RB_Short, &data))
    {
        printf("%d\n", data);
    }

    time_elapsed = system_current_time_millis() - time_elapsed;

    printf("elapsed time %lu\n", time_elapsed);
}

struct XYZ_t
{
    uint64_t x;
    uint64_t y;
    uint64_t z;
};

struct XYZ_t Storage_Custom[10000];
Ring_Buffer_t RB_Custom;
void Ring_Buffer_Test_Custom()
{
    struct XYZ_t data = {0, 0, 0};
    uint64_t time_elapsed = system_current_time_millis();

    printf("elements = %zu\n", sizeof(Storage_Custom) / sizeof(Storage_Custom[0]));

    Ring_Buffer_Init(&RB_Custom, (uint8_t *)Storage_Custom, sizeof(Storage_Custom[0]), sizeof(Storage_Custom) / sizeof(Storage_Custom[0]));

    for (uint32_t i = 0; i < sizeof(Storage_Custom) / sizeof(Storage_Custom[0]); i++)
    {
        Ring_Buffer_Put(&RB_Custom, &data);
        data.x++;
        data.y += 2;
        data.z += 10;
    }

    /* get count test */
    printf("counts = %ui\n", Ring_Buffer_Get_Count(&RB_Custom));

    /* peek test*/
    for (uint32_t i = 0; i < sizeof(Storage_Custom) / sizeof(Storage_Custom[0]); i++)
    {
        Ring_Buffer_Peek(&RB_Custom, &data, i);
        printf("x%lu y%lu z%lu\n", data.x, data.y, data.z);
    }

    /* search test */
    struct XYZ_t search = {9998, 19996, 99980};
    uint32_t position = 0;
    Ring_Buffer_Search(&RB_Custom, &search, &position);
    printf("position = %ui\n", position);
    Ring_Buffer_Peek(&RB_Custom, &data, position);
    printf("x%lu y%lu z%lu\n", data.x, data.y, data.z);

    /* get test */
    while (Ring_Buffer_Get(&RB_Custom, &data))
    {
        printf("x%lu y%lu z%lu\n", data.x, data.y, data.z);
    }

    time_elapsed = system_current_time_millis() - time_elapsed;

    printf("elapsed time %lu\n", time_elapsed);
}
