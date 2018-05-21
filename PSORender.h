#ifndef _PSORENDER_H_
#define _PSORENDER_H_

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Particula.h"
#include "Enjambre.h"
#include <stdlib.h>
#include <math.h>


typedef struct Malla
{
    float *vect;
    float maxZ;
    float minZ;
    int Num;
}Malla;

typedef enum Eje
{
    EjeX,
    EjeY,
    Ejez
}Eje;

typedef enum Direccion
{
    Positive,
    Negative
}Direccion;

void graficarMalla(Malla* malla);
Malla* getMalla(Enjambre * enjambre, double (*f)(Particula *pEnj),int N);
void FreeMalla(Malla* vect);
void RenderePEnj(Enjambre * enjambre);
void Rotar();
void Escalar(Malla * malla);
void Actualizar(Eje eje,Direccion direccion);
void Resize(float X,float Y, float Z);
void angulo(float*angulo,Direccion direccion);
/***************************
 * 
 * 
 * *************************/
typedef struct ParticulaP
{
    float *Vector;
    float *Points;
    int D;
    int Numero;
}ParticulaP;

void FreeParticulaP(ParticulaP * p);
void graficar(ParticulaP * P,float t);
void Render(ParticulaP *P);
double powPSO(double x,int N);
int factorial(int N);
int n_i(int N,int i);
ParticulaP *NewParticulaP(float *Vector,int D,int num);
/********************No definidas*************************/
ParticulaP *NewVectorParticulaP(int NumParti,int D,int num);
void FreeVectoParticulasP(ParticulaP * vp,int NumParti);
int copyParticula(ParticulaP * in,ParticulaP * out);
int initParticulas(ParticulaP * out,Enjambre * in,
                    double (*Objetivo)(Particula *pEnj),
                    int (*cmd)(double, double),
                    int Ieraciones);
int copyParticulaVect(ParticulaP * out,float *in,float Fpi,int num);
/*********************************************************/
#endif //_PSORENDER_H_