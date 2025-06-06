include P18F4550.inc
    
;CONFIG FOSC=INTOSC_EC    ;Fosc=1MHz
CONFIG FOSC=EC_EC
CONFIG WDT=OFF
;CONFIG LVP=OFF
;aux1 equ 0h
;aux2 equ 1h
;aux3 equ 2h      ;RB0 Sensor
  
Inicio  ;RD0 Rojo   RD1 Azul	  RD2you Verde - LED RGB
  ;movlw b'11111000'
  ;movwf TRISD
  clrf TRISD
  ;________________________________
  
Menu
Negro
  clrf LATD
  ;movf LATD,b'00000000'
  ;movlw b'00000000'
  ;movwf LATD
  ;call Retardo20s
Azul
  bsf LATD,2
  ;movf LATD,b'00000100'
  ;movlw b'00000100'
  ;movwf LATD
  ;call Retardo20s
Cyan
  bsf LATD,1
  ;movf LATD,b'00000110'
  ;movlw b'00000110'
  ;movwf LATD
  ;call Retardo20s
Verde
  bcf LATD,2
  ;movf LATD,b'00000010'
  ;movlw b'00000010'
  ;movwf LATD
  ;call Retardo20s
Amarillo
  bsf LATD,0
  ;movf LATD,b'00000011'
  ;movlw b'00000011'
  ;movwf LATD
  ;call Retardo20s
Blanco
  bsf LATD,2
  ;movf LATD,b'00000111'
  ;movlw b'00000111'
  ;movwf LATD
  ;call Retardo20s
Magenta
  bcf LATD,1
  ;movf LATD,b'00000101'
  ;movlw b'00000101'
  ;movwf LATD
  ;call Retardo20s
Rojo
  bcf LATD,2
  ;movf LATD,b'00000001'
  ;movlw b'00000001'
  ;movwf LATD
  ;call Retardo20s
  goto Menu
  
end