#include <xc.h>
#define _XTAL_FREQ 4000000

// CONFIG
// #pragma config FOSC = INTOSC_EC
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF

// Pines de entrada
#define BTN_INC     RC0
#define BTN_RESET   RB1
#define EMERGENCY   RB0

// Pines de salida
#define LED_HEARTBEAT RA1
#define BUZZER        RA2
#define LED_R         RE0
#define LED_G         RE1
#define LED_B         RE2

// Variables
unsigned char contador = 0;
unsigned char contadorHB = 0;
unsigned char color_estado = 0;
bit sistema_bloqueado = 0;

// Interrupcion
void interrupt ISR(void);

// Leer el ADC (retorna valor 0-1023)
unsigned int ADC_Read(unsigned char canal) {
    ADCON0 = (canal << 2) | 0x01;  // Selecciona canal y enciende ADC
    __delay_us(20);                 // Tiempo de adquisición
    GO_DONE = 1;                    // Inicia conversión
    while(GO_DONE);                 // Espera a que termine
    return ((ADRESH << 8) + ADRESL); // Retorna resultado 10 bits
}

// Convertir lectura ADC a voltaje (en mV)
unsigned int ADC_to_mV(unsigned int valor_adc) {
    // Si Vref = 5V: (valor_adc * 5000) / 1024
    return (unsigned int)((unsigned long)valor_adc * 5000UL / 1024UL);
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

// Función para mostrar un número en el display
void mostrarDisplay(unsigned char numero) {
    LATD &= 0b11110000;
    LATD |= (numero & 0x0F);
}

// Sonido de buzzer corto
void buzzCorto() {
    BUZZER = !BUZZER;
    __delay_ms(100);
    BUZZER = !BUZZER;
}

// Sonido de buzzer largo
void buzzLargo() {
    BUZZER = !BUZZER;
    __delay_ms(500);
    BUZZER = !BUZZER;
}

// Inicialización de puertos
void init() {
    ADCON1 = 0X0E;
    ADCON2 = 0b10101010;
    ADCON0 = 0b00000001;
    
    // RBPU = 0x00;
    TRISD = 0x00;  //Salida  - 7seg
    TRISE = 0x00;  //Salida  - LED
    TRISB = 0xFF;  //Entrada - Botones
    TRISC = 0xFF;
    TRISA = 0b00000001;
    
    LATA = 0x00;
    LATC = 0x00;
    LATE = 0x00;
    LATD = 0x00;
    
    INT0IF = 0;
    INT0IE = 1;
    GIE = 1;
    
    setRGBColor(0);
    mostrarDisplay(0);
}

// Función de rebote simple
void esperaAntirebote() {
    __delay_ms(100);
}

// Main
void main(void) {
    
    init();
    
    unsigned char btnIncAnterior = BTN_INC;
    unsigned char btnResetAnterior = BTN_RESET;
    unsigned int voltaje_mV;
    unsigned char delay_HB = 10;
    
    while (1) {
        
        __delay_ms(50);
        
        // Parpadeo del LED indicador (1 Hz)
        voltaje_mV = ADC_to_mV(ADC_Read(0));
        
        if(voltaje_mV < 4000){
            delay_HB = 5;
        }else{
            delay_HB = 10;
        }

        contadorHB++;
        if(contadorHB >= delay_HB) {
            LED_HEARTBEAT = !LED_HEARTBEAT;
            contadorHB = 0;
        }
        
        // Parada de emergencia
        if (EMERGENCY == 1) {
            sistema_bloqueado = 1;
            setRGBColor(7); // Rojo
        }

        if (sistema_bloqueado) {
            continue; // Salir del bucle
        }

        // Pulso de incremento (detección de flanco descendente)
        if (btnIncAnterior == 1 && BTN_INC == 0) {
            contador++;
            if (color_estado == 0) {
                color_estado = 1;
                setRGBColor(color_estado);
            }
            if (contador > 9) {
                contador = 0;
                color_estado++;
                if (color_estado <= 6) {
                    buzzCorto();
                } else if (color_estado > 6) {
                    color_estado = 1;
                    buzzLargo();
                }
                setRGBColor(color_estado);
            }
            mostrarDisplay(contador);
            esperaAntirebote();
        }
        btnIncAnterior = BTN_INC;

        // Botón de reinicio (flanco descendente)
        if (btnResetAnterior == 1 && BTN_RESET == 0) {
            contador = 0;
            color_estado = 1;
            mostrarDisplay(contador);
            setRGBColor(color_estado);
            esperaAntirebote();
        }
        btnResetAnterior = BTN_RESET;

    }
    
}

void interrupt ISR(void) {
    if (INT0IF==1) {
        PORTE &= 0b11111000;
        PORTE |= 0b00000001;
        while(1);
    }
}