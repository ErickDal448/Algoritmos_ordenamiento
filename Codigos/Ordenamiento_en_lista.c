#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Tamaño_Lista 100001
#define Cantidad_numeros 100000
#define Media_experimentos 30

int A[Tamaño_Lista];

void ordenar(int *A, int n) {
    for (int i = 0; i < n - 1; i++) {
        // Encontrar el indice del maximo en A[0..n-1-i]
        int idx_max = 0;
        for (int j = 1; j <= n - 1 - i; j++) {
            if (A[j] > A[idx_max]) {
                idx_max = j;
            }
        }
        // Intercambiar el maximo con la ultima posicion no ordenada
        int aux = A[n - 1 - i];
        A[n - 1 - i] = A[idx_max];
        A[idx_max] = aux;
    }
}

void Leer_lista(char * Nombre_archivo, int n){
    FILE *Archivo = fopen(Nombre_archivo, "r");
    if (Archivo == NULL) {
        printf("Error al abrir el archivo\n");
    }

    int valor, leidos = 0;
    while (leidos < n && fscanf(Archivo, "%d,", &valor) == 1) {
        A[leidos] = valor;
        leidos++;
    }

    fclose(Archivo);
}

int main(int argc, char ** argv) {
    printf("n,Tiempo(s)\n");
    fflush(stdout);

    for(int n = 1000; n <= Cantidad_numeros; n += 1000){
        if(argv[1] != NULL){
            break;
        }
        double mayor_tiempo = -1;
        double menor_tiempo = 999999;
        double suma_tiempos = 0;
        for(int num_test = 0; num_test < Media_experimentos; num_test++){
            Leer_lista("datos.csv", n);
        
            double start = clock();
            ordenar(A, n);
            double fin = clock();
            double tiempo = (double)(fin - start) / CLOCKS_PER_SEC;
            suma_tiempos += tiempo;

            if(tiempo < menor_tiempo){
                menor_tiempo = tiempo;
            }
            if(tiempo > mayor_tiempo){
                mayor_tiempo = tiempo;
            }
        }
        double promedio = (suma_tiempos - menor_tiempo - mayor_tiempo) / (Media_experimentos - 2);
        printf("%d, %.8f\n", n, promedio);
        fflush(stdout);
    }

    if(argc == 2){
        int n = atoi(argv[1]);
        if(n > Cantidad_numeros){
            printf("Numero de elementos superan la lista de muestra\n");
            return 1;
        }
        Leer_lista("datos.csv", n);
    
        ordenar(A, n);

        for(int j = 0; j < n; j++){
            printf("%d\n", A[j]);
            fflush(stdout);
        }
    }
    return 0;
}
