include P18F4550.inc
CONFIG FOSC=INTOSC_EC    ;Fosc=1MHz
CONFIG WDT=OFF
CONFIG LVP=OFF
    
#define delay_1s d'80'
#define delay_2s d'162'
#define delay_3s d'243'
    
aux1 equ 0h
aux2 equ 1h
aux3 equ 2h

Inicio    ;RB7 Rojo   RB6 Amarillo  RB5 Verde
  clrf TRISD
  bsf TRISE,0
  bsf ADCON1,7
  
  movlw b'11000111'
  movwf T0CON
  
Menu
  call Pausa
Negro
  clrf LATD
  movlw delay_3s
  call Retardo
  call Pausa
Azul
  movlw b'00000100'
  movwf LATD
  movlw delay_1s
  call Retardo
  call Pausa
Cyan
  movlw b'00000110'
  movwf LATD
  movlw delay_2s
  call Retardo
  call Pausa
Verde
  movlw b'00000010'
  movwf LATD
  movlw delay_1s
  call Retardo
  call Pausa
Amarillo
  movlw b'00000011'
  movwf LATD
  movlw delay_2s
  call Retardo
  call Pausa
Blanco
  movlw b'00000111'
  movwf LATD
  movlw delay_3s
  call Retardo
  call Pausa
Magenta
  movlw b'00000101'
  movwf LATD
  movlw delay_2s
  call Retard
  call Pausa
Rojo
  movlw b'00000001'
  movwf LATD
  movlw delay_1s
  call Retardo
  call Pausa
  
  goto Menu
  
Pausa
  btfss PORTE,0
  goto Pausa
  return
  
Retardo
	movwf aux1
AuxRetardo
	decfsz aux1,f
	goto AuxRetardo
	return

  
end


