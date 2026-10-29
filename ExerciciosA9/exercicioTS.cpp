#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ORDEM 4
#define MAX_CHAVES (ORDEM - 1)
#define MIN_CHAVES ((ORDEM + 1) / 2 - 1)

// PRIMEIRA PARTE DO EX -> APENAS STRUCT E ALOCAÇÃO/INICIALIZAÇÃO
typedef struct NoB {
    int num_chaves;
    char chaves[MAX_CHAVES];
    struct NoB *filhos[ORDEM];
    bool folha;
} NoB;

typedef struct {
    NoB *raiz;
} ArvoreB;

NoB* alocar_no(bool eh_folha) {
    NoB *novo = (NoB*) malloc(sizeof(NoB));
    if (novo == NULL) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }

    novo->num_chaves = 0;
    novo->folha = eh_folha;

    for (int i = 0; i < ORDEM; i++) {
        novo->filhos[i] = NULL;
    }

    return novo;
}

void inicializar_arvore(ArvoreB *arvore) {
    arvore->raiz = NULL;
}

//SEGUNDA PARTE DO EX -> FUNÇÕES 
NoB* buscar_chave(NoB *raiz, char chave, int *indice_retorno) {
    if (raiz == NULL) return NULL;

    int i = 0;
    while (i < raiz->num_chaves && chave > raiz->chaves[i]) {
        i++;
    }

    if (i < raiz->num_chaves && chave == raiz->chaves[i]) {
        if (indice_retorno != NULL) *indice_retorno = i;
        return raiz;
    }

    if (raiz->folha) return NULL;

    return buscar_chave(raiz->filhos[i], chave, indice_retorno);
}

