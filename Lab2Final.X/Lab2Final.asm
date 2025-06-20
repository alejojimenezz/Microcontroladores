include P18F4550.inc
    
;CONFIG FOSC=INTOSC_EC    ;Fosc=1MHz
CONFIG FOSC=EC_EC
CONFIG WDT=OFF
  
Inicio  ;RD0 Rojo RD1 Azul  RD2 Verde - LED RGB
  clrf TRISD
  
Menu
Negro
  clrf LATD
Azul
  bsf LATD,2
Cyan
  bsf LATD,1
Verde
  bcf LATD,2
Amarillo
  bsf LATD,0
Blanco
  bsf LATD,2
Magenta
  bcf LATD,1
Rojo
  bcf LATD,2
  goto Menu
  
end