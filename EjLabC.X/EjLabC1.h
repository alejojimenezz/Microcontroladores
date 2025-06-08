//Código en lenguaje C para dos interrupciones: alta y baja prioridad:

#include <xc.h>
#define _XTAL_FREQ 1000000
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF
void interrupt ISRH(void);
void interrupt low_priority ISRL(void);
void main(void){    //POR
  TRISB0=1;
  TRISB1=1;
  INTEDG0=1;
  INTEDG1=0;
  INT0IF=0;
  INT0IE=1;
  INT1IF=0;
  INT1IE=1;
  INT1IP=0;
  IPEN=1;
  PEIE=1;
  GIE=1;
  TRISD0=0;
  while(1){
 
  }
}
void interrupt ISRH(void){
  INT0IF=0;
  LATD0=LATD0^1;  
}
void interrupt low_priority ISRL(void){
  INT1IF=0;
  LATD0=1;
  __delay_ms(100);
  LATD0=0;
  __delay_ms(100);
  LATD0=1;
  __delay_ms(100);
  LATD0=0;
  __delay_ms(100);
  LATD0=1;
  __delay_ms(100);
  LATD0=0;
  __delay_ms(100);
  LATD0=1;
  __delay_ms(100);
  LATD0=0;
  __delay_ms(100);  
}