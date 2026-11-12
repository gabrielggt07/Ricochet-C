#include "score.h"
#include <stdio.h>
#include <stdlib.h>

void salvarScore(int pontuacao) {
    FILE *f = fopen("scores.txt", "a");
    if (f == NULL) return;
    fprintf(f, "%d\n", pontuacao);
    fclose(f);
}

ScoreNode *carregarScores() {
    FILE *f = fopen("scores.txt", "r");
    if (f == NULL) return NULL;

    ScoreNode *head = NULL;
    int pontuacao;

    while (fscanf(f, "%d", &pontuacao) == 1) {
        ScoreNode *novo = (ScoreNode *)malloc(sizeof(ScoreNode));
        if (novo == NULL) break;
    novo->pontuacao = pontuacao;
if (head == NULL || pontuacao > head->pontuacao) {
    novo->next = head;
    head = novo;
} else {
    ScoreNode *atual = head;
    while (atual->next != NULL && atual->next->pontuacao >= pontuacao)
        atual = atual->next;
    novo->next = atual->next;
    atual->next = novo;
}
    }

    fclose(f);
    return head;
}

void liberarScores(ScoreNode *head) {
    ScoreNode *atual = head;
    while (atual != NULL) {
        ScoreNode *prox = atual->next;
        free(atual);
        atual = prox;
    }
}