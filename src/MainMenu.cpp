#include "MainMenu.h"
#include "Ponto.h"
#include <string>

// **********************************************************************
//
// Guilherme Hiago Costa dos Santos
// Tela Menu Principal
// **********************************************************************


const int GridOptions[] = {5, 10, 20, 25};
int values[] = {10, 9};
int gridChoise = 1;

MainMenu::MainMenu()
{
    //ctor
    this->startGame = false;
    this->selectState = CELL_NUMBER;
}

void MainMenu::keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
        case 13:
            this->startGame = true;
		default:
		    //this->startGame = true;
			break;
	}
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void MainMenu::arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
        // altera configuração selecionada
		case GLUT_KEY_UP:       // Se pressionar UP
		    switch(this->selectState){
                case MINE_NUMBER:
                    this->selectState = CELL_NUMBER;
                    break;

                default:
                    break;
		    }

			break;
        // altera configuração selecionada
	    case GLUT_KEY_DOWN:     // Se pressionar UP
	        switch(this->selectState){
                case CELL_NUMBER:
                    this->selectState = MINE_NUMBER;
                    break;

                default:
                    break;
		    }
			break;

        case GLUT_KEY_LEFT:
            // diminui numero de celulas
            if(this->selectState == CELL_NUMBER){
                gridChoise = (4+(gridChoise-1))%4;
                values[0] = GridOptions[gridChoise];

                if(values[1] > values[0]*2){
                    values[1] = values[0]*2;
                }
            }
            // ou diminui numero de minas
            else{
                values[1] = 1 + (((values[0]*2) + (values[1]-2)) % (values[0]*2));
            }

            break;
        case GLUT_KEY_RIGHT:
            // aumenta numero de celulas
            if(this->selectState == CELL_NUMBER){
                gridChoise = (4+(gridChoise+1))%4;
                values[0] = GridOptions[gridChoise];

                if(values[1] > values[0]*2){
                    values[1] = values[0]*2;
                }
            }
            // ou aumenta numero de minas
            else{
                values[1] = ((values[0]*2) + (values[1]+1)) % (values[0]*2);
            }

            break;
		default:
			break;
	}
}

void MainMenu::mouseInput(int button, int state, int x, int y){
    //this->startGame = true;
}

void MainMenu::init(){

}

// **********************************************************************
// Desenha Menu Principal
//
// **********************************************************************
void MainMenu::draw(){
    glClearColor(18/255.0,18/255.0,18/255.0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for(int i = 0; i < 3; i++){
        if(this->selectState == i-1){
            glColor3ub(100,100,255);
        }
        else{
            glColor3ub(200,200,200);
        }

        glPushMatrix();
        glRasterPos2f(this->limitesTela.x/2-8*(this->options[i].length()/2), this->limitesTela.y/(1+i) -(this->limitesTela.y*0.1));

        string aux = this->options[i];

        if(i-1 == CELL_NUMBER){
            aux = (this->options[i] + " (" + to_string(values[i-1]) + "x" + to_string(values[i-1]) + ")");
            glRasterPos2f(this->limitesTela.x/2-8*(aux.length()/2), this->limitesTela.y/(1+i) -(this->limitesTela.y*0.1));
        }
        else if(i-1 == MINE_NUMBER){
            aux = (this->options[i] + " (" + to_string(values[i-1]) + ")");
            glRasterPos2f(this->limitesTela.x/2-8*(aux.length()/2), this->limitesTela.y/(1+i) -(this->limitesTela.y*0.1));
        }

        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)aux.c_str());

        glPopMatrix();
    }
}

int MainMenu::getGridChosed(){
    return values[0];
}
int MainMenu::getMineNumberChosed(){
    return values[1];
}

void MainMenu::loop(){

}

MainMenu::~MainMenu()
{
    delete &SCALA;
    delete &limitesTela;
    delete &this->selectState;
}
