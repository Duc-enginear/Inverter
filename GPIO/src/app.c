#include "app.h"
#include "System/Tick.h"
#include "peripheral/wdt/plib_wdt.h"
#include "peripheral/gpio/plib_gpio.h"

static void CallWhileDelay(void) // <editor-fold defaultstate="collapsed" desc="Delay callback function">
{
    WDT_Clear();
} // </editor-fold>

void APP_Initialize(void) // <editor-fold defaultstate="collapsed" desc="APP Initialize">
{
    printf("\nAPP initialize");
    Tick_Init(CallWhileDelay);
} // </editor-fold>

void APP_Tasks(void) // <editor-fold defaultstate="collapsed" desc="APP loop tasks">
{
    IND_RUN_Toggle();
    printf("\nHello");
    __delay_ms(100);
    
} // </editor-fold>
