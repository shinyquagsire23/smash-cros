#include <stdint.h>

int dumb_strlen(char *str)
{
    uint32_t len = 0;
    while(1)
    {
        if(str[len] == 0)
            return len;
        len++;
    }
}

void svcOutputDebugString(const char *str, int32_t size)
{
    __asm__("svc 0x3D");
}

void debug_print(char *str)
{
   int len = dumb_strlen(str);
   svcOutputDebugString(str, len);
}
