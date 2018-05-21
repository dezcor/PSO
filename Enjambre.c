#include "Enjambre.h"

Enjambre *InicializarEnjambre(const unsigned int NumeroParticulas,
                              const unsigned char NumeroParametrosPorParticula,
                              const float LimitInferiorXi,
                              const float LimiteSuperiorXi,
                              const float LimitInferiorVi,
                              const float LimiteSuperiorVi,
                              const float C1,
                              const float C2,
                              double W,
                              double fw)
{
    unsigned int i;
    Enjambre *enjambre = (Enjambre *)malloc(sizeof(Enjambre));
    enjambre->NumeroParticulas = NumeroParticulas;
    enjambre->NumeroParametrosPorParticula = NumeroParametrosPorParticula;
    enjambre->LimiteSuperiorVi = Vector(LimiteSuperiorVi, NumeroParametrosPorParticula);
    enjambre->LimitInferiorVi = Vector(LimitInferiorVi, NumeroParametrosPorParticula);
    enjambre->LimiteSuperiorXi = Vector(LimiteSuperiorXi, NumeroParametrosPorParticula);
    enjambre->LimitInferiorXi = Vector(LimitInferiorXi, NumeroParametrosPorParticula);
    enjambre->C1 = C1;
    enjambre->C2 = C2;
    enjambre->W = W;
    enjambre->fw = fw;
    enjambre->Pgid = 0;
    enjambre->init = 1;
    enjambre->pEnj = (Particula *)malloc(NumeroParticulas * sizeof(Particula));
    for (i = 0; i < NumeroParticulas; i++)
    {
        NewParticula(enjambre->pEnj + i, enjambre->LimiteSuperiorXi[i], enjambre->LimitInferiorXi[i], NumeroParametrosPorParticula);
    }
    return enjambre;
}

float *VectorV(float *vect, int N)
{
    float *V = NULL;
    V = (float *)malloc(N * sizeof(float));
    unsigned i;
    for (i = 0; i < N; i++)
    {
        V[i] = vect[i];
    }
    return V;
}

float *Vector(float vect, int N)
{
    float *V = NULL;
    V = (float *)malloc(N * sizeof(float));
    unsigned i;
    for (i = 0; i < N; i++)
    {
        V[i] = vect;
    }
    return V;
}
void LiberarEjambre(Enjambre *pEnj)
{
    unsigned int i;
    if (pEnj == NULL)
        return;
    for (i = 0; i < pEnj->NumeroParticulas; i++)
    {
        Free(pEnj->pEnj + i);
    }
    if (pEnj->pEnj != NULL)
        free(pEnj->pEnj);
    if (pEnj->LimiteSuperiorXi != NULL)
        free(pEnj->LimiteSuperiorXi);
    if (pEnj->LimitInferiorXi != NULL)
        free(pEnj->LimitInferiorXi);
    if (pEnj->LimiteSuperiorVi != NULL)
        free(pEnj->LimiteSuperiorVi);
    if (pEnj->LimitInferiorVi != NULL)
        free(pEnj->LimitInferiorVi);
    free(pEnj);
}

