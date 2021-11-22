#ifndef CELULA_H
#define CELULA_H

enum Estados{
    FECHADA = 0,
    ABERTA,
    NUMERADA,
    BANDEIRA

};

class Celula
{
    public:
        bool isMina;
        int qtdMinasVizinhas;
        enum Estados estado;

        Celula();
        Celula(bool isMina, int linha, int coluna);
        virtual ~Celula();

    protected:

    private:
        int linha;
        int coluna;
};

#endif // CELULA_H
