//
//  ControlaTempo.hpp
//  Imagens
//
//  Created by M�rcio Sarroglia Pinho on 23/03/20.
//  Copyright � 2020 rmd. All rights reserved.
//

#ifndef ControlaTempo_hpp
#define ControlaTempo_hpp

#include <iostream>
using namespace std;

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif


class Temporizador
{
#ifdef WIN32
    DWORD start_time;
#else
    struct timeval start_time;
#endif
public:
    Temporizador(); // Inicializa o temporizador
    double getDeltaT(); // Retorna o tempo decorrido desde a �ltima chamada desta mesma fun��o
};

#endif /* ControlaTempo_hpp */
