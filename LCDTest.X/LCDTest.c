#include <xc.h>
#include "LibLCDXC8.h"
#define _XTAL_FREQ 1000000

#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF

void main(void) {
  // Configurar puertos como salida
  TRISA = 0;
  TRISD = 0;
  LATA = 0;
  LATD = 0;
  
  __delay_ms(100);  // Espera inicial
  
  // Inicializar LCD en modo 4 bits
  ConfiguraLCD(4);
  InicializaLCD();
  
  __delay_ms(50);
  
  // Comandos adicionales para asegurar configuración
  ComandoLCD(0x28);  // 4 bits, 2 líneas, 5x8
  __delay_ms(5);
  ComandoLCD(0x0C);  // Display ON, cursor OFF
  __delay_ms(5);
  ComandoLCD(0x06);  // Incrementar cursor
  __delay_ms(5);
  ComandoLCD(0x01);  // Limpiar display
  __delay_ms(5);
  
  // Escribir caracter por caracter
  FijaCursorLCD(1, 1);
  EscribeLCD_c('H');
  __delay_ms(100);
  EscribeLCD_c('o');
  __delay_ms(100);
  EscribeLCD_c('l');
  __delay_ms(100);
  EscribeLCD_c('a');
  __delay_ms(100);
  
  // Probar segunda línea
  FijaCursorLCD(2, 1);
  EscribeLCD_c('T');
  EscribeLCD_c('e');
  EscribeLCD_c('s');
  EscribeLCD_c('t');
  
  while(1) {
    // Loop infinito
  }
}