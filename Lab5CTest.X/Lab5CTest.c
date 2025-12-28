#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 1000000
#pragma config FOSC = INTOSC_EC
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config BOR = OFF

#include "LibLCDXC8.h"

// Pines de entrada
#define ECHO           RC1  // Pin de ECHO del sensor ultrasónico

// Pines de salida
#define TRIGGER        RC0  // Pin de TRIGGER del sensor ultrasónico
#define LED_HEARTBEAT  RA1
#define BUZZER         RA2
#define BACKLIGHT      RA5
#define LED_R          RE0
#define LED_G          RE1
#define LED_B          RE2
#define MOTOR          RC2  // Pin CCP1 para PWM

// Umbral de detección (en cm)
#define UMBRAL_DETECCION 15  // Detecta objetos a menos de 15cm

// Variables
unsigned char contador = 0;
unsigned char numero = 0;
unsigned char digito = 0;
unsigned char color_estado = 0;
unsigned char tecla = 0;
unsigned char motor_detenido = 0;
unsigned char modo_manual = 0;
unsigned char bandera = 0;

bit backlight_on;
bit sistema_suspendido;
bit sistema_bloqueado;
bit conteo_activo;
volatile bit low_voltage;

unsigned int resultado = 0;
unsigned int inactividad_s = 0;
unsigned char distancia_anterior = 255;

char mensaje[] = "Hola Tito ";
const unsigned char cara[8] = {
    0b00000,
    0b01110,
    0b01010,
    0b01110,
    0b00100,
    0b01110,
    0b00100,
    0b01010
};

// Prototipos
void init(void);
void initPWM(void);
void setPWMDutyCycle(unsigned int duty);
void setMotorSpeed(unsigned int adc_value);
void setRGBColor(unsigned char color);
void mostrarDisplay(unsigned char numero);
void buzzCorto();
void buzzLargo();
void esperaAntirebote(void);
unsigned char solicitarTecla(void);
void guardarNumero(void);
void iniciarConteo(void);
void borrarDigito(void);
void paradaEmergencia(void);
void MostrarMensajeBienvenida(void);
void PreguntarCuentaObjetivo(void);
void mostrarMensajeCumplido(void);
void EncenderBacklightLCD(void);
void ApagarBacklightLCD(void);
void ResetearSistema(void);
unsigned int conversion(void);
void transmitir(unsigned char);
unsigned char recibir(void);
void transmitirValorADC(unsigned int valor);
unsigned char LeerTeclado(void);
unsigned char MedirDistancia(void);

// Interrupcion
void __interrupt(high_priority) ISR_HIGH(void);
void __interrupt(low_priority) ISR_LOW(void);

// Función para inicializar PWM en CCP1 (RC2)
void initPWM(void) {
    // Configurar Timer2 para PWM a ~1kHz con Fosc=1MHz
    // PWM freq = Fosc / (4 * (PR2+1) * TMR2_Prescaler)
    // 1kHz = 1MHz / (4 * 250 * 1) = 1kHz
    PR2 = 249;              
    T2CON = 0b00000100;     // TMR2 ON, Prescaler 1:1, Postscaler 1:1
    
    // Configurar CCP1 en modo PWM
    CCP1CON = 0b00001100;   // Modo PWM
    
    // Duty cycle inicial = 0 (motor apagado)
    CCPR1L = 0;
    CCP1CON &= 0xCF;        // Limpiar bits 5:4
    
    // Asegurar que RC2 sea salida
    TRISCbits.TRISC2 = 0;
}

// Función para establecer el duty cycle del PWM (0-1023)
void setPWMDutyCycle(unsigned int duty) {
    if(duty > 1023) duty = 1023;
    
    // Los 8 bits más significativos van a CCPR1L
    CCPR1L = (unsigned char)(duty >> 2);
    
    // Los 2 bits menos significativos van a CCP1CON<5:4>
    CCP1CON &= 0xCF;  // Limpiar bits 5:4
    CCP1CON |= ((duty & 0x03) << 4);
}

