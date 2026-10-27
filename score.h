#ifndef SCORE_H
#define SCORE_H

#include "game.h"

void salvarScore(int pontuacao);
ScoreNode *carregarScores();
void liberarScores(ScoreNode *head);

#endif