static void dividir_filho(NoB *pai, int indice, NoB *filho) {
    NoB *irmao = alocar_no(filho->folha);
    irmao->num_chaves = 1;

    irmao->chaves[0] = filho->chaves[2];

    if (!filho->folha) {
        irmao->filhos[0] = filho->filhos[2];
        irmao->filhos[1] = filho->filhos[3];
        filho->filhos[2] = NULL;
        filho->filhos[3] = NULL;
    }

    filho->num_chaves = 1;

    for (int j = pai->num_chaves; j >= indice + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->filhos[indice + 1] = irmao;

    for (int j = pai->num_chaves - 1; j >= indice; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }

    pai->chaves[indice] = filho->chaves[1];
    pai->num_chaves++;
}

static void inserir_nao_cheio(NoB *no, char chave) {
    int i = no->num_chaves - 1;

    if (no->folha) {
        while (i >= 0 && no->chaves[i] > chave) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->num_chaves++;
    } else {
        while (i >= 0 && no->chaves[i] > chave) {
            i--;
        }
        i++;

        if (no->filhos[i]->num_chaves == MAX_CHAVES) {
            dividir_filho(no, i, no->filhos[i]);
            if (chave > no->chaves[i]) {
                i++;
            }
        }
        inserir_nao_cheio(no->filhos[i], chave);
    }
}

void inserir(ArvoreB *arvore, char chave) {
    NoB *raiz = arvore->raiz;

    if (raiz == NULL) {
        arvore->raiz = alocar_no(true);
        arvore->raiz->chaves[0] = chave;
        arvore->raiz->num_chaves = 1;
        return;
    }

    if (raiz->num_chaves == MAX_CHAVES) {
        NoB *nova_raiz = alocar_no(false);
        nova_raiz->filhos[0] = raiz;
        dividir_filho(nova_raiz, 0, raiz);

        int i = 0;
        if (chave > nova_raiz->chaves[0]) {
            i++;
        }
        inserir_nao_cheio(nova_raiz->filhos[i], chave);
        arvore->raiz = nova_raiz;
    } else {
        inserir_nao_cheio(raiz, chave);
    }
}

static char obter_predecessor(NoB *no, int idx) {
    NoB *atual = no->filhos[idx];
    while (!atual->folha) atual = atual->filhos[atual->num_chaves];
    return atual->chaves[atual->num_chaves - 1];
}

static char obter_sucessor(NoB *no, int idx) {
    NoB *atual = no->filhos[idx + 1];
    while (!atual->folha) atual = atual->filhos[0];
    return atual->chaves[0];
}

static void fundir(NoB *no, int idx) {
    NoB *filho = no->filhos[idx];
    NoB *irmao = no->filhos[idx + 1];

    filho->chaves[filho->num_chaves] = no->chaves[idx];

    for (int i = 0; i < irmao->num_chaves; i++) {
        filho->chaves[filho->num_chaves + 1 + i] = irmao->chaves[i];
    }

    if (!filho->folha) {
        for (int i = 0; i <= irmao->num_chaves; i++) {
            filho->filhos[filho->num_chaves + 1 + i] = irmao->filhos[i];
        }
    }

    filho->num_chaves += 1 + irmao->num_chaves;

    for (int i = idx; i < no->num_chaves - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }
    no->num_chaves--;

    free(irmao);
}

static void emprestar_anterior(NoB *no, int idx) {
    NoB *filho = no->filhos[idx];
    NoB *irmao = no->filhos[idx - 1];

    for (int i = filho->num_chaves - 1; i >= 0; i--) {
        filho->chaves[i + 1] = filho->chaves[i];
    }

    if (!filho->folha) {
        for (int i = filho->num_chaves; i >= 0; i--) {
            filho->filhos[i + 1] = filho->filhos[i];
        }
        filho->filhos[0] = irmao->filhos[irmao->num_chaves];
    }

    filho->chaves[0] = no->chaves[idx - 1];
    no->chaves[idx - 1] = irmao->chaves[irmao->num_chaves - 1];

    filho->num_chaves++;
    irmao->num_chaves--;
}

static void emprestar_proximo(NoB *no, int idx) {
    NoB *filho = no->filhos[idx];
    NoB *irmao = no->filhos[idx + 1];

    filho->chaves[filho->num_chaves] = no->chaves[idx];

    if (!filho->folha) {
        filho->filhos[filho->num_chaves + 1] = irmao->filhos[0];
    }

    no->chaves[idx] = irmao->chaves[0];

    for (int i = 1; i < irmao->num_chaves; i++) {
        irmao->chaves[i - 1] = irmao->chaves[i];
    }

    if (!irmao->folha) {
        for (int i = 1; i <= irmao->num_chaves; i++) {
            irmao->filhos[i - 1] = irmao->filhos[i];
        }
    }

    filho->num_chaves++;
    irmao->num_chaves--;
}

static void preencher(NoB *no, int idx) {
    if (idx != 0 && no->filhos[idx - 1]->num_chaves > MIN_CHAVES) {
        emprestar_anterior(no, idx);
    } else if (idx != no->num_chaves && no->filhos[idx + 1]->num_chaves > MIN_CHAVES) {
        emprestar_proximo(no, idx);
    } else {
        if (idx != no->num_chaves) {
            fundir(no, idx);
        } else {
            fundir(no, idx - 1);
        }
    }
}

static void remover_no(NoB *no, char chave) {
    int idx = 0;
    while (idx < no->num_chaves && no->chaves[idx] < chave) {
        idx++;
    }

    if (idx < no->num_chaves && no->chaves[idx] == chave) {
        if (no->folha) {
            for (int i = idx + 1; i < no->num_chaves; i++) {
                no->chaves[i - 1] = no->chaves[i];
            }
            no->num_chaves--;
        } else {
            if (no->filhos[idx]->num_chaves >= MIN_CHAVES + 1) {
                char pred = obter_predecessor(no, idx);
                no->chaves[idx] = pred;
                remover_no(no->filhos[idx], pred);
            } else if (no->filhos[idx + 1]->num_chaves >= MIN_CHAVES + 1) {
                char succ = obter_sucessor(no, idx);
                no->chaves[idx] = succ;
                remover_no(no->filhos[idx + 1], succ);
            } else {
                fundir(no, idx);
                remover_no(no->filhos[idx], chave);
            }
        }
    } else {
        if (no->folha) return;

        bool flag = (idx == no->num_chaves);

        if (no->filhos[idx]->num_chaves < MIN_CHAVES + 1) {
            preencher(no, idx);
        }

        if (flag && idx > no->num_chaves) {
            remover_no(no->filhos[idx - 1], chave);
        } else {
            remover_no(no->filhos[idx], chave);
        }
    }
}

void remover(ArvoreB *arvore, char chave) {
    if (arvore->raiz == NULL) return;

    remover_no(arvore->raiz, chave);

    if (arvore->raiz->num_chaves == 0) {
        NoB *antiga_raiz = arvore->raiz;
        if (arvore->raiz->folha) {
            arvore->raiz = NULL;
        } else {
            arvore->raiz = arvore->raiz->filhos[0];
        }
        free(antiga_raiz);
    }
}