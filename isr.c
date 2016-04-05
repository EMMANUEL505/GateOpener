#include <stdint.h>
#include <xc.h>

#include "usart.h"

void interrupt ISR(void)
{
    if (RCIE && RCIF) {
        USARTHandleRxInt();
        return;
    }
}