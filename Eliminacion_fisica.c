#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Tamaño_Lista 100001
#define Cantidad_numeros 100000
#define Media_experimentos 30

int s[Tamaño_Lista];

//eliminar elemento por indice
void remover_elemento(int **vector, int *size, int indice){
    for(int i = indice; i < *size - 1; i++){
        (*vector)[i] = (*vector)[i+1];
    }
    (*size)--; 
    *vector = realloc(*vector, (*size)*sizeof(int) );   
}

int* Leer_lista(char *Nombre_archivo, int n) {
    FILE *Archivo = fopen(Nombre_archivo, "r");
    if (!Archivo) return NULL;

    int *Numeros = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (fscanf(Archivo, "%d,", &Numeros[i]) != 1) break;
    }
    fclose(Archivo);
    return Numeros;
}

int main(){
    //printf("Número de elementos, Tiempos(seg)\n");
    //fflush(stdout);

    for(int k=1000; k<=Cantidad_numeros; k+=1000){
        double mayor_tiempo = -1;
        double menor_tiempo = 999999;
        double suma_tiempos = 0;

        for(int num_test = 0; num_test < Media_experimentos; num_test++){

            int size = k;
            int size_s = size;
            int *v = Leer_lista("datos.csv", size);
            
            if (v == NULL) {
                printf("Error al cargar datos para k=%d\n", k);
                continue;
            }

            //encontrar el menor
            double start = clock();
            while(size > 0){
                int indice = 0;
                for(int i = 1; i < size; i++){
                    if( v[i] > v[indice] ){
                        indice = i;
                    }
                }
                s[size - 1] = v[indice];
                remover_elemento(&v, &size, indice);
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
    return 0;
}
