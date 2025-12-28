#include<xc.h>
#define _XTAL_FREQ 1000000
#pragma config FOSC=INTOSC_EC
//LA frec del oscilador debe ir antes de la librereria del LCD
#include "LibLCDXC8.h"
#pragma config WDT=OFF
#pragma config PBADEN=OFF
#pragma config LVP=OFF

unsigned char Tecla=0;
unsigned char contador = 0;
unsigned char numero = 0;
unsigned int contador_ms = 0;
unsigned char digito = 0;
unsigned char bandera = 0;
unsigned int inactividad_s = 0;
unsigned char backlight_on = 1;
unsigned char sistema_suspendido = 0;
const unsigned char carita[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b10101,
    0b11111,
    0b01110,
    0b01010,
    0b11011
};
char mensaje[] = "Hola Andres ";

unsigned char solicitarTecla(void);
void mostrarNumero(unsigned char valor);
void iniciarConteo(void);
void mostrarRGB(unsigned char dec);
void guardarNumero(void);

void MostrarMensajeBienvenida();
void PreguntarCuentaObjetivo();
void mostrarMensajeCumplido(void);

void EncenderBacklightLCD(void);
void ApagarBacklightLCD(void);    

unsigned char LeerTeclado(void);
void interrupt ISR(void);


void main(void){
    unsigned char i;
    // Configuracion de puertos
    ADCON1 = 0x0F; // Todo digital
    CMCON = 0x07;  // Comparadores apagados
    
    TRISA1 = 0; // LED de 1Hz
    TRISA3 = 0; //Backligh del display
    TRISA4=0; //RS Display
    TRISA5=0; //E Display
    TRISB=0b11110000;
    TRISD=0;    // 7 segmentos: RD0:A    RD1:B   RD2:C   RD3:D  |  LCD: RD4:D4  RD5:D5  RD6:D6  RD7:D7
    TRISE = 0b11111000; // RE0:Rojo RE1:Verde   RE2:Azul
    TRISC1 = 1; // RC1:entrada (pulsador de aumento de numero)
    LATB=0b00000000;
    LATA1 = 0;
    LATA4 = 0;
    LATA3 = 0;
    LATA5 = 0;
    LATE = 0x00;
    LATD = 0x00;
    RBPU=0;
    __delay_ms(100);
    
    
    TMR0=3036;
    T0CON=0b00000001;
    TMR0IF=0;
    TMR0IE=1;
    TMR0ON=1;
    
    
    RBIF=0;
    RBIE=1;
    GIE=1;
    
    __delay_ms(1000);
    LATA3 = 1;
    ConfiguraLCD(4);
    InicializaLCD();
    MostrarMensajeBienvenida();
    PreguntarCuentaObjetivo(); 
    
    
    while(1){
        
        // LED 1Hz
        /*
         * __delay_ms(50); // delay base
        contador_ms += 50;
        if (contador_ms >= 1000) {
            LATAbits.LATA1 = !LATAbits.LATA1; 
            contador_ms = 0;
        }
        */
        
        
        unsigned char valor = solicitarTecla();
        if(valor != 0xFF){
            digito = valor;

            if(numero < 10){
                mostrarNumero(valor); // Para display 7 segmentos

                // Mostrar en LCD
                FijaCursorLCD(2,1);
                if(numero == 0){
                    //EscribeLCD_c(' ');
                    EscribeLCD_n8(digito,1);
                } else {
                    // Mostrar decena y unidad
                    EscribeLCD_n8(numero,1);
                    EscribeLCD_n8(digito,1);
                }
            }
        }
        
        
        
    }
}

void guardarNumero(){
    if((numero == 0) && (bandera == 0)){
        numero = digito;
        if(digito==0){
            bandera=1;
        }

        // Permitir ingresar primer d gito (incluso si es 0)
        mostrarRGB(0);
        digito = 0;

    } else if((numero < 10)){
        mostrarRGB(numero);
        mostrarNumero(digito);
        
        if(bandera==1){
            numero = numero + digito;
            bandera=0;
        }else{
            numero = (numero * 10) + digito;
        }
        
        if(numero > 0 && numero < 60){
            iniciarConteo();
        } else {
            // Mostrar mensaje error: mayor que 59 y 00
            BorraLCD();
            FijaCursorLCD(1,1);
            MensajeLCD_Var("Numero invalido");
            __delay_ms(2000); 
            BorraLCD();
            PreguntarCuentaObjetivo();
            numero = 0;
        }

        digito = 0;

    } else {
        return;
    }
}

