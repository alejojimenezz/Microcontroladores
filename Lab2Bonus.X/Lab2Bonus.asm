include P18F4550.inc

CONFIG FOSC=EC_EC    ;Fosc=1MHz
CONFIG WDT=OFF

#define time1s .73
#define time2s .148
#define time3s .223

aux1 equ 0h

Inicio    ;
  clrf TRISE
  ;bsf TRISB,0
  movlw 0x0F
  movwf ADCON1
  
Menu
  call Pausa
Negro
  clrf LATE
  movlw time3s
  call Retardo
  call Pausa
Azul
  bsf LATE,2
  movlw time1s
  call Retardo
  call Pausa
Cyan
  bsf LATE,1
  movlw time2s
  call Retardo
  call Pausa
Verde
  bcf LATE,2
  movlw time1s
  call Retardo
  call Pausa
Amarillo
  bsf LATE,0
  movlw time2s
  call Retardo
  call Pausa
Blanco
  bsf LATE,2
  movlw time3s
  call Retardo
  call Pausa
Magenta
  bcf LATE,1
  movlw time2s
  call Retardo
  call Pausa
Rojo
  bcf LATE,2
  movlw time1s
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