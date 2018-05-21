#ifndef _PARTICULA_H_
#define _PARTICULA_H_

#include <stdlib.h>

typedef struct Particula
{
    float * Xi;
    float * Vi;
    float * Pi;
    double Fxi;
    double Fpi;
    int NumParticulas;
}Particula;

int NewParticula(Particula * P,float LS,float LI,int NumParticulas);
int NewParticulaV(Particula * P,float * LS,float * LI, int NumParticulas);
void Free(Particula * P);
#endif //_PARTICULA_H_