// Función para establecer velocidad del motor según ADC (0-1023)
void setMotorSpeed(unsigned int adc_value) {
    if(motor_detenido) {
        setPWMDutyCycle(0);
        return;
    }
    
    if(modo_manual) {
        return;
    }
    
    // Mapeo simple y efectivo
    if(adc_value < 50) {
        // Umbral bajo -> motor apagado
        setPWMDutyCycle(0);
    } else {
        // Mapeo directo proporcional
        // ADC 50-1023 -> PWM 100-1023 (~10%-100%)
        unsigned long duty = ((unsigned long)(adc_value - 50) * 923UL) / 973UL + 100UL;
        if(duty > 1023) duty = 1023;
        setPWMDutyCycle((unsigned int)duty);
    }
}

// Función para medir distancia con sensor ultrasónico
unsigned char MedirDistancia(void) {
    unsigned char aux = 0;
    
    // Deshabilitar PWM temporalmente
    unsigned char ccp1_backup = CCP1CON;
    CCP1CON = 0x00;
    
    // Configurar CCP2 para medición
    CCP2CON = 0b00000100;
    TMR1H = 0;
    TMR1L = 0;
    CCP2IF = 0;
    TMR1IF = 0;
    
    TRIGGER = 1;
    __delay_us(10);
    TRIGGER = 0;
    
    unsigned int timeout = 0;
    while(ECHO == 0 && timeout < 5000) {
        timeout++;
        __delay_us(1);
    }
    
    if(timeout >= 5000) {
        CCP2CON = 0;
        CCP1CON = ccp1_backup;
        return 255;
    }
    
    TMR1ON = 1;
    
    timeout = 0;
    while(CCP2IF == 0 && TMR1IF == 0 && timeout < 30000) {
        timeout++;
    }
    
    TMR1ON = 0;
    
    if(TMR1IF == 1 || timeout >= 30000) {
        aux = 255;
    } else {
        if(CCPR2 >= 3556)
            CCPR2 = 3556;
        aux = CCPR2 / 14;
    }
    
    // Restaurar PWM
    CCP2CON = 0;
    CCP1CON = ccp1_backup;
    
    return aux;
}

void setRGBColor(unsigned char color) {
    switch(color) {
        case 0: LED_R = 0; LED_G = 0; LED_B = 0; break;
        case 1: LED_R = 1; LED_G = 0; LED_B = 1; break;
        case 2: LED_R = 0; LED_G = 0; LED_B = 1; break;
        case 3: LED_R = 0; LED_G = 1; LED_B = 1; break;
        case 4: LED_R = 0; LED_G = 1; LED_B = 0; break;
        case 5: LED_R = 1; LED_G = 1; LED_B = 0; break;
        case 6: LED_R = 1; LED_G = 1; LED_B = 1; break;
        case 7: LED_R = 1; LED_G = 0; LED_B = 0; break;
    }
}

void mostrarDisplay(unsigned char numero) {
    LATD &= 0b11110000;
    LATD |= (numero & 0x0F);
}

void buzzCorto() {
    BUZZER = !BUZZER;
    __delay_ms(100);
    BUZZER = !BUZZER;
}

void buzzLargo() {
    BUZZER = !BUZZER;
    __delay_ms(500);
    BUZZER = !BUZZER;
}

void esperaAntirebote() {
    __delay_ms(150);
}

void init() {
    ADCON0 = 0x01;
    ADCON1 = 0x0E;
    ADCON2 = 0x88;
    
    CMCON = 0x07;
    
    TRISA = 0x01;
    TRISB = 0xF0;
    TRISC = 0xFA;  // RC2 será configurado por PWM
    TRISD = 0x00;
    TRISE = 0x00;
    
    LATA = 0x01;
    LATB = 0xF0;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    
    RBPU = 0;
    
    sistema_bloqueado = 0;
    low_voltage = 0;
    backlight_on = 1;
    sistema_suspendido = 0;
    conteo_activo = 0;
    modo_manual = 0;
    motor_detenido = 0;
    bandera = 0;
    
    HLVDEN = 0;
    HLVDCON = 0b00011010;
    HLVDEN = 1;
    __delay_ms(50);
    HLVDIF = 0;
    HLVDIE = 1;
    HLVDIP = 1;
    
    // Configuración timer 0 - MUY IMPORTANTE: no interferir con Timer2
    TMR0ON = 0;
    T0CON = 0b00000111;  // Prescaler 1:256
    TMR0 = 61536;
    TMR0IP = 0;
    TMR0IF = 0;
    TMR0IE = 1;
    
    // Configuración Timer1 para sensor ultrasónico
    T1CON = 0b10000000;
    TMR1IF = 0;
    TMR1IE = 0;
    
    // Inicializar PWM DESPUÉS de configurar timers
    initPWM();
    
    // UART
    TXSTA = 0b00100100;
    RCSTA = 0b10010000;
    BAUDCON = 0b00001000;
    SPBRG = 25;
    
    RBIF = 0;
    RBIE = 1;
    RBIP = 0;
    
    RCIF = 0;
    RCIE = 1;
    RCIP = 0;
    
    IPEN = 1;
    PEIE = 1;
    GIEH = 1;
    GIEL = 1;
    
    // Iniciar Timer0 al final
    TMR0ON = 1;
    
    __delay_ms(1000);
    BACKLIGHT = 1;
    ConfiguraLCD(4);
    InicializaLCD();
    
    setRGBColor(0);
    mostrarDisplay(0);
    
    setPWMDutyCycle(0);
}

