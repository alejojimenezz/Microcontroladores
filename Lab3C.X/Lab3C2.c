/*
 * File:   Lab3C2.c
 * Author: ThinkPad
 *
 * Created on 29 de mayo de 2025, 11:38 AM
 */


#include <xc.h>
//#define _XTAL_FREQ 1000000
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config BOR=ON
#pragma config BORV=0
//#pragma config MCLRE=OFF

void IrAHome(void) {
    IrAHome();
}

void main(void) {
    if(a==1 && RA3==0) {
        IrAHome();
        RESET();
    }
    while(1) {
        
    }
}
