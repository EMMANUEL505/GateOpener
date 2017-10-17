#include "general.h"
#include "system.h"
/*WDTPS configurations
00000 = 1:32 (Interval 1 ms typ)
00001 = 1:64 (Interval 2 ms typ)
00010 = 1:128 (Interval 4 ms typ)
00011 = 1:256 (Interval 8 ms typ)
00100 = 1:512 (Interval 16 ms typ)
00101 = 1:1024 (Interval 32 ms typ)
00110 = 1:2048 (Interval 64 ms typ)
00111 = 1:4096 (Interval 128 ms typ)
01000 = 1:8192 (Interval 256 ms typ)
01001 = 1:16384 (Interval 512 ms typ)
01010 = 1:32768 (Interval 1s typ)
01011 = 1:65536 (Interval 2s typ) (Reset value)
01100 = 1:131072 (217) (Interval 4s typ)
01101 = 1:262144 (218) (Interval 8s typ)
01110 = 1:524288 (219) (Interval 16s typ)
01111 = 1:1048576 (220) (Interval 32s typ)
10000 = 1:2097152 (221) (Interval 64s typ)
10001 = 1:4194304 (222) (Interval 128s typ)
10010 = 1:8388608 (223) (Interval 256s typ)*/

void SYSTEMInit(uint8_t freq)
{
    WDTCONbits.WDTPS = 17;
    switch(freq)
    {
        case 4:
            OSCCON=0b01101010;      //Configure Internal Oscillator to 4MHz
        break;
        case 8:
            OSCCON=0b01110010;      //Configure Internal Oscillator to 8MHz
        break;
        case 16:
            OSCCON=0b01111010;      //Configure Internal Oscillator to 16MHz
        break;
        case 32:
            OSCCON=0b01110000;      //Configure Internal Oscillator to 32MHz
        break;        
        default:
        break;
    }
}