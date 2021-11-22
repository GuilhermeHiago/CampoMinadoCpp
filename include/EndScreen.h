#ifndef ENDSCREEN_H
#define ENDSCREEN_H

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

class EndScreen
{
    public:
        EndScreen();
        virtual ~EndScreen();
        void init(bool vitoria);
        void draw();
        bool shouldGoBack();
        void setGoBack(bool goBack);

        void loop();
        void mouseInput(int button, int state, int x, int y);
        void keyboard ( unsigned char key, int x, int y );
        void arrow_keys ( int key, int x, int y );

    protected:

    private:
        bool vitoria;
        bool goBack = false;
        const float SCALA = 0.25;
        Ponto limitesTela = Ponto(500, 500);
};

#endif // ENDSCREEN_H
