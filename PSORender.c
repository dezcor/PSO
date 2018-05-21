#include "PSORender.h"
float anguloX = 0;
float anguloY = 0;
float anguloZ = 0;

void graficarMalla(Malla* malla)
{
    if(malla == NULL) return;
    float deltaZ= fabs(malla->maxZ-malla->minZ)/malla->Num;
    glColor4f(1,1,1,0.1);
    glBegin(GL_POLYGON);
        glVertex3f(malla->vect[0],malla->vect[0],malla->minZ-deltaZ);
        glVertex3f(malla->vect[0],malla->vect[malla->Num-1],malla->minZ-deltaZ);
        glVertex3f(malla->vect[malla->Num-1],malla->vect[malla->Num-1],malla->minZ-deltaZ);
        glVertex3f(malla->vect[malla->Num-1],malla->vect[0],malla->minZ-deltaZ);
    glEnd();
    glColor4f(0,1,0,1);
    for(int i=0; i<malla->Num;i++)
    {
        glBegin(GL_LINE_STRIP);
        for(int j=0;j < malla->Num;j++)
        {
            glVertex3f(malla->vect[i],malla->vect[j],malla->vect[(i+1)*(malla->Num+1)+j]);
        }
        glEnd();
    }
    for(int i=0; i<malla->Num;i++)
    {
        glBegin(GL_LINE_STRIP);
        for(int j=0;j < malla->Num;j++)
        {
            glVertex3f(malla->vect[j],malla->vect[i],malla->vect[(j+1)*(malla->Num+1)+i]);
        }
        glEnd();
    }

}

void RenderePEnj(Enjambre * enjambre)
{
    unsigned int i=0;
    glColor4f(1,0,0,1);
    glPointSize(4);
    glBegin(GL_POINTS);
    for(i=0; i<enjambre->NumeroParticulas; i++)
    {
        glVertex3f(enjambre->pEnj[i].Pi[0],enjambre->pEnj[i].Pi[1],enjambre->pEnj[i].Fpi);
    }
    glEnd();
}

Malla* getMalla(Enjambre * enjambre, double (*f)(Particula *pEnj),int N)
{
        if(enjambre->NumeroParametrosPorParticula!=2)return NULL;
        const float Rmax= enjambre->LimiteSuperiorXi[0],Rmin=enjambre->LimitInferiorXi[0];

        Particula X;
        Malla * vect = NULL;
        vect= (Malla*)malloc(sizeof(Malla));
        vect->Num=N;
        float rango =(Rmax-Rmin)/N;
        glColor3f(0,0,1);
        int i,j;
        if(vect->vect !=NULL)
            free(vect->vect);
        vect->vect = (float*)malloc(((N+2)*(N+1)+1)*sizeof(float));
        NewParticula(&X,0,0,enjambre->NumeroParametrosPorParticula);
        X.Xi[0]=Rmin;
        X.Xi[1]=Rmin;
        float resul,maxF=-1,minF=f(&X),y,x;
        for(y=Rmin,i=1; y<Rmax;y+=rango,i++)
        {
            X.Xi[0]=y;
            vect->vect[i-1]=y;
            //glBegin(GL_LINE_STRIP);
            for(x=Rmin,j=0;x<Rmax;x+=rango,j++)
            {
                X.Xi[1]=x;
                resul=f(&X);
                vect->vect[i*(N+1)+j]=resul;
                if(fabs(resul)>maxF)
                    maxF = fabs(resul);
                if(resul<minF)
                    minF = resul;
                //glVertex3f(X.Xi[0],X.Xi[1],resul);
            }
        //    glEnd();
        }
        vect->maxZ = maxF;
        vect->minZ = minF;

        Free(&X);
        return vect;
}

void FreeMalla(Malla* vect)
{
    if(vect == NULL)return;
    if(vect->vect == NULL)free(vect->vect);
    free(vect);
}
void angulo(float * angulo,Direccion direccion)
{
    if(direccion == Positive)
    {
        if(*angulo>=360)
        {
            *angulo=0;
        }
        *angulo+=1;
    }
    else
    {
       if(*angulo<=0)
        {
            *angulo=360;
        }
        *angulo-=1;
    }
}

void Actualizar(Eje eje,Direccion direccion)
{
    switch (eje)
    {
        case EjeX:
            angulo(&anguloX,direccion);
            break;
        case EjeY:
            angulo(&anguloY,direccion);
            break;
        case Ejez:
            angulo(&anguloZ,direccion);
            break;
    };
}

void Rotar()
{
    glRotated(anguloX,1,0,0);
    glRotated(anguloY,0,1,0);
    glRotated(anguloZ,0,0,1);
}

void Escalar(Malla * malla)
{
    if(malla== NULL)return;
    float largo=malla->vect[malla->Num-1] - malla->vect[0];
    float Escalar= malla->maxZ - malla->minZ;
    glScalef(1,1,largo/Escalar);
}

void Resize(float X,float Y, float Z)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//gluOrtho2D(-10,10,-10,10);
	glOrtho(-X*1.2,X*1.2,Y*1.2,-Y*1.2,-X*1.2,X*1.2);
}


/************************************
 * 
 * 
 * 
 * 
 ************************************/

