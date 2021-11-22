#include "Game.h"

// **********************************************************************
//
// Guilherme Hiago Costa dos Santos
// Tela de jogo
// **********************************************************************

// **********************************************************************
//    Variaveis do jogo
// **********************************************************************

const float SCALA = 0.25;
const Ponto limitesTela = Ponto(500, 500);
int contadorFinal;

Game::Game()
{
    this->derrota = false;
    this->fim = false;
}

// **********************************************************************

// Inicializa novo jogo, recebendo dimenções e quantidade de minas presentes

// **********************************************************************
void Game::init( int nCelulasHorizontal, int nCelulasVertical, int numeroMinas){
    this->fim = false;
    this->derrota = false;

    this->nCelulasHorizontal = nCelulasHorizontal;
    this->nCelulasVertical = nCelulasVertical;

    this->nMinas = numeroMinas;


    // inicializa tabuleiro
    this->tabuleiroCelula = new Celula*[nCelulasHorizontal];
    for (int i = 0; i < nCelulasVertical; ++i) {
      this->tabuleiroCelula[i] = new Celula[nCelulasVertical];
    }
}

// **********************************************************************
// Cria os pontos de um circulo
// **********************************************************************
Poligono Game::GenerateCircle(float cx, float cy, float r, int num_segments) {
    Poligono p = Poligono();

    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        p.insereVertice(Ponto(cx+x, cy+y));
        //glVertex2f(x + cx, y + cy);//output vertex
    }

    return p;
}
// **********************************************************************
// Cria o tabuleiro com posicionamento aleatorio de minas
// **********************************************************************
void Game::CriaTabuleiro(){

    /* initialize random seed: */
    srand (time(NULL));

    int qtd = this->nMinas;

    Ponto posMinas[qtd];

    int k = 0;

    while(k < qtd){
        Ponto pos = Ponto(rand() % nCelulasHorizontal, rand() % nCelulasVertical);

        bool valido = true;

        Ponto *p = find(posMinas, posMinas+qtd, pos);

        // caso nao encontre repeti�ao
        if(p == posMinas+qtd){
            posMinas[k] = pos;
            k++;
        }
    }

    for(int i = 0; i < nCelulasVertical; i++){
        Celula linha[nCelulasHorizontal];

        for(int j = 0; j < nCelulasHorizontal; j++){
            Ponto *p = find(posMinas, posMinas+qtd, Ponto(i, j));

            // caso numero seja repitido
            if(p != posMinas+qtd){
                tabuleiroCelula[i][j] = Celula(true, i, j);
            }else{
                tabuleiroCelula[i][j] = Celula(false, i, j);
            }
        }
    }
}
// **********************************************************************
// Conta a quantidade de minas vizinhas a celula x,y
// **********************************************************************
int Game::contaMinasVizinhas(int x, int y){
    int qtd = 0;

    for(int i = x-1; i <= x+1 && i < nCelulasHorizontal; i++){
        if(i < 0) continue;

        for(int j = y-1; j <= y+1 && j < nCelulasVertical; j++){
            if(j < 0) continue;
            if(i == x && j == y) continue;

            if(tabuleiroCelula[i][j].isMina)
                qtd++;
        }
    }

    tabuleiroCelula[x][y].qtdMinasVizinhas = qtd;
    tabuleiroCelula[x][y].estado = NUMERADA;

    if(qtd == 0){
        tabuleiroCelula[x][y].estado = ABERTA;
        for(int i = x-1; i <= x+1 && i < nCelulasHorizontal; i++){
            if(i < 0) continue;

            for(int j = y-1; j <= y+1 && j < nCelulasVertical; j++){
                if(j < 0) continue;
                if(i == x && j == y) continue;
                if(tabuleiroCelula[i][j].isMina) break;

                if(tabuleiroCelula[i][j].estado == FECHADA || tabuleiroCelula[i][j].estado == BANDEIRA)
                    contaMinasVizinhas(i, j);
            }
        }
    }

    return qtd;
}