void EvaluarEjambre(Enjambre *pEnj, double (*funcion)(Particula *pEnj), int (*cmd)(double, double))
{
    unsigned int i;
    unsigned int d;
    for (i = 0U; i < pEnj->NumeroParticulas; i++)
    {
        pEnj->pEnj[i].Fxi = funcion(pEnj->pEnj + i);
        if (cmd(pEnj->pEnj[i].Fxi, pEnj->pEnj[i].Fpi))
        {
            /*Pi <= Xi*/
            for (d = 0; d < pEnj->NumeroParametrosPorParticula; d++)
            {
                pEnj->pEnj[i].Pi[d] = pEnj->pEnj[i].Xi[d];
            }
            /*Fpi = Fxi*/
            pEnj->pEnj[i].Fpi = pEnj->pEnj[i].Fxi;
        }

        if (cmd(pEnj->pEnj[i].Fxi, pEnj->pEnj[pEnj->Pgid].Fpi))
            pEnj->Pgid = i;
    }
}
void ActualizarVelocidad(Enjambre *pEnj)
{
    unsigned int i;
    int d;
    double E1, E2, aux;
    for (i = 0; i < pEnj->NumeroParticulas; i++)
    {
        for (d = 0; d < pEnj->NumeroParametrosPorParticula; d++)
        {
            E1 = (double)rand() / RAND_MAX;
            E2 = (double)rand() / RAND_MAX;
            aux = pEnj->W * pEnj->pEnj[i].Vi[d] + pEnj->C1 * E1 * (pEnj->pEnj[i].Pi[d] - pEnj->pEnj[i].Xi[d]) + pEnj->C2 * E2 * (pEnj->pEnj[pEnj->Pgid].Pi[d] - pEnj->pEnj[i].Xi[d]);
            if (aux > pEnj->LimiteSuperiorVi[d])
            {
                pEnj->pEnj[i].Vi[d] = pEnj->LimiteSuperiorVi[d];
                continue;
            }
            else if (aux < pEnj->LimitInferiorVi[d])
            {
                pEnj->pEnj[i].Vi[d] = pEnj->LimitInferiorVi[d];
                continue;
            }
            pEnj->pEnj[i].Vi[d] = aux;
        }
    }
}

void ActualizarPosicion(Enjambre *pEnj)
{
    unsigned int i;
    unsigned int d;
    for (i = 0; i < pEnj->NumeroParticulas; i++)
    {
        for (d = 0; d < pEnj->NumeroParametrosPorParticula; d++)
        {
            pEnj->pEnj[i].Xi[d] += pEnj->pEnj[i].Vi[d];

            if (pEnj->pEnj[i].Xi[d] > pEnj->LimiteSuperiorXi[d])
                pEnj->pEnj[i].Xi[d] = pEnj->LimiteSuperiorXi[d];
            else if (pEnj->pEnj[i].Xi[d] < pEnj->LimitInferiorXi[d])
                pEnj->pEnj[i].Xi[d] = pEnj->LimitInferiorXi[d];
        }
    }
}
void EvaluarEjambreI(Enjambre *pEnj, double (*funcion)(Particula *pEnj))
{
    unsigned int i;
    unsigned int d;

    for (i = 0; i < pEnj->NumeroParticulas; i++)
    {
        pEnj->pEnj[i].Fxi = funcion(pEnj->pEnj + i);
        /*Pi <= Xi*/
        for (d = 0; d < pEnj->NumeroParametrosPorParticula; d++)
        {
            pEnj->pEnj[i].Pi[d] = pEnj->pEnj[i].Xi[d];
        }
        /*Fpi = Fxi*/
        pEnj->pEnj[i].Fpi = pEnj->pEnj[i].Fxi;
    }
}

void MostarParticula(Particula *pEnj)
{
    int d;
    printf("\nX: ");
    for (d = 0; d < pEnj->NumParticulas; d++)
    {
        printf("%f\t", pEnj->Xi[d]);
    }
    printf("\nV: ");
    for (d = 0; d < pEnj->NumParticulas; d++)
    {
        printf("%f\t", pEnj->Vi[d]);
    }
    printf("\nP: ");
    for (d = 0; d < pEnj->NumParticulas; d++)
    {
        printf("%f\t", pEnj->Pi[d]);
    }
    printf("\nFxi: %f", pEnj->Fxi);
    printf("\nFpi: %.10f\n\n", pEnj->Fpi);
}

void MostarEjambre(Enjambre *pEnj)
{
    unsigned int i;
    for (i = 0; i < pEnj->NumeroParticulas; i++)
    {
        printf("Particula: %d \n", i);
        MostarParticula(pEnj->pEnj + i);
    }
}

/**********************************************/
int maximo(double a, double b)
{
    return a > b;
}

int minimo(double a, double b)
{
    return a < b;
}
/*********************************************/