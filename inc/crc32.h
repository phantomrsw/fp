#pragma once



void crcInit();
uint32_t crcCycle(uint32_t crc32, unsigned char * buf, ssize_t cnt);