// **********************************************************************
// Utiliza opengl para desenhar bandeira na 
// celula posCelula.x, posCelula.y
// **********************************************************************
void Game::drawFlag(Ponto* posCelula){
    float celWidth = limitesTela.x/nCelulasHorizontal;
    float celHeight = limitesTela.y/nCelulasVertical;

    glPushMatrix();

        glTranslatef(posCelula->x*celWidth + celWidth/2, ((nCelulasVertical-1)-posCelula->y)*celHeight + celHeight/5, 0);

        // cor vermelha para bandeira
        glColor3f(217/255.0, 11/255.0, 28/255.0);

        glBegin(GL_POLYGON);

            glVertex3f(0, 0, 0);
            glVertex3f(2, 0, 0);
            glVertex3f(2, celHeight*0.60, 0);
            glVertex3f(0, celHeight*0.60, 0);

        glEnd();

        glBegin(GL_POLYGON);

            glVertex3f(0, celHeight*0.60, 0);
            glVertex3f(0, celHeight*0.40, 0);
            glVertex3f(celHeight*-0.4, celHeight*0.40, 0);
            glVertex3f(celHeight*-0.20, celHeight*0.50, 0);

        glEnd();

        // cor preta para base da bandeira
        glColor3f(18/255.0, 18/255.0, 18/255.0);

        glBegin(GL_POLYGON);

            glVertex3f(celWidth*-0.15, 0, 0);
            glVertex3f(celWidth*0.17, 0, 0);

            glVertex3f(celWidth*0.15, celWidth*0.04, 0);
            glVertex3f(celWidth*-0.17, celWidth*0.04, 0);

        glEnd();
    glPopMatrix();
}
// **********************************************************************
// Utiliza opengl para desenhar mina na 
// celula pos.x, pos.y
// **********************************************************************
void Game::desenhaMina(Ponto pos){
    // Desenhas circulo
    int i = pos.x;
    int j = pos.y;

    float celWidth = limitesTela.x/nCelulasHorizontal;
    float celHeight = limitesTela.y/nCelulasVertical;

    Poligono corpo = GenerateCircle(i*celWidth + celWidth/2, j*celHeight + celHeight/2, celHeight/10, 100);

    glPushMatrix();
    {
        //glTranslatef(position.x, position.y, 0);
        glBegin(GL_POLYGON);

        // rotaciona no meio
        //glTranslatef(corpo.Max.x/2-0.8, 0, 0);
        //glTranslatef(-corpo.Max.x/2-0.8, 0, 0);
        for (int k=0; k<corpo.getNVertices(); k++){
            glVertex3f(corpo.getVertice(k).x, nCelulasVertical*celHeight - corpo.getVertice(k).y, corpo.getVertice(k).z);
        }

        glEnd();
    }
    glPopMatrix();

    // desenha bomba
    glPushMatrix();

    glTranslatef(i*celWidth + celHeight/2, ((nCelulasVertical-1)-j)*celHeight + celHeight/2, 0);

    for(int k = 0; k < 8; k++){
        glRotatef(45, 0, 0, 1);
        glBegin(GL_POLYGON);

        glVertex3f(-2, 0, 0);
        glVertex3f(0, celHeight*0.3, 0);
        glVertex3f(2, 0, 0);

        glEnd();
    }

    glPopMatrix();
}
// **********************************************************************
// Utiliza opengl para desenhar fundo da celula na 
// celula pos.x, pos.y
// **********************************************************************
void Game::desenhaCelula(Ponto pos){
    int i = pos.x;
    int j = pos.y;

    glPushMatrix();

    glBegin(GL_POLYGON);

    float celWidth = limitesTela.x/nCelulasHorizontal;
    float celHeight = limitesTela.y/nCelulasVertical;

    glVertex2f(i*celWidth+2, (nCelulasVertical-j)*celHeight-2);
    glVertex2f(i*celWidth+(celWidth-2), (nCelulasVertical-j)*celHeight-2);
    glVertex2f(i*celWidth+(celWidth-2), (nCelulasVertical-j)*celHeight-(celHeight-2));
    glVertex2f(i*celWidth+2, (nCelulasVertical-j)*celHeight-(celHeight-2));

    glEnd();

    glPopMatrix();
}
// **********************************************************************
// Escreve texto t na posiçao pos.x, pos.y
// **********************************************************************
void Game::desenhaTexto(const char* t, Ponto pos){
    glPushMatrix();

    float xAdd = pos.x * limitesTela.x/nCelulasHorizontal + (limitesTela.x/nCelulasHorizontal)*0.5-6;
    float yAdd = ((nCelulasVertical-1)-pos.y) * limitesTela.y/nCelulasVertical + (limitesTela.y/nCelulasVertical)*0.5-8;

    glRasterPos2f(xAdd,yAdd);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)t);
    fim = true;
    glPopMatrix();
}
// **********************************************************************
// Usa opengl para desenhar a grade
// **********************************************************************
void Game::desenhaMargens(){
    for(int i = 0; i < limitesTela.x; i+=(limitesTela.x/nCelulasHorizontal)){
        glPushMatrix();
        {
            //glTranslatef(i, 0, 0);
            //glScalef(escala.x, escala.y, escala.z);

            glBegin(GL_LINES);

            glColor3f(1, 1, 1);

            glVertex3f(i, 0, 0);
            glVertex3f(i, limitesTela.y, 0);

            glEnd();
        }
        glPopMatrix();
    }

    for(int i = 0; i < limitesTela.y; i+=(limitesTela.y/nCelulasVertical)){
        glPushMatrix();
        {
            //glTranslatef(i, 0, 0);
            //glScalef(escala.x, escala.y, escala.z);

            glBegin(GL_LINES);

            glColor3f(1, 1, 1);

            glVertex3f(0, i, 0);
            glVertex3f(limitesTela.x, i, 0);

            glEnd();
        }
        glPopMatrix();
    }
}
// **********************************************************************
// Chama todos os metodos de desenho necessarios
// **********************************************************************
void Game::desenhaTabuleiro(){
    desenhaMargens();

    for(int i = 0; i < nCelulasVertical; i++){

        for(int j = 0; j < nCelulasVertical; j++){
            switch(tabuleiroCelula[i][j].estado)
            {
                case FECHADA:

                    glColor3ub(149,149,149);
                    desenhaCelula(Ponto(i, j));

                    break;

                case ABERTA:
                    if (tabuleiroCelula[i][j].isMina){
                        glColor3ub(217, 11, 28);
                        //glColor3f(217/255.0, 11/255.0, 28/255.0);
                        desenhaCelula(Ponto(i,j));

                        glColor3ub(18,18,18);
                        desenhaMina(Ponto(i, j));
                        break;
                    }

                    //glColor3b(61, 61, 61);
                    glColor3ub(18,18,18);
                    desenhaCelula(Ponto(i,j));

                    glPopMatrix();

                    break;
                case BANDEIRA:
                    //glColor3b(61, 61, 61);
                    glColor3ub(149,149,149);
                    desenhaCelula(Ponto(i,j));

                    drawFlag(new Ponto(i, j));
                    break;

                case NUMERADA:
                    //glColor3b(61, 61, 61);
                    glColor3ub(18,18,18);
                    desenhaCelula(Ponto(i,j));

                    {
                        //glColor3b(255, 255, 255);
                        //glColo
                        glColor3f(1,1,1);

                        int qtd = tabuleiroCelula[i][j].qtdMinasVizinhas;

                        int colors[8][3] = {{76,106,200}, {82,165,147}, {217, 11, 28}, {42,31,66}, {142,36,63}, {34,64,68}, {18, 18, 18}, {158,81,50}};
                        glColor3ub(colors[qtd-1][0], colors[qtd-1][1], colors[qtd-1][2]);

                        string s = to_string(qtd);
                        char const *pchar = s.c_str();  //use char const* as target type

                        desenhaTexto(pchar, Ponto(i,j));
                        //drawString(i, j, 1, p);
                    }
                    break;

            }
        }
    }

    if(this->fim){
        contadorFinal++;
    }
}
// **********************************************************************
// Verifica de jogo acabou
// **********************************************************************
bool Game::verificaFimJogo(){
    if(this->derrota){
        this->fim = true;
        return true;
    }

    for(int i = 0; i < nCelulasHorizontal; i++){
        for(int j = 0; j < nCelulasVertical; j++){
            if(!tabuleiroCelula[i][j].isMina && (tabuleiroCelula[i][j].estado != ABERTA && tabuleiroCelula[i][j].estado != NUMERADA)){
                fim = false;
                return fim;
            }
        }
    }

    this->fim = true;
    return true;
}

