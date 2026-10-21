# Ricochet-C

Jogo de quebra-blocos inspirado no Arkanoid, feito em C com a biblioteca Raylib. O jogador move o paddle para rebater a bola e destruir os blocos. Quando a partida termina, o jogo mostra o ranking com as melhores pontuações.

## Integrantes
- Gabriel Gueiros Tabosa
- Arthur Casanova de Azevedo
- Victor Rodrigues Tavares

## Requisitos
- GCC
- Raylib

Para instalar a Raylib no Linux:
```bash
sudo apt update
sudo apt install libraylib-dev
```

## Compilação

Windows (a Raylib precisa estar em `C:/raylib/w64devkit`):
```bash
mingw32-make windows
.\Ricochet-C.exe
```

Linux:
```bash
make linux
./Ricochet-C
```

## Como jogar
- Seta esquerda e seta direita: movem o paddle
- Espaço: lança a bola
- R: reinicia o jogo
- ESC: sai do jogo

## Estrutura do projeto
```
Ricochet-C
├── main.c       - loop principal do jogo
├── game.c       - bola, paddle, blocos, colisão e partículas
├── game.h       - structs e protótipos das funções
├── score.c      - leitura e escrita das pontuações em arquivo
├── score.h      - protótipos das funções de pontuação
├── scores.txt   - top scores (criado pelo jogo ao salvar a primeira pontuação)
└── Makefile     - compilação do projeto
```