unsigned char LeerTeclado(void) {
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);
    LATB = 0b11111110;
    if(RB4==0) return 1;
    if(RB5==0) return 2;
    if(RB6==0) return 3;
    if(RB7==0) return 4;
    LATB = 0b11111101;
    if(RB4==0) return 5;
    if(RB5==0) return 6;
    if(RB6==0) return 7;
    if(RB7==0) return 8;
    LATB = 0b11111011;
    if(RB4==0) return 9;
    if(RB5==0) return 10;
    if(RB6==0) return 11;
    if(RB7==0) return 12;
    LATB = 0b11110111;
    if(RB4==0) return 13;
    if(RB5==0) return 14;
    if(RB6==0) return 15;
    if(RB7==0) return 16;
    return 0;
}

unsigned char solicitarTecla(void) { 
    if(tecla != 0) {
        if(backlight_on == 0) {
            EncenderBacklightLCD();
            inactividad_s = 0;
        }
        
        unsigned char entrada = 0xFF;
        unsigned char tecla_temp = tecla;
        tecla = 0;
        
        switch(tecla_temp) {
            case 1: entrada = 1; break;
            case 2: entrada = 2; break;
            case 3: entrada = 3; break;
            case 4: paradaEmergencia(); break;
            case 5: entrada = 4; break;
            case 6: entrada = 5; break; 
            case 7: entrada = 6; break; 
            case 8: entrada = 0xFE; break;
            case 9: entrada = 7; break;
            case 10: entrada = 8; break;
            case 11: entrada = 9; break;
            case 12: 
                if(conteo_activo) {
                    contador = numero;
                }
                break;
            case 13: entrada = 0xFD; break;
            case 14: entrada = 0; break;
            case 15: entrada = 0xFC; break;
            case 16: 
                if(backlight_on) {
                    ApagarBacklightLCD();
                    backlight_on = 0;
                } else {
                    EncenderBacklightLCD();
                    inactividad_s = 0;
                    backlight_on = 1;
                }
                break;
            default: entrada = 0xFF; break;
        }
        return entrada;
    }
    return 0xFF;
}

void guardarNumero() {
    if(numero == 0 && digito > 0) {
        numero = digito;
        digito = 0;
        return;
    }
    
    if(numero > 0 && numero <= 59) {
        conteo_activo = 1;
    } else if(numero > 59) {
        BorraLCD();
        FijaCursorLCD(1,1);
        MensajeLCD_Var("Numero invalido");
        __delay_ms(2000); 
        BorraLCD();
        PreguntarCuentaObjetivo();
        numero = 0;
        digito = 0;
    }
}

void iniciarConteo(void) {
    BorraLCD();
    conteo_activo = 1;
    
    distancia_anterior = 255;
    
    while(contador < numero) {
        EncenderBacklightLCD();
        inactividad_s = 0;
        
        mostrarDisplay(contador % 10);
        setRGBColor((contador / 10)+1);
        
        FijaCursorLCD(1,1);
        MensajeLCD_Var("Meta: ");
        EscribeLCD_n8(numero, 2);
        
        FijaCursorLCD(2,1);
        MensajeLCD_Var("Faltan: ");
        EscribeLCD_n8(numero - contador, 2);
        
        unsigned char distancia_actual = MedirDistancia();
        
        if(distancia_anterior > UMBRAL_DETECCION && 
           distancia_actual <= UMBRAL_DETECCION &&
           distancia_actual != 255) {
            contador++;
            buzzCorto();
            esperaAntirebote();
        }
        
        distancia_anterior = distancia_actual;
        
        unsigned char tecla_temp = solicitarTecla();
        if(tecla_temp == 0xFC) {
            break;
        }
        
        __delay_ms(100);
    }
    
    conteo_activo = 0;
}

