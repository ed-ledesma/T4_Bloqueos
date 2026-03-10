#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define P 5 // Número de procesos
#define R 3 // Número de tipos de recursos
#define ITER 100000 // Número de solicitudes

/* Recursos iniciales disponibles en el sistema*/
int available_init[R] = {20,20,20};

/**
 * Matrices usadas por los algoritmos.
 */
int max[P][R];
int allocation[P][R];
int need[P][R];
int available[R];

/**
 * Función auxiliar para reiniciar el sistema antes 
 * de simular cada algoritmo.
 * 
 */
void reset_state() {

    for(int r=0;r<R;r++)
        available[r] = available_init[r];

    for(int p=0;p<P;p++)
        for(int r=0;r<R;r++) {
            allocation[p][r] = 0;
            max[p][r] = rand()%6 + 1;
            need[p][r] = max[p][r];
        }
}

/**
 * Libera todos los recursos de un proceso.
 * 
 * Simula cuando un proceso termina o libera recursos.
 */
void release_resources(int pid){

    for(int r=0;r<R;r++){
        available[r] += allocation[pid][r];
        need[pid][r] += allocation[pid][r];
        allocation[pid][r] = 0;
    }
}

/**
 * Función auxiliar del Banker's Algorithm
 * 
 * Verifica si el sistema está en un estado seguro.
 * 
 * Simula si todos los procesos pueden terminar en algún ornde
 * sin que el sistema quede bloqueado.
 * 
 */
bool is_safe() {

    int work[R];
    bool finish[P] = {0};

    /* Copia los recursos disponibles */
    for(int r=0;r<R;r++)
        work[r] = available[r];

    int completed = 0;

    while(completed < P) {

        bool progress = false;

        for(int p=0;p<P;p++) {

            if(!finish[p]) {

                bool possible = true;

                /* Revisa si el proceso puede terminar */
                for(int r=0;r<R;r++) {
                    if(need[p][r] > work[r]) {
                        possible = false;
                        break;
                    }
                }

                if(possible) {

                    /* Simula que el proceso termina */
                    for(int r=0;r<R;r++)
                        work[r] += allocation[p][r];

                    finish[p] = true;
                    completed++;
                    progress = true;
                }
            }
        }

        /* Si el sistema se bloquea, es un estado inseguro */
        if(!progress)
            return false;
    }

    return true;
}

/**
 * Solicitud de recursos usando el Banker's Algorithm
 * 
 * Verifica que la solicitud no exceda need ni available
 * Simula la asignación
 * Ejecuta is_safe()
 * Si el estado no es seguro, revierte la asignación
 * 
 */
bool bankers_request(int pid, int request[R]) {

    for(int r=0;r<R;r++)
        if(request[r] > need[pid][r] || request[r] > available[r])
            return false;

    /* Asignación temporal */
    for(int r=0;r<R;r++) {
        available[r] -= request[r];
        allocation[pid][r] += request[r];
        need[pid][r] -= request[r];
    }

    if(is_safe())
        return true;

    /* Rollback si no es seguro */
    for(int r=0;r<R;r++) {
        available[r] += request[r];
        allocation[pid][r] -= request[r];
        need[pid][r] += request[r];
    }

    return false;
}

/**
 * Prevención de deadlock usando la condición
 * de espera circular.
 * 
 * Los recursos se solicitan en orden creciente
 */
bool circular_request(int pid, int request[R]) {

    int last = -1;

    /* Verifica el orden de recursos */
    for(int r=0;r<R;r++) {

        if(request[r] > 0) {

            if(r < last)
                return false;

            last = r;
        }
    }

    /* Verifica disponibilidad */
    for(int r=0;r<R;r++)
        if(request[r] > available[r])
            return false;

    /* Asignación */
    for(int r=0;r<R;r++){
        available[r] -= request[r];
        allocation[pid][r] += request[r];
        need[pid][r] -= request[r];
    }

    return true;
}

/**
 * Ostrich Algorithm
 * 
 * Ignora los deadlocks, simplemente concede
 * recursos si están disponnibles, no hace
 * verificaciones de seguridad ni orden.
 * 
 */
bool ostrich_request(int pid, int request[R]) {

    for(int r=0;r<R;r++)
        if(request[r] > available[r])
            return false;

    for(int r=0;r<R;r++){
        available[r] -= request[r];
        allocation[pid][r] += request[r];
        need[pid][r] -= request[r];
    }

    return true;
}

/**
 * Función auxiliar que genera solicitudes
 * aleatorios de recursos.
 * 
 * Si un proceso ya no necesita cierto recurso,
 * la solicitud para ese recurso es 0.
 * 
 */
void random_request(int pid, int request[R]) {

    for(int r=0;r<R;r++) {

        if(need[pid][r] == 0)
            request[r] = 0;
        else
            request[r] = rand()%2;
    }
}

/**
 * Función main que maneja la lógica de las
 * simulaciones.
 * 
 */
int main() {

    srand(time(NULL));

    int accepted = 0, rejected = 0; // Declara variables
    int request[R];

    clock_t start, end;

    /* BANKER */

    reset_state(); // Reinicia el sistema antes de empezar

    start = clock(); // Empieza el cronometro

    for(int i=0;i<ITER;i++) {

        int pid = rand()%P;

        random_request(pid, request);

        if(bankers_request(pid, request))
            accepted++;
        else
            rejected++;

        /* Ocasionalmente libera recursos */
        if(rand()%8 == 0)
            release_resources(pid);
    }

    end = clock();

    // Imprime resultados
    printf("Banker results\n");
    printf("accepted: %d\n", accepted);
    printf("rejected: %d\n", rejected);
    printf("time: %.4f sec\n\n", (double)(end-start)/CLOCKS_PER_SEC);

    /* CIRCULAR WAIT */

    accepted = rejected = 0; // Reinicia variables
    reset_state(); // Reinicia el sistema antes de empezar

    start = clock(); // Empieza el cronometro

    for(int i=0;i<ITER;i++) {

        int pid = rand()%P;

        random_request(pid, request);

        if(circular_request(pid, request))
            accepted++;
        else
            rejected++;

        if(rand()%8 == 0)
            release_resources(pid);
    }

    end = clock(); // Termina el cronometro

    // Imprime resultados
    printf("Circular wait results\n");
    printf("accepted: %d\n", accepted);
    printf("rejected: %d\n", rejected);
    printf("time: %.4f sec\n\n", (double)(end-start)/CLOCKS_PER_SEC);

    /* OSTRICH */

    accepted = rejected = 0; // Reinicia variables
    reset_state(); // Reinicia el sistema antes de empezar

    start = clock(); // Empieza el cronometro

    for(int i=0;i<ITER;i++) {

        int pid = rand()%P;

        random_request(pid, request);

        if(ostrich_request(pid, request))
            accepted++;
        else
            rejected++;

        if(rand()%8 == 0)
            release_resources(pid);
    }

    end = clock(); // Termina el cronometro

    // Imprime resultados
    printf("Ostrich results\n");
    printf("accepted: %d\n", accepted);
    printf("rejected: %d\n", rejected);
    printf("time: %.4f sec\n", (double)(end-start)/CLOCKS_PER_SEC);
}