void Render(ParticulaP *P)
{
    int i=0;
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
        for(i=0;i < P->Numero;i++)
        {
            glVertex3f(P->Vector[i*P->D],P->Vector[i*P->D+1],P->Vector[i*P->D+2]);
        }
    glEnd();
    glPointSize(5);
    glBegin(GL_POINTS);
        for(i=0;i < P->Numero;i++)
        {
            glVertex3f(P->Vector[i*P->D],P->Vector[i*P->D+1],P->Vector[i*P->D+2]);
        }
    glEnd();
}

double powPSO(double x,int N)
{
    if(N<=0)
        return 1;
    return pow(x,N-1)*x;
}

int factorial(int N)
{
    if(N<=0)
        return 1;
    return factorial(N-1)*N;
}

int n_i(int N,int i)
{
    return factorial(N)/(factorial(i)*factorial((N)-i));
}

ParticulaP *NewParticulaP(float *Vector,int D,int num)
{
    if(D<=1 || D>3)return NULL;
    ParticulaP * newP=NULL;
    newP= (ParticulaP*)malloc(sizeof(ParticulaP));
    newP->Numero = num;
    newP->D = D;
    newP->Vector =(float*)calloc( (D+1) * num, sizeof(float));
    newP->Vector =(float*)calloc( num, sizeof(float));
    int i;
    for(i=0;i < D * num;i++)
    {
        newP->Vector[i]= Vector[i];
    }
    for(i=0;i < num;i++)
    {
        newP->Points[i]= n_i(num-1,i);
    }
    return newP;
}

void FreeParticulaP(ParticulaP * p)
{
    if(p==NULL)return;
    if(p->Vector!=NULL)
        free(p->Vector);
    if(p->Points!=NULL)
        free(p->Points);
    free(p);
}

void graficar(ParticulaP * P,float t)
{
    if(P==NULL)return;
    float vect[3]={0};
    float x=0,y=0,z=0,T=t*100;
    int i=0,j;
    for(i=0;i<P->Numero;i++)
    {
        for(j=0;j<P->D;j++)
        {
            vect[j]+=P->Points[i]*P->Vector[i*P->D+j]*pow(1-t,(P->Numero-1)-i)*pow(t,i);
        }
    }
    glPointSize(4);
    glBegin(GL_POINTS);
        glVertex3f(vect[0],vect[1],vect[2]);
    glEnd();
}


ParticulaP *NewVectorParticulaP(int NumParti,int D,int num)
{
    if(D<=1 || D>3)return NULL;
    ParticulaP * newP=NULL;
    int i,j;
    newP= (ParticulaP*)malloc(NumParti*sizeof(ParticulaP));
    for(j=0; j < NumParti; j++)
    {  
        newP[j].Numero = num;
        newP[j].D = D;
        newP[j].Vector =(float*)calloc( (D+1) * num, sizeof(float));
        newP[j].Points =(float*)calloc( num, sizeof(float));
        for(i=0;i < num;i++)
        {
            newP[j].Points[i]= n_i(num-1,i);
        }
    }
    return newP;
}

void FreeVectoParticulasP(ParticulaP * vp,int NumParti)
{
    if(vp==NULL)return;
    int i,j;
    for(j=0; j < NumParti; j++)
    {     
    if(vp[j].Vector!=NULL)
        free(vp[j].Vector);
    if(vp[j].Points!=NULL)
        free(vp[j].Points);
    }
    free(vp);
}

int copyParticula(ParticulaP * in,ParticulaP * out)
{
    if(in == NULL)return 1;
    if(out == NULL) return 2;
    if(in->Numero != out->Numero) return 3;
    if(in->D != out->D) return 4;
    int i,j;
    for(i=0;i< in->Numero*in->D;i++)
    {
        out->Vector[i]=in->Vector[i];
    }
    return 0;
}

int initParticulas(ParticulaP * out,Enjambre * in,
                    double (*Objetivo)(Particula *pEnj),
                    int (*cmd)(double, double),
                    int Iteraciones)
{
    if(out == NULL)return 1;
    if(in == NULL)return 2;
    int i=0,j;
    if(in->init == 1)
    {
        EvaluarEjambreI(in,Objetivo);
        in->init=0;
        for(j=0;j<in->NumeroParticulas;j++)
        {
            copyParticulaVect(out+j,in->pEnj[j].Xi,in->pEnj[j].Fxi,i);
        }
        i++;
    }
    else if (in->init == 0)
    {
        for(j=0;j<in->NumeroParticulas;j++)
        {
            copyParticulaVect(out+j,in->pEnj[j].Xi,in->pEnj[j].Fxi,i);
        }
        i++;
    }
    while(i<Iteraciones)
    {
        ActualizarVelocidad(in);
        ActualizarPosicion(in);
        EvaluarEjambre(in,Objetivo,cmd);   
        for(j=0;j<in->NumeroParticulas;j++)
        {
            copyParticulaVect(out+j,in->pEnj[j].Xi,in->pEnj[j].Fxi,i);
        }
        i++;
    }
    //MostarEjambre(in);
}

int copyParticulaVect(ParticulaP * out,float *in,float fxi,int num)
{
    if(out == NULL)return 1;
    if(in == NULL)return 2;
    int i,j;
    for(i=0;i < out->D-1;i++)
    {
        out->Vector[num*out->D+i]=in[i];
    }
    out->Vector[num*out->D+i]=fxi;
    return 0;
}