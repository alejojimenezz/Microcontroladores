#include <xc.h>
#define _XTAL_FREQ 4000000

// CONFIG
// #pragma config FOSC = INTOSC_EC
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF

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
volatile bit low_voltage = 0;

// Interrupcion
void __interrupt(high_priority) ISR_HIGH(void);
void __interrupt(low_priority) ISR_LOW(void);

static void hlvd_init(void);

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
    ADCON1 = 0X0F;
    TRISD = 0x00;  //Salida  - 7seg
    TRISE = 0x00;  //Salida  - LED
    TRISB = 0xFF;  //Entrada - Botones
    TRISC = 0xFF;
    TRISA = 0x00;
    
    LATA = 0x00;
    LATC = 0x00;
    LATE = 0x00;
    LATD = 0x00;
    
    HLVDEN = 0;
    HLVDCON = 0b00011010;
    HLVDEN = 1;
    __delay_ms(50);
    HLVDIF = 0;
    HLVDIE = 0;
    
    
    TMR0ON = 0;
    T0CON = 0b00000011;
    
    TMR0 = 34286;
    TMR0IP = 0;
    TMR0IF = 0;
    TMR0IE = 1;
    
    TMR0ON = 1;
    
    IPEN = 1;
    
    INTEDG0 = 0;
    INT0IF = 0;
    INT0IE = 1;
    
    INTEDG1 = 0;
    INT1IF = 0;
    INT1IE = 1;
    INT1IP = 0;
    
    PEIE = 1;
    GIEH = 1;
    GIEL = 1;
    
    setRGBColor(0);
    mostrarDisplay(0);
}

// Función de rebote simple
void esperaAntirebote() {
    __delay_ms(150);
}

// Main
void main(void) {
    
    init();
    unsigned char btnIncAnterior = BTN_INC;
    unsigned char btnResetAnterior = BTN_RESET;
    
    while (1) {
        
        __delay_ms(50);
        
        // Parpadeo del LED indicador (1 Hz)
//        unsigned char hb_factor;
//        hb_factor = (low_voltage) ? 5 : 10;
//        
//        contadorHB++;
//        
//        if(contadorHB >= hb_factor) {
//            LED_HEARTBEAT = !LED_HEARTBEAT;
//            contadorHB = 0;
//        }
        
//        if(low_voltage == 1) {
//            if(contadorHB >= 5) {
//                LED_HEARTBEAT = !LED_HEARTBEAT;
//                contadorHB = 0;
//            }
//        } else {
//            if(contadorHB >= 10) {
//                LED_HEARTBEAT = !LED_HEARTBEAT;
//                contadorHB = 0;
//            }
//        }
               
        // Parada de emergencia
//        if (EMERGENCY == 1) {
//            sistema_bloqueado = 1;
//            setRGBColor(7); // Rojo
//        }
//
//        if (sistema_bloqueado) {
//            continue; // Salir del bucle
//        }

        // Pulso de incremento
        if (btnIncAnterior == 0 && BTN_INC == 1) {
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

        // Botón de reinicio
//        if (btnResetAnterior == 0 && BTN_RESET == 1) {
//            contador = 0;
//            color_estado = 1;
//            mostrarDisplay(contador);
//            setRGBColor(color_estado);
//            esperaAntirebote();
//           
//        }
        btnIncAnterior = BTN_INC;
        // btnResetAnterior = BTN_RESET;
    }
    
}

void __interrupt(high_priority) ISR_HIGH(void) {
    if (INT0IF==1) {
        setRGBColor(7);
        while(1);
    }
//    if (HLVDIF==1) { // Circuito debe iniciar > 4V
//        HLVDIF = 0;
//        // HLVDEN = 0;
//        //__delay_ms(10);
//        VDIRMAG = !VDIRMAG;
//        // __delay_ms(10);
//        // HLVDEN = 1;
//        low_voltage = !low_voltage;
//    }
//    if (IRVST==1) { // Circuito debe iniciar > 4V
//        IRVST = 0;
//        VDIRMAG = !VDIRMAG;
//        low_voltage = !low_voltage;
//    }
}

//void __interrupt(low_priority) ISR_LOW(void) {
//    if(TMR0IF == 1) {
//        TMR0IF = 0;
//        
//        if(HLVDIF == 1) {
//            low_voltage = 1;
//            HLVDIF = 0;
//        } else {
//            low_voltage = 0;
//        }
//        
//        if(low_voltage == 1) {
//            TMR0 = 65536 - 10000;
//            LATA0 ^= 1;
//        } else {
//            TMR0 = 3036;
//            LATA0 ^= 1;
//        }
//        // LATA1 ^= 1;
//    }
//    
//}

void __interrupt(low_priority) ISR_LOW(void) {
    if(TMR0IF == 1) {
        TMR0IF = 0;
        
        // Verificar estado de voltaje
        if(HLVDIF == 1) {
            low_voltage = 1;
            HLVDIF = 0;
        } else {
            low_voltage = 0;
        }
        
        // Configurar período según voltaje
        if(low_voltage == 1) {
            TMR0 = 65536 - 12500;
        } else {
            TMR0 = 34286;
        }
        
        LED_HEARTBEAT = !LED_HEARTBEAT;
        
    }
    
    if (INT1IF) {
        INT1IF = 0;
        contador = 0;
        color_estado = 1;
        mostrarDisplay(contador);
        setRGBColor(color_estado);
        esperaAntirebote();
    }
}