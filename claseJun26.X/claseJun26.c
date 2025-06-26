#include <xc.h>
#define _XTAL_FREQ
#pragma config
#pragma config

void main(void){
    TRISC6=1;
    TRISC7=1;
    TXSTA=0b;
    RCSTA=0b;
    BAUDCON=0b;
    SPBRG=;    
}