void iniciarConteo(void){
    BorraLCD(); 
    while(contador<numero){
        EncenderBacklightLCD();
        inactividad_s = 0;
        mostrarNumero(contador%10);
        mostrarRGB(contador/10);
        
        // Mostrar en LCD:
        FijaCursorLCD(1,1);
        MensajeLCD_Var("Contar: ");
        EscribeLCD_n8(numero, 2);  // Meta

        FijaCursorLCD(2,1);
        MensajeLCD_Var("Faltan: ");
        EscribeLCD_n8(numero - contador, 2); // Faltantes
        
        
        if (PORTCbits.RC1 == 0) {
            __delay_ms(50); //50ms para probar si tuve ruido o no
            if (PORTCbits.RC1 == 0) {
                contador++;
                while(PORTCbits.RC1 == 0); // Esperar que suelte
            }
        }
        
    }
    mostrarMensajeCumplido();
    
}

void borrarDigito(void){
    if((numero/10)!=0){
                    numero=numero/10;
                    mostrarRGB(0);
                    mostrarNumero(numero);
                    
                    // Actualiza LCD
                    FijaCursorLCD(2,1);
                    EscribeLCD_n8(numero, 1);
                    FijaCursorLCD(2,2);
                    EscribeLCD_c(' '); // borra unidad
                }else{
                    numero=0;
                    mostrarRGB(0);
                    mostrarNumero(0);
                    // Mostrar 0 en el LCD y borrar la unidad
                    FijaCursorLCD(2,1);
                    EscribeLCD_n8(0, 1);
                    FijaCursorLCD(2,2);
                    EscribeLCD_c(' ');
                }
}

void mostrarRGB(unsigned char dec) {
    PORTE &= 0b11111000; // Apaga RGB (RE0-RE2)
    switch(dec) {
        case 0: PORTE |= 0b00000101; break; // Magenta
        case 1: PORTE |= 0b00000100; break; // Azul
        case 2: PORTE |= 0b00000110; break; // Cyan
        case 3: PORTE |= 0b00000010; break; // Verde
        case 4: PORTE |= 0b00000011; break; // Amarillo
        case 5: PORTE |= 0b00000111; break; // Blanco
    }
}

void mostrarNumero(unsigned char unidad) {
    LATD &= 0b11110000; // Limpia bits BCD (RD0-RD3)
    LATD |= (unidad & 0x0F); // Carga unidades en display
    //mostrarRGB(decenas); // Muestra color seg n decena
}

void paradaEmergencia(void){
    PORTE &= 0b11111000;
    PORTE |= 0b00000001; // Solo rojo (RE0)
    BorraLCD();
    FijaCursorLCD(1,1);
    MensajeLCD_Var("PARADA DE EMERGENCIA");
    while(1); // Lo bloqueo!
}

unsigned char solicitarTecla(void){ 
    if(Tecla!=0){
        if(backlight_on==0){
            EncenderBacklightLCD();
            inactividad_s = 0;
        }
        //mostrarNumero(Tecla);
        unsigned char entrada = 0xFF;
        switch(Tecla) {
            case 1: entrada=1; break;
            case 2: entrada=2; break;
            case 3: entrada=3; break;
            //case 4: paradaEmergencia(); break;//Parada de energencia
            case 5: entrada=4; break;
            case 6: entrada=5; break; 
            case 7: entrada=6; break; 
            case 8: guardarNumero(); break;//Tecla OK
            case 9: entrada=7; break;
            case 10: entrada=8; break;
            case 11: entrada=9; break;
            //case 12: iniciarConteo(); break;//Tecla END
            case 13: borrarDigito(); break;//Tecla BACK
            case 14: entrada=0; break;
            //case 15: break;//Tecla RESET
            //case 16: break;//Tecla ON/OFF Backligh  
            default: entrada = 0xFF; break; // No mostrar nada para otra posibilidad
            }
        Tecla = 0; // Limpia bandera
        return entrada;
    }
    return 0xFF;
}

unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);
    LATB=0b11111110;
    if(RB4==0) return 1;
    if(RB5==0) return 2;
    if(RB6==0) return 3;
    if(RB7==0) return 4;
    LATB=0b11111101;
    if(RB4==0) return 5;
    if(RB5==0) return 6;
    if(RB6==0) return 7;
    if(RB7==0) return 8;
    LATB=0b11111011;
    if(RB4==0) return 9;
    if(RB5==0) return 10;
    if(RB6==0) return 11;
    if(RB7==0) return 12;
    LATB=0b11110111;
    if(RB4==0) return 13;
    if(RB5==0) return 14;
    if(RB6==0) return 15;
    if(RB7==0) return 16;
}
void interrupt ISR(void){
    if(RBIF==1){
        
        if(sistema_suspendido){
            sistema_suspendido = 0;
            inactividad_s = 0;
            EncenderBacklightLCD();
        }
        
        if(PORTB!=0b11110000){
            Tecla=0;
            LATB=0b11111110;
            if(RB4==0) Tecla=1;
            else if(RB5==0) Tecla=2;
            else if(RB6==0) Tecla=3;
            else if(RB7==0){ //interrupcion parada emergencia
                Tecla=4;
                paradaEmergencia();
            }
            else{
                LATB=0b11111101;
                if(RB4==0) Tecla=5;
                else if(RB5==0) Tecla=6;
                else if(RB6==0) Tecla=7;
                else if(RB7==0) Tecla=8;
                else{
                    LATB=0b11111011;
                    if(RB4==0) Tecla=9;
                    else if(RB5==0) Tecla=10;
                    else if(RB6==0) Tecla=11;
                    else if(RB7==0){
                        Tecla=12;
                        contador=numero;
                    }
                    else{
                        LATB=0b11110111;
                        if(RB4==0) Tecla=13;
                        else if(RB5==0) Tecla=14;
                        else if(RB6==0){ //Reset cotador
                            Tecla=15;
                            contador=0;
                        }
                        else if(RB7==0){
                            Tecla=16;
                            if(backlight_on){
                                ApagarBacklightLCD();
                                backlight_on = 0;
                            }else{
                                EncenderBacklightLCD();
                                inactividad_s=0;
                                backlight_on = 1;
                            }
                        }
                    }
                }
            }
            LATB=0b11110000;
        }
        __delay_ms(100);
        RBIF=0;
    }
    if(TMR0IF==1){
        TMR0IF=0;
        TMR0=3036;
        LATA1=LATA1^1;
        inactividad_s += 1;
        if(inactividad_s >= 10 && backlight_on){
            ApagarBacklightLCD(); 
            backlight_on = 0;
        }
        if(inactividad_s >= 20 && !sistema_suspendido){
            sistema_suspendido = 1; // Marca que est  suspendido
            Sleep(); // Instrucci n que pone el micro en bajo consumo
        }
    }
}


void MostrarMensajeBienvenida() {
    CreaCaracterLCD(0, carita);     // Crear car cter en CGRAM
    ComandoLCD(0x0C);               // Apagar el cursor (comando 0x0C = display ON, cursor OFF, blink OFF)

    BorraLCD();
    FijaCursorLCD(1,1);

    // Mostrar mensaje y s mbolo
    MensajeLCD_Var(mensaje);
    EscribeLCD_c(0);  // Escribe el car cter especial creado

    // Desplazamiento a la derecha (4 pasos)
    for(int i = 0; i < 4; i++){
        __delay_ms(400);
        DesplazaPantallaD();
    }

    // Desplazamiento a la izquierda (4 pasos)
    for(int i = 0; i < 4; i++){
        __delay_ms(400);
        DesplazaPantallaI();
    }

    __delay_ms(4000); // Esperar 5 segundos
    BorraLCD();
}

void PreguntarCuentaObjetivo(){
    FijaCursorLCD(1,1);
    MensajeLCD_Var("Cuenta objetivo?");
}

void mostrarMensajeCumplido(void){
    BorraLCD();
    FijaCursorLCD(1,1);
    MensajeLCD_Var("Meta alcanzada!");
    FijaCursorLCD(2,1);
    EscribeLCD_n8(numero,2);
    MensajeLCD_Var(" piezas.");

    // Esperar OK (Tecla 8)
    Tecla=0;
    while(1){
        if(Tecla == 8){
            Tecla=0;
            break;
        }
    }

    // Reiniciar sistema
    contador = 0;
    numero = 0;
    digito = 0;
    BorraLCD();
    PreguntarCuentaObjetivo(); // Vuelve a solicitar
}

void EncenderBacklightLCD(){
    LATAbits.LATA3 = 1;
    backlight_on=1;
}

void ApagarBacklightLCD(){
    LATAbits.LATA3 = 0;
    backlight_on=0;
}
