// **********************************************************************
//
// Guilherme Hiago Costa dos Santos
//
// Classe que faz o gerenciamento de janelas e de Input de
// teclado e mouse
// **********************************************************************


#include <iostream>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <ctime>
//#include <fstream>
#include <vector>
#include <algorithm>
#define PI 3.14159265


using namespace std;

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

#include "Ponto.h"
#include "Poligono.h"
#include "Celula.h"
#include "MainMenu.h"
#include "Game.h"
#include "EndScreen.h"

#include "Temporizador.h"

Temporizador T;
double AccumDeltaT=0;

// Limites l�gicos da �rea de desenho
Ponto Min, Max;
Ponto Meio, Terco, Largura;


const float SCALA = 1;//0.25;

Ponto limitesTela = Ponto(500, 500);

// **********************************************************************
//    Variaveis do gerenciador
// **********************************************************************

enum ScreenStates{
    MAIN_MENU= 0,
    GAME,
    END_GAME
};

// estado do gerenciador
ScreenStates screenState = MAIN_MENU;

// telas
Game jogo = Game();
MainMenu menu = MainMenu();
EndScreen endScreen = EndScreen();


// **********************************************************************
// inicializa opengl
// **********************************************************************
void init()
{
    Ponto MinAux, MaxAux;

    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Atualiza os limites globais ap�s cada leitura
    Min = Ponto(0,0);
    Max = Ponto(500*SCALA, 500*SCALA);

    // Ajusta a largura da janela l�gica
    // em fun��o do tamanho dos pol�gonos
    Largura.x = Max.x-Min.x;
    Largura.y = Max.y-Min.y;

    // Calcula 1/3 da largura da janela
    Terco = Largura;
    double fator = 1.0/3.0;
    Terco.multiplica(fator, fator, fator);

    // Calcula 1/2 da largura da janela
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    menu.init();
}

double nFrames=0;
double TempoTotal=0;


// **********************************************************************
// Loop do opengl em que os calculos devem ser realizados
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        //angulo+=0.05;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        TempoTotal = 0;
        nFrames = 0;
    }

    /*
        CALCULOS DE MOVIMENTO (ANIMAÇÃO)
    */

    // verifica se precisa trocar de tela
    switch(screenState){
        case MAIN_MENU:
            menu.loop();

            if (menu.startGame){
                jogo.init(menu.getGridChosed(), menu.getGridChosed(), menu.getMineNumberChosed());
                screenState = GAME;
                jogo.CriaTabuleiro();
            }

            menu.startGame = false;

            break;
        case GAME:

            jogo.loop();

            if(jogo.isEndGame()){
                jogo.desenhaTabuleiro();
                endScreen.init(!jogo.getDerrota());
                screenState = END_GAME;
            }

            break;
        case END_GAME:

            if(endScreen.shouldGoBack()){
                jogo = Game();
                endScreen.setGoBack(false);
                screenState = MAIN_MENU;
            }
            break;
        default:
            break;
    }

}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x,
            Min.y,Max.y,
            0,1);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

// **********************************************************************
//  Loop do opengl onde devem ser feitas as chamadas de desenho
//
// **********************************************************************
void display( void )
{
	// Limpa a tela coma cor de fundo
    //	glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(18/255.0, 18/255.0, 18/255.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites l�gicos da �rea OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	// Coloque aqui as chamadas das rotinas que desenham os objetos
//	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    switch(screenState){
        case MAIN_MENU:
            menu.draw();
            break;

        case GAME:
            jogo.desenhaTabuleiro();
            break;

        case END_GAME:
            endScreen.draw();
            break;

        default:
            break;
    }

	glutSwapBuffers();
}
// **********************************************************************
//  entrada do teclado
//  repassa as entradas lidas para a tela atual
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

    switch(screenState){
        case MAIN_MENU:
            menu.keyboard (key, x, y);
            break;
        case GAME:
            jogo.keyboard (key, x, y);
            break;
        case END_GAME:
            endScreen.keyboard (key, x, y);
            break;
        default:
            break;
    }
}
// **********************************************************************
//  Entrada das teclas especiais
//  repassa as entradas lidas para a tela atual
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch(screenState){
        case MAIN_MENU:
            menu.arrow_keys(a_keys, x, y);
            break;
        case GAME:
            jogo.arrow_keys(a_keys, x, y);
            break;
        case END_GAME:
            endScreen.arrow_keys(a_keys, x, y);
            break;
        default:
            break;
    }
}


// **********************************************************************
//  Entrada do mouse
//  repassa as entradas lidas para a tela atual
// **********************************************************************
void mouseInput(int button, int state, int x, int y){
    switch(screenState){
        case MAIN_MENU:
            menu.mouseInput(button, state, x, y);
            break;
        case GAME:
            jogo.mouseInput(button, state, x, y);
            break;
        case END_GAME:
            endScreen.mouseInput(button, state, x, y);
            break;
        default:
            break;
    }
};

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    // inicia semente aleatoria
    srand((unsigned)time(0));

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 500, 500);

    float xPos = (glutGet(GLUT_SCREEN_WIDTH)/2) - (limitesTela.x/2.0);
    float yPos = (glutGet(GLUT_SCREEN_HEIGHT)/2) - (limitesTela.y/2.0) - (glutGet(GLUT_SCREEN_HEIGHT)*0.05);
    glutInitWindowPosition (xPos, yPos);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow    ( "Campo Minado" );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    // Chama metodo de inicialização
    init ();

    // Define metodo que realiza chamadas
    // do display
    glutDisplayFunc ( display );

    // Define metodo que realiza chamadas
    // gerais
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela.
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas.
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    glutSpecialFunc ( arrow_keys );

    // Define que o tratador de evento para
    // o mouse
    glutMouseFunc(mouseInput);

    // inicia o tratamento dos eventos
    glutMainLoop ();

    return 0;
}
