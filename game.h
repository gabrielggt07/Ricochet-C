#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdbool.h>

#define LARGURA 800
#define ALTURA 600
#define LINHAS 5
#define COLUNAS 10
#define LARGURA_BLOCO 70
#define ALTURA_BLOCO 25

// Rebote no paddle
#define FATOR_ANGULO 1.5f

// Vidas
#define VIDAS_INICIAIS 3

// Fases
#define FASES_MAX 5
#define VEL_BOLA_INICIAL 4.0f
#define VEL_FASE 0.5f
#define PONTOS_POR_FASE 250

// Aceleração da bola
#define VEL_BOLA_MAX 9.0f
#define FATOR_ACELERACAO 1.03f

// Paleta de cores
#define COR_FUNDO    (Color){ 25, 0, 0, 255 }
#define COR_PADDLE   (Color){ 255, 255, 255, 255 }
#define COR_BOLA     (Color){ 255, 255, 255, 255 }
#define COR_TEXTO    (Color){ 255, 255, 255, 255 }
#define COR_DESTAQUE (Color){ 255, 0, 0, 255 }

typedef struct {
    float x, y;
    float dx, dy;
    float raio;
} Ball;

typedef struct {
    float x, y;
    float largura, altura;
    float velocidade;
} Paddle;

typedef struct {
    Rectangle rect;
    bool ativo;
    Color cor;
} Bloco;

typedef struct Particula {
    float x, y;
    float dx, dy;
    int vida;
    Color cor;
    struct Particula *next;
} Particula;

typedef struct node {
    int pontuacao;
    struct node *next;
} ScoreNode;

void iniciarBola(Ball *ball, Paddle *paddle, float velocidade);
void moverBola(Ball *ball, Paddle *paddle, Bloco blocos[][COLUNAS], int *pontos, bool *ganhou, Particula **particulas);
void moverPaddle(Paddle *paddle);
void gerarBlocos(Bloco blocos[][COLUNAS], int fase);
void desenharJogo(Ball *ball, Paddle *paddle, Bloco blocos[][COLUNAS], int pontos, Particula *particulas);

void adicionarParticula(Particula **head, float x, float y, Color cor);
void atualizarParticulas(Particula **head);
void desenharParticulas(Particula *head);
void liberarParticulas(Particula **head);

#endif