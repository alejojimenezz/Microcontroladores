#include <xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
void main(void){
    ADCON1=15;
    TRISA=0;
    TRISB=0;
    LATA=0;
    LATD=0;
    __delay_ms(1000);
    ConfiguraLCD(4);
    InicializaLCD();
    while(1){
        
    }
}
