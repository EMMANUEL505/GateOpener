#include <stdint.h>
#include <xc.h>

#include "usart.h"

interrupt ISR(void)
{
    if (RCIE && RCIF) {
        USARTHandleRxInt();
        return;
    }
}