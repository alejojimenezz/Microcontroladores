include P18F4550.inc

CONFIG FOSC=EC_EC    ;Fosc=1MHz
CONFIG WDT=OFF
CONFIG LVP=OFF
    
#define delay_1s .80
#define delay_2s .162
#define delay_3s .243
    
aux1 equ 0h
aux2 equ 1h
aux3 equ 2h

Inicio    ;
  clrf TRISE
  bsf TRISB,0
  movlw 0x0F
  movwf ADCON1
  
Menu
  call Pausa
Negro
  clrf LATE
  movlw delay_3s
  call Retardo
  call Pausa
Azul
  bsf LATE,2
  movlw delay_1s
  call Retardo
  call Pausa
Cyan
  bsf LATE,1
  movlw delay_2s
  call Retardo
  call Pausa
Verde
  bcf LATE,2
  movlw delay_1s
  call Retardo
  call Pausa
Amarillo
  bsf LATE,0
  movlw delay_2s
  call Retardo
  call Pausa
Blanco
  bsf LATE,2
  movlw delay_3s
  call Retardo
  call Pausa
Magenta
  bcf LATE,1
  movlw delay_2s
  call Retardo
  call Pausa
Rojo
  bcf LATE,2
  movlw delay_1s
  call Retardo
  call Pausa
  
  goto Menu
  
Pausa
  btfss PORTB,0
  goto Pausa
  return
  
Retardo
  movwf aux1
AuxRetardo
  decfsz aux1,f
  goto AuxRetardo
  return

end