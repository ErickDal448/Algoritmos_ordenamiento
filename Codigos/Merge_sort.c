#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- CONFIGURACIÓN --- */
#define ARCHIVO_DATOS      "Datos/datos.csv"
#define TAM_MAX_LISTA      100001
#define MEDIA_EXPERIMENTOS 30
#define N_INICIO           1000
#define N_FIN              100000
#define N_INCREMENTO       1000

int A[TAM_MAX_LISTA];

void merge(int A[], int izq, int med, int der, long long * contador){
    int i,j,k;
    // determinamos los tamaños de las mitades 
    int mitad_1 = med - izq + 1;
    int mitad_2 = der - med;

    // Creamos los vectores temporales de mitades
    int V_izq[mitad_1], V_der[mitad_2];

    // Ponemos los datos de A en los vectores
    for(i = 0; i < mitad_1; i++) V_izq[i] = A[izq + i];
    for(j = 0; j < mitad_2; j++) V_der[j] = A[med + 1 + j];

    // unimos los vectores
    i = j = 0;
    k = izq;
    while (i < mitad_1 && j < mitad_2)
    {
        (*contador)++;
        if(V_izq[i] <= V_der[j]){
            A[k] = V_izq[i];
            i++;
        }
        else{
            A[k] = V_der[j];
            j++;
        }
        k++;
    }

    // Si quedan elementos en V_izq los agregamos 
    while (i < mitad_1)
    {
        A[k] = V_izq[i];
        i++;
        k++;
    }

    // Si quedan elementos en V_der los agregamos 
    while (j < mitad_2)
    {
        A[k] = V_der[j];
        j++;
        k++;
    }
    
}

// Funcion de merge sort, donde izq representa el indice de la mitad izquierda y der la derecha
void ordenar(int *A, int izq, int der, long long *contador) {
    long long local_cont = 0;
    if(izq < der){
        int med = izq + (der - izq) / 2;
        // ordenamos la primera y la segunda mitad
        ordenar(A, izq, med, contador);
        ordenar(A, med + 1, der, contador);

        merge(A, izq, med, der, contador);
    }
}

void leer(const char *nom, int n) {
    FILE *f = fopen(nom, "r");
    if (!f) return;
    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%d,", &A[i]) != 1) break;
    }
    fclose(f);
}

int main(int argc, char **argv) {
    if (argc == 2) {
        int n = atoi(argv[1]);
        long long c = 0;
        leer(ARCHIVO_DATOS, n);
        ordenar(A, 0, n - 1, &c);
        for (int i = 0; i < n; i++) printf("%d\n", A[i]);
        return 0;
    }

    // Cabecera: n, Exp1...Exp30, Min, Max, Comparaciones, Promedio
    if(N_INICIO == 1000){
        printf("   n   ");
        for (int i = 1; i <= MEDIA_EXPERIMENTOS; i++) printf(", Exp: %d", i);
        printf(", Minimo, Maximo, Comparaciones, Promedio\n");
        fflush(stdout);
    }

    for (int n = N_INICIO; n <= N_FIN; n += N_INCREMENTO) {
        double tiempos[MEDIA_EXPERIMENTOS], suma_t = 0, min = 1e9, max = -1;
        long long comparaciones_n = 0;

        for (int i = 0; i < MEDIA_EXPERIMENTOS; i++) {
            leer(ARCHIVO_DATOS, n);
            long long cont_individual = 0;
            
            double start = clock();
            ordenar(A, 0, n-1, &cont_individual);
            double fin = clock();
            
            double t = (double)(fin - start) / CLOCKS_PER_SEC;
            tiempos[i] = t;
            suma_t += t;
            comparaciones_n = cont_individual; // Se guarda el último (todos son iguales)

            if (t < min) min = t;
            if (t > max) max = t;
        }

        double prom_t = (suma_t - min - max) / (MEDIA_EXPERIMENTOS - 2);

        printf("%d", n);
        for (int i = 0; i < MEDIA_EXPERIMENTOS; i++) printf(",%.8f", tiempos[i]);
        printf(",%.8f,%.8f,%lld,%.8f\n", min, max, comparaciones_n, prom_t);
        fflush(stdout);
    }
    return 0;
}