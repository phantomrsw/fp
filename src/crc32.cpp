
#include <cstdint>
#include "crc32.h"

uint32_t crc_table[256];

void crcInit()
{
    uint32_t t;
    for(int i = 0; i < 256; i++)
    {
        t = i;
        for(int j = 8; j > 0; j--)
        {
            if(t & 1)
                t = (t >> 1) ^ 0xedb88320;
            else
                t >>= 1;
        }
        crc_table[i] = t;
    }
}

uint32_t crcCycle(uint32_t crc32, unsigned char * buf, ssize_t cnt)
{
    crc32 =~ crc32;
    for(ssize_t i = 0; i < cnt; i++)
    {
        crc32 = crc_table[(crc32 ^ buf[i]) & 0xff] ^ (crc32 >> 8);
    }
    crc32 =~ crc32;
    return crc32;
}