#include "Particula.h"
#include "Enjambre.h"
#include "PSORender.h"
#include <time.h>

double xCUadrado(Particula *pEnj);
double f1(Particula* pEnj);
double f0(Particula* pEnj);

Enjambre *enjambre = NULL;
ParticulaP * vp=NULL;
Malla * malla = NULL;
int mostrarRura=0;
float dt;
double Timer=0,Timer2;
int index=-1;
float rango=1.12;
static void display(void)
{
    int i=0,aux;
    glClear(GL_COLOR_BUFFER_BIT);
    //glColor3f(0,1,1);
    glPushMatrix();
        Rotar();
        glColor3f(1,1,1);
        Escalar(malla);
        graficarMalla(malla);
        //RenderePEnj(enjambre);
        glColor4f(0,0,1,0.2);
        if(mostrarRura)
        for(i=0;i < enjambre->NumeroParticulas;i++)
        {
            for(aux=0;aux <=index;aux++)
                graficar(vp+i,aux*dt);
        }
        
        for(i=0;i < enjambre->NumeroParticulas;i++)
        {
            glColor4f(1,0,0,1);
            graficar(vp+i,index*dt);
            //Render(vp+i);
        }
    glPopMatrix();
    /*Mostrar Enjambre*/
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
            exit(0);
            break;
        case 'a':case 'A':
            Actualizar(EjeX,Positive);
            break;
        case 'd':case 'D':
            Actualizar(EjeX,Negative);
            break;
        case 'w':case 'W':
            Actualizar(EjeY,Positive);
            break;
        case 's':case 'S':
            Actualizar(EjeY,Negative);
            break;
        case 'q':case 'Q':
            Actualizar(Ejez,Positive);
            break;
        case 'e':case 'E':
            Actualizar(Ejez,Negative);
            break;
        case 'x':case 'X':
            mostrarRura=!mostrarRura;
            break;
    };
    glutPostRedisplay();
}

void idle()
{
    Timer2 = glutGet(GLUT_ELAPSED_TIME);
    if(Timer2>Timer)
    {
        Timer=Timer2+1000/60;
        if(index==101)
        {
            initParticulas(vp,enjambre,xCUadrado,minimo,vp[0].Numero);
            index=-1;
        }
        //printf("%d\n",index);
        index++;
        glutPostRedisplay();
    }
}

void cierre()
{
    FreeVectoParticulasP(vp,enjambre->NumeroParticulas);
    LiberarEjambre(enjambre);
    FreeMalla(malla);
    
}

// int main()
// {
//     /*f(x,y,z) = 50-(x-5)^2-(y-5)^2-(z-5)^2 */
//     double (*f)(Particula *pEnj);
//     f=xCUadrado;
//     //f=xmasmedio;
//     Enjambre* enjambre=NULL;
//     srand(time(NULL));
//     printf("Inicio\n");
//     /*Inicializar Enjambre*/
//     const int NumeroDeGeneraciones=3000;
//     enjambre= InicializarEnjambre(20,30,-5.12,5.12,-10,10,1.8,1.8,0.6,0.9999);
//     /*Evaluar Enjambre*/
//     EvaluarEjambreI(enjambre,f);
//     /*Mostrar Enjambre*/
//     //MostarEjambre(Enjambre);
//     //getchar();
//     int Iteracion =0;
//     while(Iteracion<NumeroDeGeneraciones && enjambre->pEnj[enjambre->Pgid].Fpi > 0.000001)
//     {
//         /*ActualizarVelocidad*/
//         ActualizarVelocidad(enjambre);
//         /*ActualizarPosicion*/
//         ActualizarPosicion(enjambre);
//         /*EvaluarEnjambre*/
//         EvaluarEjambre(enjambre,f,minimo);
//         /*Mostrar Enjambre*/
//         //MostarEjambre(Enjambre);
//         //getchar();
//         enjambre->W*=enjambre->fw;
//         Iteracion++;
//     }
//     /*Mostrar la Mejor Posicion Historica apuntada por Pgid*/
//     printf("Iteracion: %d  Mejor Particula: %d W: %.18f\n",Iteracion,enjambre->Pgid,enjambre->W);
//     MostarParticula(enjambre->pEnj+enjambre->Pgid);
//     LiberarEjambre(enjambre);
//     return 0;
// }

int main(int argc,char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(150,100); //Posicion de la Ventana
    glutInitWindowSize(600,600); //Tamaño de la Ventana
    glutCreateWindow("Malla");

    enjambre = InicializarEnjambre(30,2,-rango,rango,-10,10,1.8,1.8,0.6,0.9999);
    malla = getMalla(enjambre,xCUadrado,50);
    vp = NewVectorParticulaP(enjambre->NumeroParticulas,3,5);
    if(malla==NULL)return 1;
    printf("%d\n",malla->Num);
        /*Evaluar Enjambre*/
    initParticulas(vp,enjambre,xCUadrado,minimo,vp[0].Numero);
    dt=1.0/100.0;
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-rango*1.1,rango*1.1,-rango*1.1,rango*1.1,-rango*1.1,rango*1.1);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    atexit(&cierre);

    glClearColor(0,0,0,0);

    glutMainLoop();

    return EXIT_SUCCESS;
}


double xCUadrado(Particula *pEnj)
{
    double x0=pEnj->Xi[0];
    double x1=pEnj->Xi[1];
    return 4*x0*x0-2.1*x0*x0*x0*x0+1.0/3.0*x0*x0*x0*x0*x0*x0+
            x0*x1 - 4*x1*x1+4*x1*x1*x1*x1;
}

double f1(Particula* pEnj)
{
    double sum=0,x=0;
    for(int i = 0;i < pEnj->NumParticulas; i++)
    {
        x= pEnj->Xi[i];
        sum += -x*sin(sqrt(fabs(x)));
    }
    return sum;
}


double f0(Particula* pEnj)
{
    double sum=0;
    for(int i = 0;i < pEnj->NumParticulas; i++)
    {
        sum += pEnj->Xi[i]*pEnj->Xi[i];
    }
    return sum;
}