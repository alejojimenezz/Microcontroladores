#include<xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF

#define RS_DIR TRISAbits.TRISA3
#define E_DIR TRISAbits.TRISA4
#define RS LATAbits.LATA3
#define E LATAbits.LATA4

#define D4_DIR TRISDbits.TRISD4
#define D5_DIR TRISDbits.TRISD5
#define D6_DIR TRISDbits.TRISD6
#define D7_DIR TRISDbits.TRISD7
#define D4 LATDbits.LATD4
#define D5 LATDbits.LATD5
#define D6 LATDbits.LATD6
#define D7 LATDbits.LATD7

void main(void){
    //OSCCON = 0b01110000;
    /*
    TRISD=0;
    TRISA=0;
    LATD=0;
    LATA=0;
    */
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;

    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;

    LATAbits.LATA3 = 0;
    LATAbits.LATA4 = 0;
    LATDbits.LATD4 = 0;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
    LATDbits.LATD7 = 0;
    
    __delay_ms(1500);
    ConfiguraLCD(4);
    InicializaLCD();
    //MensajeLCD_Var("Hola mundo");
    
    EscribeLCD_c('H');
    EscribeLCD_c('o');
    EscribeLCD_c('l');
    EscribeLCD_c('a');
    EscribeLCD_c(' ');
    EscribeLCD_c('m');
    EscribeLCD_c('u');
    EscribeLCD_c('n');
    EscribeLCD_c('d');
    EscribeLCD_c('o');
    
    DireccionaLCD(0xC0);
    EscribeLCD_c('7');
    EscribeLCD_c(';');
    while(1){
    }    
}