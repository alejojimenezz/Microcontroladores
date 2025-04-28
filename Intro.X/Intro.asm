;Ensamblador
;Código ejemplo para MPLAB X

;Esto es un comentario
;Primera sección

;Inclusión de librerías

include P18F4550.inc

;Segunda seccion 
    
;Configuración (No hay para el lab 1)
		
CONFIG FOSC=INTOSC_EC	;oscilador interno
;CONFIG FOSC=EC_EC
;CONFIG FOSC=XT_XT	;cristal es 3.999 MHz o menos 
;CONFIG FOSC=HS		;cristal es 4MHz o mayor
CONFIG WDT=OFF

;Tercera seccion
	
;Definicion de variables
		
aux1 equ 0h	;variables sin signo, enteras, de 8 bits 
		;0h Primer espacio de memoria
		;0x3FF Ultimo espacio de memoria
aux2 equ 1h
aux3 equ 2h	;Para el lab 1, 4 variables, reusarlas, un solo codigo

;Cuarta seccion
 
;Instrucciones

;Primera clase
;Inicio
 ;aux1 <- 5
; movlw .5   ;w <- 5 (w es un registro temporal)
; movwf aux1 ;aux1 <- w
 ;aux2 <- 0b11001100
; movlw b'11001100'
; movwf aux2
 ;aux3 <- 0xA3
; movlw 0xA3
; movwf aux3
; goto Inicio
;end
 
;Segunda clase
Inicio
 ;bcf - salida
 ;bsf - bit set file - entrada
 bcf TRISD,0 ;Configura si es entrada salida
 ;bcf TRISA,4 ;RA4
 ;bsf TRISE,2 ;RE2
Menu
 bsf LATD,0 ;Enciende LED
 call retardo
 bcf LATD,0 ;Apaga LED
 call retardo
 goto Menu
end
 