void Game::keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		default:
			break;
	}
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void Game::arrow_keys ( int a_keys, int x, int y )
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
			break;
	}
}

void Game::mouseInput(int button, int state, int x, int y){
    //cout << button << " State: " << state << " x: "<< x << " y: " << y << endl;

    x = x*limitesTela.x/glutGet(GLUT_WINDOW_WIDTH);
    y = y*limitesTela.y/glutGet(GLUT_WINDOW_HEIGHT);

    int xCel = ceil(x/(limitesTela.x/nCelulasHorizontal))-1;
    int yCel = ceil(y/(limitesTela.y/nCelulasVertical))-1;

    //cout << "celula clicada: " << xCel << ", " << yCel << endl;
    
    // Verifica celula pressionada com botão esquerdo
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            // Abre a celula
            if(tabuleiroCelula[xCel][yCel].isMina){
                tabuleiroCelula[xCel][yCel].estado = ABERTA;
                this->derrota = true;
                this->fim = true;
            }
            else if (tabuleiroCelula[xCel][yCel].estado != ABERTA){
                tabuleiroCelula[xCel][yCel].qtdMinasVizinhas = contaMinasVizinhas(xCel, yCel);

                if(tabuleiroCelula[xCel][yCel].qtdMinasVizinhas != 0){
                    tabuleiroCelula[xCel][yCel].estado = NUMERADA;
                }
                else{
                    tabuleiroCelula[xCel][yCel].estado = ABERTA;
                }

            }
        }
    }
    // Verifica celula pressionada com botão direito
    else if(button == GLUT_RIGHT_BUTTON){
        // adiciona bandeira na celula
        if(state == GLUT_DOWN){
            if (tabuleiroCelula[xCel][yCel].estado == FECHADA){
                tabuleiroCelula[xCel][yCel].estado = BANDEIRA;
            }
            else if(tabuleiroCelula[xCel][yCel].estado == BANDEIRA){
                tabuleiroCelula[xCel][yCel].estado = FECHADA;
            }
        }
    }
}

void Game::loop(){
    this->verificaFimJogo();
}

bool Game::isEndGame(){
    return this->fim;
}

bool Game::getDerrota(){
    return this->derrota;
}

Game::~Game()
{
    //dtor
}
