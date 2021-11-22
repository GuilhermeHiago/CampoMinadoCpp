#ifndef MAINMENU_H
#define MAINMENU_H
#include "Ponto.h"

#include<GL/gl.h>
#include<GL/glu.h>

#ifdef WIN32
#include <windows.h>
#include <glut.h>

#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

using namespace std;

enum SelectStates{
    //START= 0,
    CELL_NUMBER = 0,
    MINE_NUMBER
};

class MainMenu
{
    public:
        bool startGame;
        MainMenu();
        virtual ~MainMenu();
        void init();
        void draw();
        int getGridChosed();
        int getMineNumberChosed();

        void loop();
        void mouseInput(int button, int state, int x, int y);
        void keyboard ( unsigned char key, int x, int y );
        void arrow_keys ( int key, int x, int y );

    protected:

    private:
        string options[3] = {"Precione Enter para Começar", "[<] Tamanho da Grade [>]", "[<] Quantidade de Minas [>]"};
        int nCeluas, nMinas;
        SelectStates selectState;
        const float SCALA = 0.25;
        Ponto limitesTela = Ponto(500, 500);
};

#endif // MAINMENU_H
