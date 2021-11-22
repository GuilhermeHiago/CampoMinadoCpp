#include "EndScreen.h"
#include "Game.h"

// **********************************************************************
//
// Guilherme Hiago Costa dos Santos
//
// Tela de fim de jogo
// **********************************************************************

EndScreen::EndScreen()
{
    //ctor
}

void EndScreen::init(bool vitoria){
    this->vitoria = vitoria;
}

void EndScreen::keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		default:
		    this->goBack = true;
			break;
	}
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void EndScreen::arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
	    /*
		case GLUT_KEY_UP:       // Se pressionar UP
			break;
	    case GLUT_KEY_DOWN:     // Se pressionar UP
			break;

        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        */
		default:
		    this->goBack = true;
			break;
	}
}

void EndScreen::mouseInput(int button, int state, int x, int y){
    //this->goBack = true;
}

// **********************************************************************
//
// Desenha Tela Final
// **********************************************************************
void EndScreen::draw(){
    glClearColor(1,1,1,1);

    glPushMatrix();
    glRasterPos2f(this->limitesTela.x/2-(3.5*8), this->limitesTela.y/2-5);

    if(this->vitoria){
        glColor3ub(82,165,147);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Vit�ria");
    }
    else{
        glColor3ub(217, 11, 28);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Derrota");
    }

    glRasterPos2f(this->limitesTela.x/2-(21*8), this->limitesTela.y/2-25);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Pressione Uma tecla para Voltar ao Menu");

    glPopMatrix();
}

bool EndScreen::shouldGoBack(){
    return this->goBack;
}

void EndScreen::setGoBack(bool goBack){
    this->goBack = goBack;
}

EndScreen::~EndScreen()
{
    //dtor
}
