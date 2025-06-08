//Código visto en clase el jueves 5 de junio

//Primera sección
#include <xc.h>
//#include <stdio.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
//Segunda sección
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config PBADEN = OFF
#define _XTAL_FREQ 1000000
//Tercera sección
//bit a;     //1 bits
//char b=10;   //8 bits
//int c;    //16 bits
//long d;   //32 bits
/*float e;
double f=3.14; //IEEE24  o IEEE32
char A[4]={4,5,6,7};
int B[2][3]; + - * / & | ^    
char *g;*/
void verde(void);
void amarillo(void);    
void karolg(char, long);
float maluma(long);
void main(void){
  //Rojo RB7 Amarillo RB6 Verde RB5
  unsigned char i=0;
  TRISB=0b00011111;
  IDLEN=0;//POR, BOR, MCLR, WDT
          //INT0, INT1, INT2, RB
  IDLEN=1;//POR, BOR, MCLR, WDT
          //21 INTERRUPCIONES
  SLEEP();
  verde();  
  TRISD0=0;  //bcf TRISD,0
  //TRISD=0b11110000;   //clrf TRISD
  LATB=0;
  LATD0=0;
  for(i=0;i<3;i++){
    LATB=0b01000000;
    __delay_ms(500);
    LATB=0b00000000;
    __delay_ms(500);
  }
  i=0;
  while(1){
    switch(i){
        case 0: LATB=0b00100000;
                __delay_ms(3000);
                i++;
                break;
        case 1: LATB=0b01000000;
                __delay_ms(1000);
                i++;
                break;
        case 2: LATB=0b10000000;
                __delay_ms(2000);
                i++;
                break;
        case 3: LATB=0b11000000;
                __delay_ms(1000);
                i=0;
                break;
        default: i=0;
                break;        
    }
  }    
}
void verde(void){
   
}
void amarillo(void){
   
}