<<<<<<< HEAD
#include <stdint.h>
#include <xc.h>

#include "usart.h"

void interrupt ISR(void)
{
    if (RCIE && RCIF) {
        USARTHandleRxInt();
        return;
    }
=======
#include <stdint.h>
#include <xc.h>

#include "usart.h"

interrupt ISR(void)
{
    if (RCIE && RCIF) {
        USARTHandleRxInt();
        return;
    }
>>>>>>> 42e4ede146a92b1bced05e513c6904290aeeaaa9
}