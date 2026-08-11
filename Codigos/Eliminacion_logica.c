#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define Tamaño_Lista 100001
#define Cantidad_numeros 100000
#define Media_experimentos 30

int s[Tamaño_Lista];

int tomar_menor(int * numeros, int tamaño_lista){
    int menor = INT_MAX; // Determinar el primer elemento como el menor
    int indice_menor = -1;
    for(int i = 0; i < tamaño_lista; i++){ // Recorrer los n elementos de la lista 
        if(numeros[i] == INT_MAX){
            continue;
        }
        if(numeros[i] < menor){ // Determinar si el numero que vemos es menor que el que teniamos como menor
            menor = numeros[i]; // Guardar el menor
            indice_menor = i;
        }
    }

    return indice_menor; // regresar el indice del menor numero
}


int * Leer_lista(char * Nombre_archivo, int n){
    int * Numeros = malloc(n * sizeof(int));
    if (Numeros == NULL) return NULL;
    
    FILE *Archivo = fopen(Nombre_archivo, "r");
    if (Archivo == NULL) {
        free(Numeros); // Liberar si el archivo no abre
        return NULL;
    }
    
    int valor, leidos = 0;
    while (leidos < n && fscanf(Archivo, "%d,", &valor) == 1) {
        Numeros[leidos] = valor;
        leidos++;
    }

    fclose(Archivo);
    return Numeros;
}

int main(int argc, char ** argv){ 
    printf("Número de elementos, Tiempos(seg)\n");
    fflush(stdout);
    
    for(int k = 1000; k <= Cantidad_numeros; k += 1000)
    {
        if(argv[1] != NULL){
            break;
        }
        double mayor_tiempo = -1;
        double menor_tiempo = 999999;
        double suma_tiempos = 0;

        for(int num_test = 0; num_test < Media_experimentos; num_test++){
            
            int *v = Leer_lista("datos.csv", k);
            if (v == NULL) {
                printf("Error al leer archivo para k=%d\n", k);
                continue;
            }
            int i = 0;
            double start= clock();
            while(i < k){
                int idx = tomar_menor(v, k);  // determinamos la posicion del menor de los v
                if(idx != -1) {
                    s[i] = v[idx];   
                    v[idx] = INT_MAX; // Eliminación lógica
                }
                i++;
            }
            double fin = clock();
            double tiempo = (double)(fin - start) / CLOCKS_PER_SEC;
            suma_tiempos += tiempo;

            if(tiempo < menor_tiempo){
                menor_tiempo = tiempo;
            }
            if(tiempo > mayor_tiempo){
                mayor_tiempo = tiempo;
            }
            free(v);
        }
        double promedio = (suma_tiempos - menor_tiempo - mayor_tiempo) / (Media_experimentos - 2);
        printf("%d, %.8f\n", k, promedio);
        fflush(stdout);
    }
    // Caso de testeo con n elementos de la lista
    if (argc == 2)
    {
        printf("Caso testeo\n");
        int k = atoi(argv[1]);
        if(k > Cantidad_numeros){
            printf("Numero de elementos superan la lista de muestra\n");
            return 1;
        }
        int *v = Leer_lista("datos.csv", k);
        if (v == NULL) {
            printf("Error al leer archivo para k=%d\n", k);
            return 1;
        }

        int i = 0;
        double start= clock();
        while(i < k){
            int idx = tomar_menor(v, k);  // determinamos la posicion del menor de los v
            if(idx != -1) {
                s[i] = v[idx];   
                v[idx] = INT_MAX; // Eliminación lógica
            }
            i++;
        }
        double fin = clock();
        double tiempo_total = (double)(fin - start) / CLOCKS_PER_SEC;
        free(v);

        for(int j = 0; j < k; j++){
            printf("%d\n", s[j]);
        }
    }
    
    return 0;
}