include P18F4550.inc
    
CONFIG FOSC=INTOSC_EC    ;Fosc=1MHz
CONFIG WDT=OFF
CONFIG LVP=OFF
aux1 equ 0h
aux2 equ 1h
aux3 equ 2h      ;RB0 Sensor
 
ORG 0h
  goto Inicio
;ORG 8h
;  goto ISR
  
Inicio  ;RD0 Rojo   RD1 Azul	  RD2you Verde - LED RGB
  movlw b'11111000'
  movwf TRISD
  ;________________________________
  movlw b'00000001'
  movwf T0CON
  movlw 0xB
  movwf TMR0H
  movlw 0xDC
  movwf TMR0L
  bcf INTCON,TMR0IF
  bsf INTCON,TMR0IE
  bsf INTCON,GIE
  bsf INTCON,TMR0ON
  clrf LATB
  
Menu
Negro
  movlw b'00000000'
  movwf LATD
  call Retardo1s
;Azul
;  movlw b'00000100'
;  movwf LATD
;  call Retardo1s
;Cyan
;  movlw b'00000110'
;  movwf LATD
;  call Retardo1s
;Verde
;  movlw b'00000010'
;  movwf LATD
;  call Retardo1s
;Amarillo
;  movlw b'00000101'
;  movwf LATD
;  call Retardo1s
;Blanco
;  movlw b'00000111'
;  movwf LATD
;  call Retardo1s
;Magenta
;  movlw b'00000011'
;  movwf LATD
;  call Retardo1s
Rojo
  movlw b'00000001'
  movwf LATD
  call Retardo1s
  goto Menu
  
Retardo1s
  movlw .254
  movwf aux1
  movlw .155
  movwf aux2
  movlw .2
  movwf aux3
AuxRetardo1s
  decfsz aux1,f
  goto AuxRetardo1s
  movlw .254
  movwf aux1
  decfsz aux2,f
  goto AuxRetardo1s
  movlw .155
  movwf aux2
  decfsz aux3,f
  goto AuxRetardo1s
  return
;ISR
;  bcf INTCON,TMR0IF
;  movlw 0xB
;  movwf TMR0H
;  movlw 0xDC
;  movwf TMR0L
;  btg LATD,0
;  retfie
end