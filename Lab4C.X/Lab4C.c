#include <xc.h>
#include "LibLCDXC8.h"
#define _XTAL_FREQ 1000000

#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF
#pragma config LVP=OFF

unsigned char Tecla=0;
unsigned char piezasAContar=0;
unsigned char regresiva=0;

unsigned char LeerTeclado(void);
void interrupt ISR(void);

unsigned char obtenerNumero(unsigned char tecla) {
    switch(tecla) {
        case 1: return 1;
        case 2: return 4;
        case 3: return 7;
      //case 4: return *;
        case 5: return 2;
        case 6: return 5;
        case 7: return 8;
        case 8: return 0;
        case 9: return 3;
        case 10: return 6;
        case 11: return 9;
      //case 12: return #;
      //case 13: return A;
      //case 14: return B;
      //case 15: return C;
      //case 16: return D;
        default: return 255;
    }
}

void main(void){
    unsigned char i;
    TRISB=0b11110000;
    TRISD=0;
    LATB=0b00000000;
    LATD=0;
    RBPU=0;
    __delay_ms(100);
    RBIF=0;
    RBIE=1;
    GIE=1;
    __delay_ms(1000);
    ConfiguraLCD(4);
    InicializaLCD();
    //MensajeLCD_Var("Hola mundo");
    EscribeLCD_c('B');
    EscribeLCD_c('i');
    EscribeLCD_c('e');
    EscribeLCD_c('n');
    EscribeLCD_c('v');
    EscribeLCD_c('e');
    EscribeLCD_c('n');
    EscribeLCD_c('i');
    EscribeLCD_c('d');
    EscribeLCD_c('o');
    EscribeLCD_c('s');
    EscribeLCD_c(' ');
    EscribeLCD_c('c');
    EscribeLCD_c(':');
    
    __delay_ms(5000);
    BorraLCD();
    DireccionaLCD(0x80);
    EscribeLCD_c('P');
    EscribeLCD_c('i');
    EscribeLCD_c('e');
    EscribeLCD_c('z');
    EscribeLCD_c('a');
    EscribeLCD_c('s');
    EscribeLCD_c(' ');
    EscribeLCD_c('a');
    EscribeLCD_c(' ');
    EscribeLCD_c('c');
    EscribeLCD_c('o');
    EscribeLCD_c('n');
    EscribeLCD_c('t');
    EscribeLCD_c('a');
    EscribeLCD_c('r');
    EscribeLCD_c(':');
    EscribeLCD_c(' ');
    
    DireccionaLCD(0xC0);
    while(1){
        /*
         *for(i=0; i<Tecla; i++){
         *    LATD0=1;
         *    __delay_ms(400);
         *    LATD0=0;
         *    __delay_ms(400);
         *}
         *Tecla=0;
         */
        if(Tecla && Tecla != 4 && Tecla <= 11) {  // Teclas 0 a 9
            unsigned char Numero = obtenerNumero(Tecla);
            
            if(Numero != 255) {
                piezasAContar = piezasAContar*10 + Numero;
                EscribeLCD_n8(piezasAContar, 2);
                regresiva = piezasAContar;
            }
            
            if(piezasAContar > 59) {
                piezasAContar = 0;
                BorraLCD();
                DireccionaLCD(0x80);
                EscribeLCD_c('P');
                EscribeLCD_c('i');
                EscribeLCD_c('e');
                EscribeLCD_c('z');
                EscribeLCD_c('a');
                EscribeLCD_c('s');
                EscribeLCD_c(' ');
                EscribeLCD_c('a');
                EscribeLCD_c(' ');
                EscribeLCD_c('c');
                EscribeLCD_c('o');
                EscribeLCD_c('n');
                EscribeLCD_c('t');
                EscribeLCD_c('a');
                EscribeLCD_c('r');
                EscribeLCD_c(':');
                EscribeLCD_c(' ');
            }
        }
        else if(Tecla == 13) { // Tecla A
            BorraLCD();
            DireccionaLCD(0x80);
            EscribeLCD_c('P');
            EscribeLCD_c('i');
            EscribeLCD_c('e');
            EscribeLCD_c('z');
            EscribeLCD_c('a');
            EscribeLCD_c('s');
            EscribeLCD_c(' ');
            EscribeLCD_c('f');
            EscribeLCD_c('a');
            EscribeLCD_c('l');
            EscribeLCD_c('t');
            EscribeLCD_c('a');
            EscribeLCD_c('n');
            EscribeLCD_c('t');
            EscribeLCD_c('e');
            EscribeLCD_c('s');
            EscribeLCD_c(':');
            EscribeLCD_c(' ');
            EscribeLCD_n8(regresiva, 2);
            
            DireccionaLCD(0x94);
            EscribeLCD_c('C');
            EscribeLCD_c('u');
            EscribeLCD_c('e');
            EscribeLCD_c('n');
            EscribeLCD_c('t');
            EscribeLCD_c('a');
            EscribeLCD_c(' ');
            EscribeLCD_c('o');
            EscribeLCD_c('b');
            EscribeLCD_c('j');
            EscribeLCD_c('e');
            EscribeLCD_c('t');
            EscribeLCD_c('i');
            EscribeLCD_c('v');
            EscribeLCD_c('o');
            EscribeLCD_c(':');
            EscribeLCD_c(' ');
            EscribeLCD_n8(piezasAContar, 2);
        }
        else if(Tecla == 4 && regresiva != 0) { // Tecla *
            regresiva--;
            BorraLCD();
            DireccionaLCD(0x80);
            EscribeLCD_c('P');
            EscribeLCD_c('i');
            EscribeLCD_c('e');
            EscribeLCD_c('z');
            EscribeLCD_c('a');
            EscribeLCD_c('s');
            EscribeLCD_c(' ');
            EscribeLCD_c('f');
            EscribeLCD_c('a');
            EscribeLCD_c('l');
            EscribeLCD_c('t');
            EscribeLCD_c('a');
            EscribeLCD_c('n');
            EscribeLCD_c('t');
            EscribeLCD_c('e');
            EscribeLCD_c('s');
            EscribeLCD_c(':');
            EscribeLCD_c(' ');
            EscribeLCD_n8(regresiva, 2);
            
            DireccionaLCD(0x94);
            EscribeLCD_c('C');
            EscribeLCD_c('u');
            EscribeLCD_c('e');
            EscribeLCD_c('n');
            EscribeLCD_c('t');
            EscribeLCD_c('a');
            EscribeLCD_c(' ');
            EscribeLCD_c('o');
            EscribeLCD_c('b');
            EscribeLCD_c('j');
            EscribeLCD_c('e');
            EscribeLCD_c('t');
            EscribeLCD_c('i');
            EscribeLCD_c('v');
            EscribeLCD_c('o');
            EscribeLCD_c(':');
            EscribeLCD_c(' ');
            EscribeLCD_n8(piezasAContar, 2);
        }
        else if(Tecla == 4 && regresiva == 0) {
            BorraLCD();
            DireccionaLCD(0x80);
            EscribeLCD_c('C');
            EscribeLCD_c('u');
            EscribeLCD_c('e');
            EscribeLCD_c('n');
            EscribeLCD_c('t');
            EscribeLCD_c('a');
            EscribeLCD_c(' ');
            EscribeLCD_c('f');
            EscribeLCD_c('i');
            EscribeLCD_c('n');
            EscribeLCD_c('a');
            EscribeLCD_c('l');
            EscribeLCD_c('i');
            EscribeLCD_c('z');
            EscribeLCD_c('a');
            EscribeLCD_c('d');
            EscribeLCD_c('a');
            __delay_ms(5000);
        }
    }    
}

unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);
    LATB=0b11111110;
    if(RB4==0) return 1;
    if(RB5==0) return 2;
    if(RB6==0) return 3;
    if(RB7==0) return 4;
    LATB=0b11111101;
    if(RB4==0) return 5;
    if(RB5==0) return 6;
    if(RB6==0) return 7;
    if(RB7==0) return 8;
    LATB=0b11111011;
    if(RB4==0) return 9;
    if(RB5==0) return 10;
    if(RB6==0) return 11;
    if(RB7==0) return 12;
    LATB=0b11110111;
    if(RB4==0) return 13;
    if(RB5==0) return 14;
    if(RB6==0) return 15;
    if(RB7==0) return 16;
}

void interrupt ISR(void){
    if(RBIF==1){
        if(PORTB!=0b11110000){
            Tecla=0;
            LATB=0b11111110;
            if(RB4==0) Tecla=1;
            else if(RB5==0) Tecla=2;
            else if(RB6==0) Tecla=3;
            else if(RB7==0) Tecla=4;
            else{
                LATB=0b11111101;
                if(RB4==0) Tecla=5;
                else if(RB5==0) Tecla=6;
                else if(RB6==0) Tecla=7;
                else if(RB7==0) Tecla=8;
                else{
                    LATB=0b11111011;
                    if(RB4==0) Tecla=9;
                    else if(RB5==0) Tecla=10;
                    else if(RB6==0) Tecla=11;
                    else if(RB7==0) Tecla=12;
                    else{
                        LATB=0b11110111;
                        if(RB4==0) Tecla=13;
                        else if(RB5==0) Tecla=14;
                        else if(RB6==0) Tecla=15;
                        else if(RB7==0) Tecla=16;
                    }
                }
            }
            LATB=0b11110000;
        }
        __delay_ms(100);
        RBIF=0;
    }
    if(TMR0IF==1){
        TMR0IF=0;
        TMR0=3036;
        LATD1=LATD1^1;
    }
}
