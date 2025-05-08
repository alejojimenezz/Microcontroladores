include P18F4550.inc
    
    CONFIG FOSC=INTOSC_EC
    CONFIG WDT=OFF
    CONFIG PBADEN=OFF
    CONFIG LVP=OFF
    
    aux1 equ 0h
    aux2 equ 1h
    aux3 equ 2h
    
    ORG 0h
	goto Inicio
    ORG 8h
	goto ISR
    
    Inicio  ;RB7 Rojo	RB6 amarillo	RB5 Verde
	;movlw .31		    ;0b00011111
	movlw b'00011111'
	movwf TRISB
	bcf INTCON,INT0IF
	bsf INTCON,INT0IE
	bsf INTCON,GIE
	clrf LATB
    Menu
    Verde
	movlw b'00100000'
	movwf LATB
	;call RetardoVerde
	;call RetardoVerde
	;btfsc PORTB,0	;RB0==0?    Entrada
	goto Verde
	
    RetardoVerde
	movlw .254
	movwf aux1
	movlw .160
	movwf aux2
	movlw .2
	movwf aux3
    AuxRetardoVerde
	decfsz aux1,f
	goto AuxRetardoVerde
	movlw .254
	movwf aux1
	decfsz aux2,f
	goto AuxRetardoVerde
	movlw .160
	movwf aux2
	decfsz aux3,f
	goto AuxRetardoVerde
	return
	
    ISR
	bcf INTCON,INT0IF
	call RetardoVerde
	Amarillo
	    movlw b'01000000'
	    movwf LATB
	    call RetardoVerde
	Rojo
	    movlw b'10000000'
	    movwf LATB
	    call RetardoVerde
	RojoAmarillo
	    movlw b'11000000'
	    movwf LATB
	    call RetardoVerde
	    bcf INTCON,INT0IF
	retfie
    end


