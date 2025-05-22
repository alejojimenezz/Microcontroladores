# Microcontroladores

* [Iniciar proyectos en MPLAB](#proyectos-en-mplab)
* [Guía de GIT](#git-guide)
* [Laboratorios](#laboratorios)

## Proyectos en MPLAB

1. File
2. New Project...
   - Microchip Embedded &#8594; Standalone Project
   - Device: PIC18F4550
   - Tool: Simulator / PICkit3
   - Compiler:
     - mpasm (Ensamblador)
     - XC8: Para trabajar en lenguaje C, distinguir los archivos de proyecto trabajados en C, agregando "C" al final.  
       **NOTA: Para MPLab X v3.30, se requiere XC8 v2.50 o menor** <!--HostID: 8c16456dc0bb-->
   - Project Name = Project File
3. Project Properties
   - mpasm (Global Options) &#8594; Build in absolute mode :white_check_mark:
4. New File
   - Assembler (.asm) o C
        - Para C: Solo puede haber un archivo con "main", en carpeta "Header Files"
   - Mismo nombre del proyecto

## [Git Guide](https://github.com/git-guides)

Para git bash mientras se usa MPLAB:

* `git init`: Inicia repositorio en directorio.
* `git clone`: Clona repositorio desde url ya existente.
* `git add -A`: Agrega todos los archivos modificados a memoria de cambios.
* `git commit -m "Mensaje de commit"`: Commit / Crea memoria lista para subir a repositorio.
* `git push`: Sube el ultimo commit a repositorio.

## Laboratorios

[Carpeta compartida](https://drive.google.com/drive/folders/1e3D2eaxxBDkjEKisnG7sA9y9QmNVdCeo?usp=sharing)

* [**Laboratorio 1**](/Guias/Lab01.pdf)
     - [Presentación](https://docs.google.com/presentation/d/1keYl6i42yLfYA7a7t1YOM0G9IK62Owfi/edit?usp=sharing&ouid=100165221574825375189&rtpof=true&sd=true)
     - [Código](/Lab1.X/Lab1.asm)
 
* [**Laoratorio 2**](/Guias)
* [**Laoratorio 3**](/Guias)