void borrarDigito(void) {
    if((numero / 10) != 0) {
        numero = numero / 10;
        setRGBColor(0);
        mostrarDisplay(numero);
        
        FijaCursorLCD(2,1);
        EscribeLCD_n8(numero, 1);
        FijaCursorLCD(2,2);
        EscribeLCD_c(' ');
    } else {
        numero = 0;
        setRGBColor(0);
        mostrarDisplay(0);
        
        FijaCursorLCD(2,1);
        EscribeLCD_n8(0, 1);
        FijaCursorLCD(2,2);
        EscribeLCD_c(' ');
    }
}

void paradaEmergencia(void) {
    setRGBColor(7);
    setPWMDutyCycle(0);
    motor_detenido = 1;
    BorraLCD();
    FijaCursorLCD(1,1);
    MensajeLCD_Var("PARADA");
    FijaCursorLCD(2,1);
    MensajeLCD_Var("DE EMERGENCIA");
    buzzLargo();
    __delay_ms(500);
    buzzLargo();
    while(1);
}

void ResetearSistema(void) {
    contador = 0;
    numero = 0;
    digito = 0;
    color_estado = 0;
    motor_detenido = 0;
    conteo_activo = 0;
    bandera = 0;
    distancia_anterior = 255;
    
    mostrarDisplay(0);
    setRGBColor(0);
    
    BorraLCD();
    PreguntarCuentaObjetivo();
}

void MostrarMensajeBienvenida(void) {
    CreaCaracterLCD(0, cara);
    ComandoLCD(0x0C);
    
    BorraLCD();
    FijaCursorLCD(1,1);
    MensajeLCD_Var(mensaje);
    EscribeLCD_c(0);
    
    for(int i = 0; i < 4; i++) {
        __delay_ms(400);
        DesplazaPantallaD();
    }
    
    for(int i = 0; i < 4; i++) {
        __delay_ms(400);
        DesplazaPantallaI();
    }
    
    __delay_ms(2000);
    BorraLCD();
}

void PreguntarCuentaObjetivo(void) {
    FijaCursorLCD(1,1);
    MensajeLCD_Var("Cuenta objetivo?");
}

void mostrarMensajeCumplido(void) {
    BorraLCD();
    FijaCursorLCD(1,1);
    MensajeLCD_Var("Meta alcanzada!");
    FijaCursorLCD(2,1);
    EscribeLCD_n8(numero, 2);
    MensajeLCD_Var(" piezas.");
    
    buzzLargo();
    __delay_ms(300);
    buzzLargo();
    
    tecla = 0;
    while(1) {
        if(tecla == 8) {
            tecla = 0;
            break;
        }
    }
    
    ResetearSistema();
}

void EncenderBacklightLCD(void) {
    BACKLIGHT = 1;
    backlight_on = 1;
}

void ApagarBacklightLCD(void) {
    BACKLIGHT = 0;
    backlight_on = 0;
}

unsigned int conversion(void) {
    ADCON0 = (ADCON0 & 0x01);
    __delay_ms(50);
    GO = 1;
    while(GO);
    return ADRES;
}

void transmitir(unsigned char BufferT){
    while(TRMT == 0);
    TXREG = BufferT;
}

unsigned char recibir(void) {
    while(RCIF == 0);
    return RCREG;
}

void transmitirValorADC(unsigned int valor) {
    char buffer[6];
    sprintf(buffer, "%u\r\n", valor);
    for(char *ptr = buffer; *ptr; ptr++) {
        transmitir(*ptr);
    }
}

