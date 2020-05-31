#include "stdio.h"
#include "ring_buffer.h"
#include "sys/timeb.h"

uint8_t Ring_Buffer_Storage[1000];
Ring_Buffer_t RB1;

struct XYZ_t
{
    uint64_t x;
    uint64_t y;
    uint64_t z;
};

struct XYZ_t RB_Generic_Storage[1000];
RB_Generic_t RBG1;

void Test();
void RB_Generic_Test();

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
    //Test();
    RB_Generic_Test();
}

void Test()
{
    uint8_t data;

    Ring_Buffer_Init(&RB1, Ring_Buffer_Storage, sizeof(Ring_Buffer_Storage));

    for (uint16_t i = 0; i < 1000; i++)
    {
        Ring_Buffer_Put_Char(&RB1, i);
    }

    while (Ring_Buffer_Get_Char(&RB1, &data))
    {
        printf("%d\n", data);
    }
}

void RB_Generic_Test()
{
    struct XYZ_t data = {1, 0, 0};

    uint64_t time_elapsed = system_current_time_millis();

    printf("elements = %d\n", sizeof(RB_Generic_Storage) / sizeof(RB_Generic_Storage[0]));

    RB_Generic_Init(&RBG1, (uint8_t *)RB_Generic_Storage, sizeof(RB_Generic_Storage[0]), sizeof(RB_Generic_Storage) / sizeof(RB_Generic_Storage[0]));

    for (size_t i = 0; i < sizeof(RB_Generic_Storage) / sizeof(RB_Generic_Storage[0]); i++)
    {
        RB_Generic_Put(&RBG1, &data);
        data.x++;
        data.y += 2;
        data.z += 10;
    }

    while (RB_Generic_Get(&RBG1, &data))
    {
        printf("x%llu y%llu z%llu\n", data.x, data.y, data.z);
    }

    time_elapsed = system_current_time_millis() - time_elapsed;

    printf("elapsed time %llu\n", time_elapsed);
}