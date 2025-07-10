include P18F4550.inc
    
;CONFIG FOSC=INTOSC_EC    ;Fosc=1MHz
CONFIG FOSC=EC_EC
CONFIG WDT=OFF
  
Inicio  ;RD0 Rojo RD1 Azul  RD2 Verde - LED RGB
  clrf TRISE
  
Menu
Negro
  clrf LATE
Azul
  bsf LATE,2
Cyan
  bsf LATE,1
Verde
  bcf LATE,2
Amarillo
  bsf LATE,0
Blanco
  bsf LATE,2
Magenta
  bcf LATE,1
Rojo
  bcf LATE,2
  goto Menu
  
end