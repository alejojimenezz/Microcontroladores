//Primera seccion
#include <xc.h>

//Segunda seccion
#pragma config FOSC=HS
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config PBADEN=OFF
#define _XTAL_FREQ 4000000  //Frecuencia de funcionamiento de reloj - Constante

//Tercera secci�n
unsigned char count=0;
unsigned char last_button_state=1;
unsigned char color_index=0;
bit emergency=0;

unsigned int blink_delay = 1000;

const unsigned char numeros[10]={
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};
const unsigned char colores[7][3] = {
    {1,0,0},
    {0,0,1},
    {0,1,1},
    {0,1,0},
    {1,1,0},
    {1,1,1},
    {0,0,0}
};

//Procedimientos
void mostrar_color(unsigned char index);
void beep();
void beeep();
void leerADC();

void main(void) {
    TRISD = 0x00;
    TRISB = 0xFF;
    TRISC = 0xF0;
    TRISA = 0x01;

    LATC = 0;

    while(1){
        unsigned int adc_val=leer_adc();
        blink_delay = (adc_val < 820) ? 250 : 1000;

        // Emergencia
        if(!PORTBbits.RB2){
            emergency=1;
            LATCbits.LATC0=1;
            LATCbits.LATC1=0;
            LATCbits.LATC2=0;
            continue;
        }

        if(emergency) continue;

        LATCbits.LATC3=1;
        __delay_ms(blink_delay/2);
        LATCbits.LATC3=0;
        __delay_ms(blink_delay/2);

        LATD = numeros[count % 10];

        if(count == 10) beep();
        if(count == 60) beeep();

        if(!PORTBbits.RB0 && last_button_state){
            __delay_ms(20);
            if(!PORTBbits.RB0){
                count++;
                if(count % 10 == 0){
                    color_index=(color_index + 1) % 7;
                    mostrar_color(color_index);
                }
            }
        }
        last_button_state=PORTBbits.RB0;

        if(!PORTBbits.RB1){
            __delay_ms(20);
            if(!PORTBbits.RB1){
                count=0;
            }
        }
    }
}

mostrar_color(unsigned char index){
    LATCbits.LATC0=colores[index][0];
    LATCbits.LATC1=colores[index][1];
    LATCbits.LATC2=colores[index][2];
}

beep(){
    LATCbits.LATC4=1;
    __delay_ms(100);
    LATCbits.LATC4=0;
}

beeep(){
    LATCbits.LATC4=1;
    __delay_ms(500);
    LATCbits.LATC4=0;
}

leerADC(){
    ADCON0=0b00000001;
    ADCON0=0b00000001;
    ADCON0=0b00000001;
    __delay_ms(2);
    GO_nDONE=1;
    while(GO_nDONE);
    return((ADRESH<<8) + ADRESL);
}
