#ifndef _ENJAMBRE_H_
#define _ENJAMBRE_H_

#include "Particula.h"
#include <stdio.h>

typedef struct Enjambre
{
    unsigned int NumeroParticulas;
    unsigned char NumeroParametrosPorParticula;
    unsigned int NumeroDeGeneraciones; 
    float *LimitInferiorXi;
    float *LimiteSuperiorXi;
    float *LimitInferiorVi;
    float *LimiteSuperiorVi;
    float C1;
    float C2;
    double W;
    double fw;
    unsigned int Pgid;
    Particula *pEnj;
    unsigned char init;
}Enjambre;

Enjambre * InicializarEnjambre(const unsigned int NumeroParticulas,
                                const unsigned char NumeroParametrosPorParticula, 
                                const float LimitInferiorXi,
                                const float LimiteSuperiorXi,
                                const float LimitInferiorVi,
                                const float LimiteSuperiorVi,
                                const float C1,
                                const float C2,
                                double W,
                                double fw);
void EvaluarEjambre(Enjambre* pEnj,double (*funcion)(Particula *pEnj),int (*cmd)(double,double));
void LiberarEjambre(Enjambre* pEnj);
void ActualizarVelocidad(Enjambre * pEnj);
void ActualizarPosicion(Enjambre * pEnj);
void EvaluarEjambreI(Enjambre * pEnj,double (*funcion)(Particula *pEnj));
void MostarParticula(Particula *pEnj);
void MostarEjambre(Enjambre * pEnj);
float * VectorV(float*vect,int N);
float * Vector(float vect,int N);
/**********************************************/
int maximo(double a,double b);
int minimo(double a,double b);
/**********************************************/
#endif //_ENJAMBRE_H_