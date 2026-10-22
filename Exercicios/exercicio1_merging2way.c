#include <stdio.h>


void merging2Way(int lista1[], int n1, int lista2[], int n2, int saida[], int *nSaida) {
    int i = 0; // lista1
    int j = 0; // lista2
    int k = 0; // saida


    while (i < n1 && j < n2) {
        if (lista1[i] < lista2[j]) {
            saida[k++] = lista1[i];
            i++; 
        } else if (lista1[i] > lista2[j]) {
            saida[k++] = lista2[j];
            j++; 
        } else {
            saida[k++] = lista1[i];
            i++;
            j++;
        }
    }

    while (i < n1) {
        saida[k++] = lista1[i++];
    }

    while (j < n2) {
        saida[k++] = lista2[j++];
    }

    *nSaida = k;
}

int main() {
    int lista1[] = {10, 25, 30, 45, 60};
    int n1 = sizeof(lista1) / sizeof(lista1[0]);

    int lista2[] = {5, 20, 30, 40, 50, 70};
    int n2 = sizeof(lista2) / sizeof(lista2[0]);

    int saida[100];
    int nSaida = 0;

    merging2Way(lista1, n1, lista2, n2, saida, &nSaida);

    printf("Lista 1: ");
    for (int i = 0; i < n1; i++) printf("%d ", lista1[i]);
    printf("\n");

    printf("Lista 2: ");
    for (int i = 0; i < n2; i++) printf("%d ", lista2[i]);
    printf("\n");

    printf("Saida: ");
    for (int i = 0; i < nSaida; i++) {
        printf("%d ", saida[i]);
    }
    printf("\n");

    return 0;
}
