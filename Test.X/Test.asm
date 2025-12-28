include P18F4550.inc
    
CONFIG FOSC=INTOSC_EC
CONFIG WDT=OFF
  
Inicio  ;RE0 Rojo RE1 Azul  RE2 Verde - LED RGB
  clrf TRISE
  clrf LATE
  
Menu
;Rojo
;  bsf LATE,0
;Verde
;  bsf LATE,1
Azul
  bsf LATE,2
;  goto Menu
  
end