#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* --- CONFIGURACIÓN --- */
#define ARCHIVO_DATOS      "datos.csv"
#define TAM_MAX_LISTA      100001
#define MEDIA_EXPERIMENTOS 30
#define N_INICIO           1
#define N_FIN              100000
#define N_INCREMENTO       1
#define LIMITE_TIEMPO_SEGUNDOS 60.0
#define semilla 321

int A[TAM_MAX_LISTA];

bool esta_ordenado(int *A, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (A[i] > A[i + 1]) return false;
    }
    return true;
}

void aleatorizar(int *A, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int aux = A[i];
        A[i] = A[j];
        A[j] = aux;
    }
}

void ordenar_permutation(int *A, int n) {
    while (!esta_ordenado(A, n)) {
        aleatorizar(A, n);
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
        leer(ARCHIVO_DATOS, n);
        double start = clock();
        ordenar_permutation(A, n);
        double fin = clock();
        double t = (double)(fin - start) / CLOCKS_PER_SEC;
        for (int i = 0; i < n; i++) printf("%d\n", A[i]);
        printf("tiempo: %.8f\n", t);
        return 0;
    }

    // Cabecera: n, Exp1...Exp30, Min, Max, Comparaciones, Promedio
    if(N_INICIO == 1000 || N_INICIO == 1){
        printf("   n   ");
        for (int i = 1; i <= MEDIA_EXPERIMENTOS; i++) printf(", Exp: %d", i);
        printf(", Minimo, Maximo, Comparaciones, Promedio\n");
        fflush(stdout);
    }

    

    for (int n = N_INICIO; n <= N_FIN; n += N_INCREMENTO) {
        double tiempos[MEDIA_EXPERIMENTOS], suma_t = 0, min = 1e9, max = -1;

        for (int i = 0; i < MEDIA_EXPERIMENTOS; i++) {
            leer(ARCHIVO_DATOS, n);
            srand(semilla);
            double start = clock();
            ordenar_permutation(A, n);
            double fin = clock();
            
            double t = (double)(fin - start) / CLOCKS_PER_SEC;
            tiempos[i] = t;
            suma_t += t;

            if (t < min) min = t;
            if (t > max) max = t;
        }

        double prom_t = (suma_t - min - max) / (MEDIA_EXPERIMENTOS - 2);

        printf("%d", n);
        for (int i = 0; i < MEDIA_EXPERIMENTOS; i++) printf(", %.8f", tiempos[i]);
        printf(", %.8f, %.8f, %.8f\n", min, max, prom_t);
        fflush(stdout);

        // Si el promedio excedió los 60s, detenemos todo.
        if (prom_t >= LIMITE_TIEMPO_SEGUNDOS) {
            fprintf(stderr, "\n -> Limite de %ds alcanzado en n=%d. Finalizando...\n", (int)LIMITE_TIEMPO_SEGUNDOS, n);
            break; 
        }
    }

    return 0;
}