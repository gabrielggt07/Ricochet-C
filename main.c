#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include "game.h"
#include "score.h"
#include <stdlib.h>
#include <time.h>

static void desenharRanking(ScoreNode *scores) {
    DrawText("--- TOP SCORES ---", LARGURA / 2 - 100, ALTURA / 2, 20, WHITE);

    ScoreNode *atual = scores;
    int y = ALTURA / 2 + 30;
    int i = 1;
    while (atual != NULL && i <= 5) {
        DrawText(TextFormat("%d. %d pontos", i, atual->pontuacao), LARGURA / 2 - 80, y, 18, WHITE);
        atual = atual->next;
        y += 25;
        i++;
    }
    DrawText("Pressione R para jogar novamente", LARGURA / 2 - 160, y + 10, 20, WHITE);
}

int main() {
    InitWindow(LARGURA, ALTURA, "Ricochet-C");
    SetTargetFPS(60);
    srand(time(NULL));

    Ball ball;
    float larguraBase = LARGURA_PADDLE;
    Paddle paddle = { LARGURA / 2 - LARGURA_PADDLE / 2, ALTURA - 40, LARGURA_PADDLE, 15, 6 };
    Bloco blocos[LINHAS][COLUNAS];
    Particula *particulas = NULL;

    int pontos = 0;
    int vidas = VIDAS_INICIAIS;
    int fase = 1;
    bool lancou = false;
    bool ganhou = false;
    bool perdeu = false;
    bool scoreSalvo = false;
    bool pausado = false;

    gerarBlocos(blocos, fase);
    iniciarBola(&ball, &paddle, VEL_BOLA_INICIAL);
    ScoreNode *scores = carregarScores();

    while (!WindowShouldClose()) {
        if (!lancou && !pausado && IsKeyPressed(KEY_SPACE))
            lancou = true;

        if (IsKeyPressed(KEY_P) && !perdeu && !ganhou)
            pausado = !pausado;

        if (!perdeu && !ganhou && !pausado) {
            moverPaddle(&paddle);

            if (lancou) {
                moverBola(&ball, &paddle, blocos, &pontos, &ganhou, &particulas);
                atualizarParticulas(&particulas);
            } else {
                ball.x = paddle.x + paddle.largura / 2;
            }

            if (ball.y + ball.raio >= ALTURA) {
                vidas--;
                if (vidas <= 0) {
                    perdeu = true;
                    salvarScore(pontos);
                    liberarScores(scores);
                    scores = carregarScores();
                } else {
                    lancou = false;
                    paddle.x = LARGURA / 2 - 50;
                    iniciarBola(&ball, &paddle, VEL_BOLA_INICIAL + (fase - 1) * VEL_FASE);
                }
            }
        }
        bool atingiuMeta = !perdeu && !ganhou && pontos >= fase * PONTOS_POR_FASE;
        if (fase < FASES_MAX && (ganhou || atingiuMeta)) {
            fase++;
            if(vidas < VIDAS_MAX) vidas++;
            larguraBase = LARGURA_PADDLE - (fase - 1) * REDUCAO_PADDLE;
            if (larguraBase < LARGURA_PADDLE_MIN) larguraBase = LARGURA_PADDLE_MIN;
            paddle.largura = larguraBase;
            ganhou = false;
            lancou = false;
            gerarBlocos(blocos, fase);
            paddle.x = LARGURA / 2 - paddle.largura / 2;
            iniciarBola(&ball, &paddle, VEL_BOLA_INICIAL + (fase - 1) * VEL_FASE);
            liberarParticulas(&particulas);
        }

        if ((ganhou || perdeu) && IsKeyPressed(KEY_R)) {
            pontos = 0;
            vidas = VIDAS_INICIAIS;
            fase = 1;
            larguraBase = LARGURA_PADDLE;
            paddle.largura = larguraBase;
            lancou = false;
            ganhou = false;
            perdeu = false;
            scoreSalvo = false;
            gerarBlocos(blocos, fase);
            paddle.x = LARGURA / 2 - 50;
            iniciarBola(&ball, &paddle, VEL_BOLA_INICIAL);
            liberarParticulas(&particulas);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (!perdeu && !ganhou) {
            desenharJogo(&ball, &paddle, blocos, pontos, particulas);
            DrawText(TextFormat("Fase: %d", fase), LARGURA / 2 - 40, 10, 20, WHITE);
            DrawText(TextFormat("Vidas: %d", vidas), LARGURA - 120, 10, 20, WHITE);
            if (!lancou)
                DrawText("Pressione ESPACO para lancar", LARGURA / 2 - 150, ALTURA / 2, 20, WHITE);
            if (pausado)
                DrawText("PAUSADO", LARGURA / 2 - 60, ALTURA / 2 - 40, 30, WHITE);
            }

        if (ganhou) {
            if (!scoreSalvo) {
                salvarScore(pontos);
                liberarScores(scores);
                scores = carregarScores();
                scoreSalvo = true;
            }
            DrawText("VOCE GANHOU!", LARGURA / 2 - 100, ALTURA / 2 - 80, 30, WHITE);
            DrawText(TextFormat("Pontos: %d", pontos), LARGURA / 2 - 60, ALTURA / 2 - 40, 20, WHITE);
            desenharRanking(scores);
        }

        if (perdeu) {
            DrawText("GAME OVER", LARGURA / 2 - 80, ALTURA / 2 - 80, 30, WHITE);
            DrawText(TextFormat("Pontos: %d", pontos), LARGURA / 2 - 60, ALTURA / 2 - 40, 20, WHITE);
            desenharRanking(scores);
        }
        EndDrawing();
    }

    liberarParticulas(&particulas);
    liberarScores(scores);
    CloseWindow();
    return 0;
}
