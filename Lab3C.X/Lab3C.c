/*
 * File:   Lab3C.c
 * Author: ThinkPad
 *
 * Created on 22 de mayo de 2025, 12:21 PM
 */




//Primera seccion
#include <xc.h>
/*
#include <stdio.h>	//Standard IO
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
*/

//Segunda seccion
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config PBADEN=OFF
#define _XTAL_FREQ 1000000  //Frecuencia de funcionamiento de reloj - Constante

//Tercera sección
/*
bit a;		//1 bit
char b=10;	//8 bits
int c;		//16 bits
long d;		//32 bits
float e;
double f=-0.15;	//IEEE24 o IEEE32 (Buscar como configurar a 32, mas preciso)
char A[4]={4,5,6,7};
int B[2][3];
char *g;    //Apuntador
*/

//Procedimientos
void verde(void);
void amarillo(void);

void main(void) {
    //Ver como inicializar matrices
    //Rojo RB7 Amarillo RB6 Verde RB5
    unsigned char i;
    TRISB=0b00011111;
    TRISD0=0;   //bcf TRISD,0
    //TRISD=0B11110000;//clrf TRISD
    LATB=0;
    LATD0=0;
    for(i=0;i<3;i++){
        LATB=0b01000000;
        __delay_ms(500);
        LATB=0b00000000;
        __delay_ms(500);
    }
    while(1){   //Ciclo infinito
        /*
        LATD0=1;
        __delay_ms(1000);   //Retardo de 1000ms = 1s
        LATD0=0;
        __delay_ms(1000);
        
        
        LATB=0b00100000;
        if(RB0==0){
            __delay_ms(3000);
            LATB=0b01000000;
            __delay_ms(1000);
            LATB=0b10000000;
            __delay_ms(2000);
            LATB=0b11000000;
            __delay_ms(1000);
        }
        */
        
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
