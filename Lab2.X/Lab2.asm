include P18F4550.inc
CONFIG FOSC=INTOSC_EC    ;Fosc=1MHz
CONFIG WDT=OFF
CONFIG LVP=OFF
aux1 equ 0h
aux2 equ 1h
aux3 equ 2h      ;RB0 Sensor
ORG 0h
  goto Inicio
ORG 8h
  goto ISR
Inicio    ;RB7 Rojo   RB6 Amarillo  RB5 Verde
  movlw    b'00011111'        ;0b00011111
  movwf TRISB
  bcf TRISD,0
  bcf LATD,0
  movlw b'00000001'
  movwf T0CON
  movlw 0xB
  movwf TMR0H
  movlw 0xDC
  movwf TMR0L
  bcf INTCON,TMR0IF
  bsf INTCON,TMR0IE
  bsf INTCON,GIE
  bsf T0CON,TMR0ON
  clrf LATB
Menu
Verde
  movlw b'00100000'
  movwf LATB
  call RetardoVerde
Amarillo
  movlw b'01000000'
  movwf LATB
  call RetardoVerde
Rojo
  movlw b'10000000'
  movwf LATB
  call RetardoVerde
  call RetardoVerde
RojoAmarillo  
  movlw b'11000000'
  movwf LATB
  call RetardoVerde
  goto Menu
RetardoVerde
  movlw .255
  movwf aux1
  movlw .161
  movwf aux2
  movlw .2
  movwf aux3
AuxRetardoVerde
  decfsz aux1,f
  goto AuxRetardoVerde
  movlw .255
  movwf aux1
  decfsz aux2,f
  goto AuxRetardoVerde
  movlw .161
  movwf aux2
  decfsz aux3,f
  goto AuxRetardoVerde
  return
ISR
  bcf INTCON,TMR0IF
  movlw 0xB
  movwf TMR0H
  movlw 0xDC
  movwf TMR0L
  btg LATD,0
  retfie
end


