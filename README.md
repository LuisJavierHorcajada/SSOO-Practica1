# Práctica 1 de SSOO 2021-2022

Este documento pretende ser una guía para la utilización del programa creado para la práctica. Más abajo se indica la instalación.

## Autor

- Luis Javier Horcajada Torres

## Inputs

- De entrada tenemos los archivos estudiantes_p1.text
- El directorio con los pdfs MODELOSEXAMEN

## Estructura general de la práctica

- El proceso manager es el principal y se encarga de crear y terminar al resto de procesos.
- Al principio PA es creado y este crea un directorio por cada DNI en el .text
- Después PB Y PC son creados de forma concurrente.
- PB se encarga de añadir el modelo de examen correcto a la carpeta de cada alumno.
- PC se encarga de crear un archivo indicando la nota mínima que debe obtener, y también notifica a manager la nota media.
- Si en algún momento el usuario pulsa Ctrl + C (SIGINT) todos los procesos son finalizados y se invoca a otro proceso PD.
- PD se encarga de borrar todos los directorios hechos para cada estudiante.

## Compilación

- Para compilar hacer make
- Y para finalizar make test