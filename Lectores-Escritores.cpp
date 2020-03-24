/*
Centro Universitario de Ciencias Exactas e Ingenierías
Departamento de Ciencias Computacionales

Materia: Uso, Adaptación y Explotación de Sistemas Operativos
Profesor: Becerra Velázquez, Violeta del Rocío

Alumno: Alcaraz Domínguez, Guillermo Augusto
Código: 217294148

Carrera: Ingeniería en informática
Sección: D02

Actividad de Aprendizaje 8
Lectores-Escritores

19 de Marzo del 2020
*/

/*
##################### Lectores-Escritores #####################

Programa de Lectores Escritores donde los escritores tienen prioridad, esto
es, No se le permitira a ningun lector el acceso al area una vez que el
escritor haya declarado su intencion de escribir

// true == 1, false == 0

*/

#include <iostream>
#include<stdlib.h>  //Para los numeros aleatorios
#include<time.h>    //Para usar el teimpo como semilla de numeros aleatorios
#include<thread>    //hilos
#include<unistd.h>  //Para el sleep

using namespace std;

// ####### ESCRITOR
void escritor();
bool comenzarEscritura();
void terminarEscritura();

// ####### LECTOR
void lector();
bool comenzarLectura();
void terminarLectura();

// Variables que controlan el estado
int lectores = 0;                   //lectores activos
bool alguienEscribe = false;        //Saber si alguien esta escribiendo
bool escrituraPermitida = false;    //Saber si se puede escribir
bool alguienEsperaEscribir = false; //Saber alguien espera escribir
                                    //(Al ser 1 escritor creo no se usara)

bool LecturaPermitida = false;      //Saber si se puede leer
bool alguienEsperaLeer = false;     //Saber si hay alguien esperando leer

int main()
{
    cout << "\n\t### Lectores-Escritores: Escritores con prioridad ###\n\n";

    int numAleatorio;

    srand(time(NULL)); //Aleatorios diferentes cada vez
    numAleatorio = 1 + rand() % (6 - 1); //Aleatorios entre 1 y 5
    // SI SE DESEA UN RANGO MAYOR CAMBIAR EL 6 de arriba

    int numEscritores = 1;
    int numLectores = numAleatorio;
    //int numLectores = 7;

    int totalDeProcesos = numEscritores + numLectores;

    //genero un numero aleatorio, que es la posicion de llegada del escritor
    int posicionDeLlegadaDelEscritor = rand() % (totalDeProcesos - 1);


    cout << "Escritores(E):    " << numEscritores << endl;
    cout << "Lectores(L):    " << numLectores << endl << endl;

    cout << "Posiciones de llegada: " << endl;

    //imprime posiciones de "llegada" de los procesos
    for(int i = 0; i < totalDeProcesos; i++)
    {
        if(i == posicionDeLlegadaDelEscritor)
        {
            cout << "E";
        }
        else{
            cout << "L";
        }

        if(i < totalDeProcesos - 1)
            cout << " - ";
    }
    cout << endl << endl;

    //Declaracion de hilos
    thread hiloEscritor;
    thread hiloLector[numLectores];

    //Inicia hilos conforme fueron "llegando"
    int nHilosLector = 0;
    for(int i = 0; i < totalDeProcesos; i++)
    {
        if(i == posicionDeLlegadaDelEscritor)
        {
            hiloEscritor = thread(escritor);
        }
        else{
            hiloLector[nHilosLector] = thread(lector);
            nHilosLector++;
        }
    }

    //sincroniza hilos
    for(int i = 0; i < numLectores; i++)
    {
        hiloLector[i].join();
    }
    hiloEscritor.join();


    sleep(1);

    return 0;
}


// ####### ESCRITOR
void escritor()
{
    while(!comenzarEscritura()) //Se repetira hasta que pueda escribir
    {
        //espera 2 para no imprimir tantos
        //mensajes de "no se puede escribir"
        sleep(2);
    }
    sleep(1); //espera 1 simulando lo que se tardo en escribir
    terminarEscritura();
}

bool comenzarEscritura()
{
    //Para saber si se le permitio entrar a escribir
    bool pudoComenzarAEscribir = false;

    //Si hay lectores activos o escritores escribiendo
    if(lectores > 0 || alguienEscribe)
    {
        alguienEsperaEscribir = true; //Se manda a la "cola" de espera
        cout << "No se puede escribir...Esperando para escribir\n";
    }
    else
    {
        alguienEsperaEscribir = false; //No hay nadie esperando a escribir
        alguienEscribe = true; //Nadie esta escribiendo
        LecturaPermitida = false; //Se puede leer
        escrituraPermitida = false; //Se puede escribir
        cout << "Inicio Escritura...\n";
        pudoComenzarAEscribir = true;
    }
    return pudoComenzarAEscribir;
}

void terminarEscritura()
{
    alguienEscribe = false; //Nadie esta escribiendo

    //esto no se va a cumplir nunca por que solo hay un escritor
    if(alguienEsperaEscribir == true)
    {
        //comenzarEscritura();//SI FALLA BORRAR
    }
    else
    {
        //como solo es uno pues al dejar de escribir ya nadie escribe
        alguienEsperaEscribir = false;//no hay otro que escriba

        escrituraPermitida = true;
        LecturaPermitida = true;
        cout << "Termino Escritura...\n";
    }
}
// ####### FIN ESCRITOR

// ####### LECTOR
void lector()
{
    while(!comenzarLectura())//Se repetira hasta que pueda leer
    {
        //espera 2 para no imprimir tantos mensajes de "no se puede leer"
        sleep(2);
    }

    //Genera un tiempo aleaotiro que es lo que "dura" el proceso leyendo
    srand(time(NULL)); //Aleatorios diferentes cada vez
    int tiempo = rand() % (3 - 1); //Aleatorios entre 1 y 2
    sleep(tiempo);

    terminarLectura();
}

bool comenzarLectura()
{
    //Para saber si se le permitio entrar a leer
    bool pudoComenzarALeer = false;

    //Si alguien esta escribiendo o esperando a escribir, el lector se va a
    //la "cola" de espera, al incluir el que alguien este esperando a
    //escribir se le da prioridad a los escritores
    if(alguienEscribe || alguienEsperaEscribir)
    {
        alguienEsperaLeer = true;
        cout << "No se puede leer...Esperando poder leer\n";
    }
    else
    {
        lectores++; //incrementan los lectores activos
        LecturaPermitida = true; //Se permite leer
        escrituraPermitida = false; //Se rechaza la escritura
        cout << "Inicio Lectura...\n";
        pudoComenzarALeer = true;
    }
    return pudoComenzarALeer;
}

void terminarLectura()
{
    lectores--; //Se decrementan los lectores activos

    if(lectores == 0) // Si no hay lectores activos
    {
        //al haber 0 lectores nadie esta esperando leer
        alguienEsperaLeer = false;
        escrituraPermitida = true; //Se permite la escritura
    }
    cout << "Termino Lectura...\n";
}
// ####### FIN LECTOR
