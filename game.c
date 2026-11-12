#include "game.h"
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

static int combo = 0;

void iniciarBola(Ball *ball, Paddle *paddle, float velocidade) {
    combo = 0;
    ball->x = paddle->x + paddle->largura / 2;
    ball->y = paddle->y - 10;
    ball->dx = velocidade;
    ball->dy = -velocidade;
    ball->raio = 8;
}

void moverPaddle(Paddle *paddle) {
    if (IsKeyDown(KEY_LEFT) && paddle->x > 0)
        paddle->x -= paddle->velocidade;
    if (IsKeyDown(KEY_RIGHT) && paddle->x + paddle->largura < LARGURA)
        paddle->x += paddle->velocidade;
}

void gerarBlocos(Bloco blocos[][COLUNAS],int fase) {
        Color coresFileira[5] = {
        {220, 20, 60, 255},
        {30, 144, 255, 255},
        {220, 20, 60, 255},
        {30, 144, 255, 255},
        {220, 20, 60, 255}
    };
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            blocos[i][j].ativo = (rand() % 10) < (7 + fase);
            blocos[i][j].rect = (Rectangle){
                j * (LARGURA_BLOCO + 5) + 15,
                i * (ALTURA_BLOCO + 5) + 50,
                LARGURA_BLOCO,
                ALTURA_BLOCO
            };
            blocos[i][j].cor = coresFileira[i % 5];
        }
    }
}

void adicionarParticula(Particula **head, float x, float y,Color cor) {
    for (int i = 0; i < 10; i++) {
        Particula *new = (Particula *)malloc(sizeof(Particula));
        if (new == NULL) return;
        new->x = x;
        new->y = y;
        new->dx = (float)(rand() % 11 - 5);
        new->dy = (float)(rand() % 11 - 5);
        new->vida = 40;
        new->cor = cor;
        new->next = *head;
        *head = new;
    }
}

void atualizarParticulas(Particula **head) {
    Particula *atual = *head;
    Particula *anterior = NULL;
    while (atual != NULL) {
        atual->x += atual->dx;
        atual->y += atual->dy;
        atual->vida--;

        if (atual->vida <= 0) {
            Particula *morta = atual;
            if (anterior == NULL)
                *head = atual->next;
            else
                anterior->next = atual->next;
            atual = atual->next;
            free(morta);
        } else {
            anterior = atual;
            atual = atual->next;
        }
    }
}

void desenharParticulas(Particula *head) {
    Particula *atual = head;
    while (atual != NULL) {
        DrawCircle(atual->x, atual->y, 5, Fade(atual->cor, atual->vida / 40.0f));
        atual = atual->next;
    }
}

void liberarParticulas(Particula **head) {
    Particula *atual = *head;
    while (atual != NULL) {
        Particula *prox = atual->next;
        free(atual);
        atual = prox;
    }
    *head = NULL;
}

void moverBola(Ball *ball, Paddle *paddle, Bloco blocos[][COLUNAS], int *pontos, bool *ganhou, Particula **particulas) {
    ball->x += ball->dx;
    ball->y += ball->dy;

    if (ball->x - ball->raio <= 0 || ball->x + ball->raio >= LARGURA)
        ball->dx = -ball->dx;

    if (ball->y - ball->raio <= 0)
        ball->dy = -ball->dy;

    Rectangle rectPaddle = { paddle->x, paddle->y, paddle->largura, paddle->altura };
    if (CheckCollisionCircleRec((Vector2){ball->x, ball->y}, ball->raio, rectPaddle) && ball->dy > 0) {
        float velocidade = fabsf(ball->dy);
        float meio = paddle->x + paddle->largura / 2;
        float impacto = (ball->x - meio) / (paddle->largura / 2);
        if (impacto > 1) impacto = 1;
        if (impacto < -1) impacto = -1;

        ball->dx = impacto * velocidade * FATOR_ANGULO;
        ball->dy = -velocidade;
        ball->y = paddle->y - ball->raio;
        combo = 0;
    }
    
    int ativos = 0;
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (blocos[i][j].ativo) {
                ativos++;
                if (CheckCollisionCircleRec((Vector2){ball->x, ball->y}, ball->raio, blocos[i][j].rect)) {
                    blocos[i][j].ativo = false;
                     if (fabsf(ball->dy) < VEL_BOLA_MAX) {
                        ball->dx *= FATOR_ACELERACAO;
                        ball->dy *= FATOR_ACELERACAO;
                    }
                     if (combo < COMBO_MAX) combo++;
                     *pontos += 10 + 5 * (combo - 1);

                    float centroX = blocos[i][j].rect.x + blocos[i][j].rect.width / 2;
                    float centroY = blocos[i][j].rect.y + blocos[i][j].rect.height / 2;

                    float dx = ball->x - centroX;
                    float dy = ball->y - centroY;

                    float overlapX = (blocos[i][j].rect.width / 2 + ball->raio) - fabs(dx);
                    float overlapY = (blocos[i][j].rect.height / 2 + ball->raio) - fabs(dy);

                    if (overlapX < overlapY)
                        ball->dx = -ball->dx;
                    else
                        ball->dy = -ball->dy;

                    adicionarParticula(particulas, centroX, centroY, blocos[i][j].cor);
                    ativos--;
                }
            }
        }
    }

    if (ativos == 0) *ganhou = true;
}

void desenharJogo(Ball *ball, Paddle *paddle, Bloco blocos[][COLUNAS], int pontos, Particula *particulas) {
    DrawRectangle(paddle->x, paddle->y, paddle->largura, paddle->altura, COR_PADDLE);
    DrawCircle(ball->x, ball->y, ball->raio, COR_BOLA);

    for (int i = 0; i < LINHAS; i++)
        for (int j = 0; j < COLUNAS; j++)
            if (blocos[i][j].ativo)
                DrawRectangleRec(blocos[i][j].rect, blocos[i][j].cor);

    desenharParticulas(particulas);
    DrawText(TextFormat("Pontos: %d", pontos), 10, 10, 20, COR_TEXTO);
    if (combo > 1)
        DrawText(TextFormat("Combo x%d", combo), LARGURA - 130, 30, 18, COR_DESTAQUE);
}
