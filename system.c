#include "general.h"
#include "system.h"


void SYSTEMInit(uint8_t freq)
{
    WDTCONbits_t.WDTPS=18;//WDTPS for 256s
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
        default:
        break;
    }
}