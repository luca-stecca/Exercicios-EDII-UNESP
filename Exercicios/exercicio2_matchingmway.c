#include <stdio.h>


void matchingMultiWay(int lista1[], int n1, 
                      int lista2[], int n2, 
                      int lista3[], int n3, 
                      int saida[], int *nSaida) {
    int i = 0; // lista1
    int j = 0; // lista2
    int k = 0; // lista3
    int out = 0;

    while (i < n1 && j < n2 && k < n3) {
        if (lista1[i] == lista2[j] && lista2[j] == lista3[k]) {
            saida[out++] = lista1[i];
            i++;
            j++;
            k++;
        } else {
            int menor = lista1[i];
            if (lista2[j] < menor) menor = lista2[j];
            if (lista3[k] < menor) menor = lista3[k];

            if (lista1[i] == menor) i++;
            if (lista2[j] == menor) j++;
            if (lista3[k] == menor) k++;
        }
    }

    *nSaida = out;
}

int main() {
    int lista1[] = {10, 25, 30, 45, 60};
    int n1 = sizeof(lista1) / sizeof(lista1[0]);

    int lista2[] = {10, 25, 30, 40, 50, 70};
    int n2 = sizeof(lista2) / sizeof(lista2[0]);

    int lista3[] = {1, 8, 10, 18, 25};
    int n3 = sizeof(lista3) / sizeof(lista3[0]);

    int saida[100];
    int nSaida = 0;

    matchingMultiWay(lista1, n1, lista2, n2, lista3, n3, saida, &nSaida);

    printf("Lista 1: ");
    for (int i = 0; i < n1; i++) printf("%d ", lista1[i]);
    printf("\n");

    printf("Lista 2: ");
    for (int i = 0; i < n2; i++) printf("%d ", lista2[i]);
    printf("\n");

    printf("Lista 3: ");
    for (int i = 0; i < n3; i++) printf("%d ", lista3[i]);
    printf("\n");

    printf("Saida: ");
    for (int i = 0; i < nSaida; i++) {
        printf("%d ", saida[i]);
    }
    printf("\n");

    return 0;
}
