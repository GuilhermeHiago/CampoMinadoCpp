#include "Celula.h"

Celula::Celula()
{
    //ctor
}

Celula::Celula(bool isMina, int linha, int coluna)
{
    this->isMina = isMina;
    this->linha = linha;
    this->coluna = coluna;
    this->estado = FECHADA;
    this->qtdMinasVizinhas = 0;
}

Celula::~Celula()
{
    //dtor
}
