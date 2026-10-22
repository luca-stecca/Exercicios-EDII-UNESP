#include <stdio.h>


void imprimirVetorComRodadas(const char *nome, int v[], int n, int tamRodada) {
    printf("%s: ", nome);
    if (n == 0) {
        printf("(vazio)\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        if (i > 0 && i % tamRodada == 0) {
            printf("| ");
        }
        printf("%02d ", v[i]);
    }
    printf("\n");
}

int main() {
    int original[] = {14, 3, 27, 9, 55, 41, 22, 18, 7, 33, 12, 29, 5, 48, 20, 11};
    int n = sizeof(original) / sizeof(original[0]);

    printf("============================================================\n");
    printf("ARQUIVO ORIGINAL (%d elementos):\n", n);
    for (int i = 0; i < n; i++) printf("%02d ", original[i]);
    printf("\n============================================================\n\n");

    int f1[32], f2[32], g1[32], g2[32];
    int n_f1 = 0, n_f2 = 0, n_g1 = 0, n_g2 = 0;

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            f1[n_f1++] = original[i];
        } else {
            f2[n_f2++] = original[i];
        }
    }

    printf("--- DIVISAO INICIAL EM f1 e f2 ---\n");
    imprimirVetorComRodadas("f1", f1, n_f1, 1);
    imprimirVetorComRodadas("f2", f2, n_f2, 1);
    printf("\n");

    int tamRodada = 1;
    int passagem = 1;

    while (tamRodada < n) {
        printf("--- %d PASSAGEM (Tamanho da nova rodada = %d) ---\n", passagem, tamRodada * 2);
        
        n_g1 = 0;
        n_g2 = 0;
        int i1 = 0, i2 = 0;
        int destino = 1; 

        while (i1 < n_f1 || i2 < n_f2) {
            int fim1 = (i1 + tamRodada < n_f1) ? (i1 + tamRodada) : n_f1;
            int fim2 = (i2 + tamRodada < n_f2) ? (i2 + tamRodada) : n_f2;

            int *g = (destino == 1) ? g1 : g2;
            int *n_g = (destino == 1) ? &n_g1 : &n_g2;

            while (i1 < fim1 && i2 < fim2) {
                if (f1[i1] <= f2[i2]) {
                    g[(*n_g)++] = f1[i1++];
                } else {
                    g[(*n_g)++] = f2[i2++];
                }
            }
            while (i1 < fim1) {
                g[(*n_g)++] = f1[i1++];
            }
            while (i2 < fim2) {
                g[(*n_g)++] = f2[i2++];
            }

            destino = (destino == 1) ? 2 : 1;
        }

        imprimirVetorComRodadas("g1", g1, n_g1, tamRodada * 2);
        imprimirVetorComRodadas("g2", g2, n_g2, tamRodada * 2);
        printf("\n");

        n_f1 = n_g1;
        for (int i = 0; i < n_f1; i++) f1[i] = g1[i];

        n_f2 = n_g2;
        for (int i = 0; i < n_f2; i++) f2[i] = g2[i];

        tamRodada *= 2;
        passagem++;
    }

    printf("============================================================\n");
    printf("ARQUIVO FINAL ORDENADO:\n");
    for (int i = 0; i < n_f1; i++) {
        printf("%02d ", f1[i]);
    }
    printf("\n============================================================\n");

    return 0;
}
