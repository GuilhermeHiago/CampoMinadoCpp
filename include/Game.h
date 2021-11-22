#ifndef GAME_H
#define GAME_H

#include "Ponto.h"
#include "Poligono.h"
#include "Celula.h"

#include <iostream>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>

class Game
{
    public:
        Game();
        virtual ~Game();
        void init();
        void draw();

        Poligono GenerateCircle(float cx, float cy, float r, int num_segments);
        void CriaTabuleiro();
        int contaMinasVizinhas(int x, int y);
        void drawFlag(Ponto* posCelula);
        void desenhaMina(Ponto pos);
        void desenhaCelula(Ponto pos);
        void desenhaTexto(const char* t, Ponto pos);
        void desenhaTabuleiro();
        bool verificaFimJogo();
        bool getDerrota();
        bool isEndGame();

        void init(int nCelulasHorizontal, int nCelulasVertical, int nMinas);
        void loop();
        void mouseInput(int button, int state, int x, int y);
        void keyboard ( unsigned char key, int x, int y );
        void arrow_keys ( int key, int x, int y );

    protected:

    private:
        bool fim;
        bool derrota;
        int nMinas;
        int nCelulasVertical;
        int nCelulasHorizontal;
        Celula **tabuleiroCelula;
        void desenhaMargens();
};

#endif // GAME_H
