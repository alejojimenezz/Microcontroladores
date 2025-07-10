include P18F4550.inc

CONFIG FOSC = EC_EC
CONFIG WDT = OFF

; =============================
Inicio
    clrf TRISD         ; Puerto D como salida (LEDs)
    bsf TRISE, 0       ; RE0 como entrada
    bsf ADCON1, 7      ; Configura RE0 como entrada digital

Menu
    call Pausa

Negro
    clrf LATD          ; Apaga todos los LEDs
    call Pausa

Azul
    bsf LATD, 2        ; LED Azul
    call Pausa

Cyan
    bsf LATD, 1        ; LED Azul + Verde
    call Pausa

Verde
    bcf LATD, 2        ; Solo Verde
    call Pausa

Amarillo
    bsf LATD, 0        ; Verde + Rojo
    call Pausa

Blanco
    bsf LATD, 2        ; Todos los colores
    call Pausa

Magenta
    bcf LATD, 1        ; Rojo + Azul
    call Pausa

Rojo
    bcf LATD, 2        ; Solo Rojo
    call Pausa

    goto Menu

; =============================
; Subrutina de Pausa
; Detiene el programa mientras RE0 esté en 0 (pausa activada)
Pausa
    btfss PORTE, 0     ; ¿RE0 = 1?
    goto Pausa         ; Si no, sigue en pausa
    return

end