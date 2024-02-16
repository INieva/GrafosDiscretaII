// definicion de GrafoSt y u32
#ifndef ESTRUCTURAGRAFO23
#define ESTRUCTURAGRAFO23


typedef struct grafo_st GrafoSt ; 
typedef  unsigned int u32;


GrafoSt* nuevoGrafo(u32 vertices, u32 lados);

void destruirGrafoSt(GrafoSt* G);

u32  getVerticesTotales(GrafoSt* G);

u32 getVerticesContados(GrafoSt* G);

u32 getMaximoLados(GrafoSt* G);

u32 getDelta(GrafoSt* G);

u32 getNombre(u32 i, GrafoSt* G);

u32 getGrado(u32 i, GrafoSt* G);

u32 getIndiceVecino(u32 j, u32 i, GrafoSt* G);

u32 agregaVertice(GrafoSt* G, u32 nombre);

void agregarLado(GrafoSt* G, u32 posicion, u32 vertice);

void ordenarGrafo(GrafoSt* G);

void printGrafo(GrafoSt* G);


#endif