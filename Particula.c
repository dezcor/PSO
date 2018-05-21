#include "Particula.h"

int NewParticula(Particula *P, float LS, float LI, int NumParticulas)
{
    int i;
    if (NumParticulas <= 0)
        return 1;
    P->Xi = (float *)calloc(NumParticulas, sizeof(float));
    P->Vi = (float *)calloc(NumParticulas, sizeof(float));
    P->Pi = (float *)calloc(NumParticulas, sizeof(float));
    P->NumParticulas = NumParticulas;
    for (i = 0; i < NumParticulas; i++)
    {
        P->Xi[i] = ((double)rand() / RAND_MAX) * (LS - LI) + LI;
        P->Vi[i] = 0;
        P->Pi[i] = P->Xi[i];
    }

    return 0;
}
int NewParticulaV(Particula *P,float *LS, float *LI, int NumParticulas)
{
    int i;
    if (NumParticulas <= 0)
        return 1;
    P->Xi = (float *)calloc(NumParticulas, sizeof(float));
    P->Vi = (float *)calloc(NumParticulas, sizeof(float));
    P->Pi = (float *)calloc(NumParticulas, sizeof(float));
    P->NumParticulas = NumParticulas;
    for (i = 0; i < NumParticulas; i++)
    {
        P->Xi[i] = ((double)rand() / RAND_MAX) * (LS[i] - LI[i]) + LI[i];
        P->Vi[i] = 0;
        P->Pi[i] = P->Xi[i];
    }

    return 0;
}

void Free(Particula *P)
{
    if(P->Xi != NULL) free(P->Xi);
    if(P->Vi != NULL) free(P->Vi);
    if(P->Pi != NULL) free(P->Pi);
}