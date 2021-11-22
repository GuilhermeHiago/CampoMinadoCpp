#ifndef Ponto_hpp
#define Ponto_hpp

#include <iostream>
using namespace std;

class Ponto {

public:
    float x,y,z;
    Ponto ();
    Ponto(float x, float y, float z=0);
    void set(float x, float y, float z=0);
    void imprime();
    void multiplica(double x, double y, double z);
    void soma(double x, double y, double z);
} ;


Ponto ObtemMinimo (Ponto P1, Ponto P2);

Ponto ObtemMaximo (Ponto P1, Ponto P2);

Ponto operator+(Ponto P1, Ponto P2);
Ponto operator- (Ponto P1, Ponto P2);
Ponto operator* (Ponto P1, float k);
Ponto operator-(Ponto P1);
bool operator==(Ponto P1, const Ponto P2);
//Ponto operator==(Ponto P1, Ponto P2);

#endif /* Ponto_hpp */
