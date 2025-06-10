include P18F4550.inc
    
CONFIG FOSC=INTOSC_EC
;CONFIG FOSC=HS_HS
CONFIG WDT=OFF
CONFIG LVP=OFF

aux1 equ 0h
aux2 equ 1h
aux3 equ 2h
 
ORG 0h
  goto Inicio
ORG 8h
  goto ISR
  
Inicio
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

;Display 7 segmentos
;    a b c d e f g
;0 - 1 1 1 1 1 1 0
;1 - 0 1 1 0 0 0 0
;2 - 1 1 0 1 1 0 1
;3 - 1 1 1 1 0 0 1
;4 - 0 1 1 0 0 1 1
;5 - 1 0 1 1 0 1 1
;6 - 1 0 1 1 1 1 1
;7 - 1 1 1 0 0 1 0
;8 - 1 1 1 1 1 1 1
;9 - 1 1 1 1 0 1 1

Negro  ;00000000
  clrf LATD
  call Retardo1s
Magenta  ;00000011
  movf b'11',LATD
  call Retardo1s
Azul  ;00000100
  movf b'100',LATD
  call Retardo1s
Cyan  ;00000110
  bsf LATD,1
  call Retardo1s
Verde  ;00000010
  bcf LATD,2
  call Retardo1s
Amarillo  ;00000101
  movf b'101',LATD
  call Retardo1s
Blanco  ;00000111
  bsf LATD,1
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

ISR
  bcf INTCON,TMR0IF
  movlw 0xB
  movwf TMR0H
  movlw 0xDC
  movwf TMR0L
  btg LATD,0
  retfie
end