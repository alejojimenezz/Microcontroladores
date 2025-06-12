#include <xc.h>

// CONFIG
#pragma config FOSC = HS // Oscilador interno
#pragma config WDTE = OFF            // Watchdog Timer apagado
#pragma config PWRTE = ON            // Power-up Timer encendido
#pragma config MCLRE = ON            // Pin MCLR como reset
#pragma config CP = OFF              // Código sin protección
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF

// Pines de entrada
#define BTN_INC     RB0
#define BTN_RESET   RB1
#define EMERGENCY   RB2

// Pines de salida
#define LED_HEARTBEAT RC0
#define LED_R         RC1
#define LED_G         RC2
#define LED_B         RC3

// Variables
unsigned char contador = 0;
unsigned char color_estado = 0;
bit sistema_bloqueado = 0;

// Tabla del display de 7 segmentos (común cátodo)
const char display_7seg[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Función para mostrar un número en el display
void mostrarDisplay(unsigned char numero) {
    PORTD = display_7seg[numero];
}

// Función para establecer el color del LED RGB
void setRGBColor(unsigned char color) {
    switch(color) {
        case 0: // Negro
            LED_R = 0; LED_G = 0; LED_B = 0; break;
        case 1: // Magenta
            LED_R = 1; LED_G = 0; LED_B = 1; break;
        case 2: // Azul
            LED_R = 0; LED_G = 0; LED_B = 1; break;
        case 3: // Cyan
            LED_R = 0; LED_G = 1; LED_B = 1; break;
        case 4: // Verde
            LED_R = 0; LED_G = 1; LED_B = 0; break;
        case 5: // Amarillo
            LED_R = 1; LED_G = 1; LED_B = 0; break;
        case 6: // Blanco
            LED_R = 1; LED_G = 1; LED_B = 1; break;
        case 7: // Rojo
            LED_R = 1; LED_G = 0; LED_B = 0; break;
    }
}

// Inicialización de puertos
void init() {
    TRISD = 0x00;
    TRISC = 0xF0;
    TRISB = 0xFF;
    PORTD = 0;
    PORTC = 0;
    
    setRGBColor(0);
    mostrarDisplay(0);
}

// Función de rebote simple
void esperaAntirebote() {
    __delay_ms(200);
}

// Main
void main(void) {
    init();
    unsigned char btnIncAnterior = 1;
    unsigned char btnResetAnterior = 1;
    
    while (1) {
        // Parada de emergencia
        if (EMERGENCY == 0) {
            sistema_bloqueado = 1;
            setRGBColor(7); // Rojo
        }

        if (sistema_bloqueado) {
            continue; // Salir del bucle
        }

        // Pulso de incremento (detección de flanco descendente)
        if (btnIncAnterior == 1 && BTN_INC == 0) {
            contador++;
            if (contador > 9) {
                contador = 0;
                color_estado++;
                if (color_estado > 6) color_estado = 1; // Ciclo de colores
                setRGBColor(color_estado);
            }
            mostrarDisplay(contador);
            esperaAntirebote();
        }
        btnIncAnterior = BTN_INC;

        // Botón de reinicio (flanco descendente)
        if (btnResetAnterior == 1 && BTN_RESET == 0) {
            contador = 0;
            mostrarDisplay(contador);
            esperaAntirebote();
        }
        btnResetAnterior = BTN_RESET;

        // Parpadeo del LED indicador (1 Hz)
        LED_HEARTBEAT = 1;
        __delay_ms(500);
        LED_HEARTBEAT = 0;
        __delay_ms(500);
    }
}