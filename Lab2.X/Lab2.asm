include P18F4550.inc
    
CONFIG FOSC=EC_EC
CONFIG WDT=OFF
  
Inicio  ;RE0 Rojo RE1 Azul  RE2 Verde - LED RGB
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
  nop
Amarillo
  bsf LATE,0
Blanco
  bsf LATE,2
Magenta
  bcf LATE,1
Rojo
  bcf LATE,2
  bz Menu
  
end