void main(void) {
    init();
    
    if (POR) {
        BorraLCD();
        MensajeLCD_Var("Reinicio por");
        DireccionaLCD(0xC0);
        MensajeLCD_Var("RESET");
    }
    else {
        BorraLCD();
        MensajeLCD_Var("Reinicio por");
        DireccionaLCD(0xC0);
        MensajeLCD_Var("MCLR");
    }

    __delay_ms(2000);
    RCON |= 0x1F;
    
    BorraLCD();
    
    MostrarMensajeBienvenida();
    PreguntarCuentaObjetivo();
    
    while (1) {
        __delay_ms(50);
        
        if(conteo_activo) {
            iniciarConteo();
            conteo_activo = 0;
            mostrarMensajeCumplido();
            continue;
        }
        
        unsigned char valor = solicitarTecla();
        
        if(valor == 0xFE) {
            guardarNumero();
        } else if(valor == 0xFD) {
            borrarDigito();
        } else if(valor == 0xFC) {
            ResetearSistema();
        } else if(valor >= 0 && valor <= 9){
            if(numero == 0) {
                numero = valor;
                mostrarDisplay(valor);
                FijaCursorLCD(2,1);
                EscribeLCD_n8(valor, 1);
            } else if(numero < 10 && numero > 0) {
                numero = (numero * 10) + valor;
                mostrarDisplay(valor);
                FijaCursorLCD(2,1);
                EscribeLCD_n8(numero, 2);
                setRGBColor((numero / 10) + 1);
            }
        }
    }
}

void __interrupt(high_priority) ISR_HIGH(void) {
    if(HLVDIF == 1) {
        HLVDIF = 0;
        low_voltage = !low_voltage;
    }
}

void __interrupt(low_priority) ISR_LOW(void) {
    if(TMR0IF == 1) {
        TMR0IF = 0;
        
        // Leer ADC
        resultado = conversion();
        
        // Ajustar velocidad del motor según ADC
        setMotorSpeed(resultado);
        
        // Transmitir después de ajustar motor
        transmitirValorADC(resultado);
        
        if(low_voltage == 1) {
            TMR0 = 64911;
        } else {
            TMR0 = 61536;
        }
        
        LED_HEARTBEAT = !LED_HEARTBEAT;
        
        inactividad_s++;
        
        if(inactividad_s >= 10 && backlight_on) {
            ApagarBacklightLCD();
            backlight_on = 0;
        }
        
        if(inactividad_s >= 20 && !sistema_suspendido) {
            sistema_suspendido = 1;
            Sleep();
        }
    }
    
    if(RCIF) {
        sistema_suspendido = 0;
        inactividad_s = 0;
        EncenderBacklightLCD();
        char comando = RCREG;
        
        if(OERR) {
            CREN = 0;
            CREN = 1;
        }
        
        switch(comando) {
            case 'P': case 'p':
                motor_detenido = 1;
                modo_manual = 0;
                setPWMDutyCycle(0);
                break;
                
            case 'E': case 'e':
                modo_manual = 1;
                motor_detenido = 0;
                setPWMDutyCycle(1023);  // 100% velocidad
                break;
            
            case 'A': case 'a':
                modo_manual = 1;
                motor_detenido = 0;
                setPWMDutyCycle(0);
                break;
                
            case 'R': case 'r':
                if(!conteo_activo) {
                    contador = 0;
                }
                break;
                
            case 'O': case 'o':
                modo_manual = 0;
                motor_detenido = 0;
                break;
                
            default:
                break;
        }
    }
    
    if(RBIF == 1) {
        if(sistema_suspendido) {
            sistema_suspendido = 0;
            inactividad_s = 0;
            EncenderBacklightLCD();
        }
        
        if(PORTB != 0b11110000) {
            tecla = 0;
            LATB = 0b11111110;
            if(RB4 == 0) tecla = 1;
            else if(RB5 == 0) tecla = 2;
            else if(RB6 == 0) tecla = 3;
            else if(RB7 == 0) {
                tecla = 4;
                bandera = 1;
            }
            else {
                LATB = 0b11111101;
                if(RB4 == 0) tecla = 5;
                else if(RB5 == 0) tecla = 6;
                else if(RB6 == 0) tecla = 7;
                else if(RB7 == 0) tecla = 8;
                else {
                    LATB = 0b11111011;
                    if(RB4 == 0) tecla = 9;
                    else if(RB5 == 0) tecla = 10;
                    else if(RB6 == 0) tecla = 11;
                    else if(RB7 == 0) tecla = 12;
                    else {
                        LATB = 0b11110111;
                        if(RB4 == 0) tecla = 13;
                        else if(RB5 == 0) tecla = 14;
                        else if(RB6 == 0) tecla = 15;
                        else if(RB7 == 0) tecla = 16;
                    }
                }
            }
            LATB = 0b11110000;
        }
        __delay_ms(100);
        RBIF = 0;
    }
}