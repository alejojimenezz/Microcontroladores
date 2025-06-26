include P18F4550.inc
    
CONFIG FOSC=HS
CONFIG WDT=OFF
CONFIG PBADEN=OFF
    
aux1 equ 0h
aux2 equ 1h
contador equ 2h
 
Tabla7Seg
    addwf   PCL, f       ; Sumar W al contador de programa
    retlw   b'00111111'  ; 0
    retlw   b'00000110'  ; 1
    retlw   b'01011011'  ; 2
    retlw   b'01001111'  ; 3
    retlw   b'01100110'  ; 4
    retlw   b'01101101'  ; 5
    retlw   b'01111101'  ; 6
    retlw   b'00000111'  ; 7
    retlw   b'01111111'  ; 8
    retlw   b'01101111'  ; 9

Inicio
 clrf TRISD
 clrf LATD
 clrf contador
 
Menu
 btfss PORTC,0
 goto Menu
Menu2
 btfsc PORTC,0
 goto Menu2
 
 incf contador, f
 
 movlw .10
 cpfseq contador
 goto Conteo_Normal
 
Reiniciar
 clrf contador
 clrf LATD
 goto Menu
 
Conteo_Normal
 movf contador, w
 call Tabla7Seg
 movwf LATD
 goto Menu
end


