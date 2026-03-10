# Simulación de esquemas para prevención de bloqueos

## Equipo 3
1. Ledesma Cuevas Eduardo
2. Mendez Camacho Monserrat

Simulación de esquemas para prevención de bloqueos. Los algoritmos seleccionados con: **Circular-Wait**, **Banker's Algorithm** y **Ostrich Algorithm**.

## Descripción

El simulador permite evalúa

- **Procesos aceptados**
- **Procesos rechazados**
- **Tiempo de ejecución**

A partir de estas medidas, podemos inferir cosas como el Runtime overhead y el System throughput.

Las pruebas utilizan la función `rand()` de la biblioteca `<stdlib.h>` para generar aleatoriamente los procesos.

##  **Circular-Wait** 

Impone un orden total a los recursos, de esta forma, los procesos solo pueden pedir recursos en ese orden y el sistema no necesita analizar los estados del sistema.

##  **Banker's Algorithm** 

Trabaja bajo la idea de evitar que el sistema entre en un estado que podría llevar a deadlock, para hacer esto se comporta como un banco. Antes de prestar recursos revisa si aún podrá satisfacer a todos los clientes eventualmente, dependiendo del resultado, concede o rechaza las solicitudes.

##  **Ostrich Algorithm** 

Consiste en simplemente ignorar el problema, con la ventaja de ser muy simple y sin overhead, pero si el deadlock ocurre, es necesario reiniciar los procesos.

## Resultados

### Comparativa: Circular-Wait vs Banker's Algorithm vs Ostrich Algorithm

| Métrica            | Circular-Wait | Banker's   | Ostrich  | 
|--------------------|---------------|------------|----------|
| Runtime Overhead   |Medio          |Alto        |No tiene  |
| System throughput  |Medio          |Medio-Bajo  |Alto      |
| Tiempo de ejecución|0.0277 sec     |0.0139 sec  |0.0101 sec|
