include P18F4550.inc
    
    CONFIG FOSC=INTOSC_EC
    CONFIG WDT=OFF
    CONFIG PBADEN=OFF
    CONFIG LVP=OFF
    
    Inicio  ;RB7 Rojo	RB6 amarillo	RB5 Verde
	;movlw .31		    ;0b00011111
	movlw b'00011111'
	movwf TRISB
    Menu
    Verde
	movlw b'00100000'
	movwf LATB
    Amarillo
	movlw b'01000000'
	movwf LATB
    Rojo
	movlw b'10000000'
	movwf LATB
    RojoAmarillo
	movlw b'11000000'
	movwf LATB
	goto Menu
    end


