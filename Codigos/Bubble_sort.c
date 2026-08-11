#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- CONFIGURACIÓN --- */
#define ARCHIVO_DATOS      "datos.csv"
#define TAM_MAX_LISTA      100001
#define MEDIA_EXPERIMENTOS 30
#define N_INICIO           1000
#define N_FIN              100000
#define N_INCREMENTO       1000

int A[TAM_MAX_LISTA];

void ordenar(int *A, int n, long long *contador) {
    long long local_cont = 0;

    for (int i = 0; i <= n - 1; i++) {
        for (int j = 0; j <= n - 2 - i; j++) {
            //(*contador)++; 
            local_cont++;
            if (A[j] > A[j + 1]) {
                int aux = A[j];
                A[j] = A[j + 1];
                A[j + 1] = aux;
            }
        }
    }
    *contador = local_cont;
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
        ordenar(A, n, &c);
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
            ordenar(A, n, &cont_individual);
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