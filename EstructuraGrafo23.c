#include "EstructuraGrafo23.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

struct Info{
    bool libre;
    u32 grado_maximo;
    u32 maximo_mod;
    u32 cantidad_mod;
    u32* indice_mod;
};

struct Aux{
    u32 vertices_contados;
    u32 aumento;
    u32 indices_ocupados;
};

struct Nodo{
    u32 nombre;
    u32 grado;
    u32 *vecinos;
};

struct grafo_st{
    u32 vertices;
    u32 lados; 
    u32 delta;  
    struct Nodo *nodo;
    struct Info *info;
    struct Aux *aux;
};

GrafoSt* nuevoGrafo(u32 vertices, u32 lados){
    GrafoSt* new = malloc(sizeof(struct grafo_st));
    new->vertices = vertices;
    new->lados = lados;
    new->delta = 0;
    new->nodo = malloc(sizeof(struct Nodo) * vertices);
    new->info = malloc(sizeof(struct Info) * vertices);
    new->aux = malloc(sizeof(struct Aux));
    new->aux->vertices_contados = 0;
    new->aux->aumento = vertices/100 +1;
    new->aux->indices_ocupados = 0;
    for(u32 i=0; i<vertices; i++){
        new->nodo[i].vecinos = NULL;
        new->nodo[i].grado = 0;
        new->info[i].libre = true;
        new->info[i].grado_maximo = 0;
        new->info[i].cantidad_mod = 0;
        new->info[i].indice_mod = NULL;
    }
    return new;
}

static void destruirDatosDeCarga(GrafoSt* G){
    if(G != NULL){
        if (G->info != NULL){
            for(u32 i=0; i < G->vertices; i++){
                if(G->info[i].indice_mod != NULL){
                    free(G->info[i].indice_mod);
                }
            }
            free(G->info);
            G->info = NULL;
        }
        if(G->aux!=NULL){
            free(G->aux);
            G->aux = NULL;
        }
    }
}

void destruirGrafoSt(GrafoSt* G){
    if(G != NULL){
        destruirDatosDeCarga(G);
        for(u32 i=0; i < G->vertices; i++){
            if(G->nodo[i].vecinos != NULL){
                free(G->nodo[i].vecinos);
            }
        }
        free(G->nodo);
        free(G->info);
        free(G);
        G = NULL;
    }
}

static void agregarIndiceMod(GrafoSt* G, u32 mod, u32 i){
    u32 cant = G->info[mod].cantidad_mod;
    if (cant == 0){
        G->info[mod].indice_mod = malloc(sizeof(u32)*G->aux->aumento);
        G->info[mod].maximo_mod = G->aux->aumento;
    } else if(cant >= G->info[mod].maximo_mod){
        u32* nuevo = realloc(G->info[mod].indice_mod, sizeof(u32)*(cant + G->aux->aumento));
        G->info[mod].maximo_mod += G->aux->aumento;
        if(nuevo != NULL){
            G->info[mod].indice_mod = nuevo;
        }
    }
    G->info[mod].indice_mod[cant] = i;
    G->info[mod].cantidad_mod++;
}

u32 agregaVertice(GrafoSt* G, u32 nombre){
    u32 modulo = nombre % G->vertices;
    u32 indice = pow(2,32)-1;               
    if (G->info[modulo].libre){                     
        G->nodo[modulo].nombre = nombre;
        G->info[modulo].libre = false;
        G->aux->vertices_contados ++;
        indice = modulo;
    } else if (G->nodo[modulo].nombre == nombre){  
        indice = modulo;
    } else {
        bool encontrado = false;          
        for(u32 i=0; i < G->info[modulo].cantidad_mod; i++){
            u32 j = G->info[modulo].indice_mod[i];
            if (G->nodo[j].nombre == nombre){      
                indice = j;
                encontrado = true;
                break;
            }
        }   
        if (!encontrado){
            for(u32 i = G->aux->indices_ocupados; i < G->vertices; i++){
                if (G->info[i].libre){
                    G->nodo[i].nombre = nombre;
                    G->info[i].libre = false;
                    G->aux->vertices_contados++;
                    agregarIndiceMod(G,modulo,i);
                    G->aux->indices_ocupados = i;
                    indice = i;
                    break;
                } 
            }
        }
    }
    return indice;   
}

void agregarLado(GrafoSt* G, u32 i, u32 vertice){
    u32 grado = G->nodo[i].grado;
    if (grado == 0){
         G->nodo[i].vecinos  = malloc(sizeof(u32)*G->aux->aumento);
         G->info[i].grado_maximo = G->aux->aumento;
    } else if(grado >= G->info[i].grado_maximo){
        u32* nuevo = realloc(G->nodo[i].vecinos, sizeof(u32)*(grado + G->aux->aumento));
        G->info[i].grado_maximo += G->aux->aumento;
        if(nuevo != NULL){
            G->nodo[i].vecinos = nuevo;
        }
    }
    G->nodo[i].vecinos[grado] = vertice;
    G->nodo[i].grado++;
    if(G->nodo[i].grado > G->delta){
        G->delta = G->nodo[i].grado ;
    }
}

static u32 indiceVertice(GrafoSt* G,u32 cantidad, u32 vertice){
    u32 indice = pow(2,32)-1;
    u32 centro = 0, inf = 0, sup = cantidad-1;
    while(inf <= sup){
        centro = ((sup-inf)/2)+inf;
        if(G->nodo[centro].nombre == vertice){
            indice = centro;
            break;
        } else if(vertice < G->nodo[centro].nombre){ 
            sup = centro-1;
        } else {
            inf = centro+1;
        }
    }
    return indice;
}

static void swapNodo(struct Nodo *i, struct Nodo *j){
    struct Nodo t = *i;
    *i = *j;
    *j = t;
}

static u32 partitionNodo(struct Nodo a[], u32 left, u32 right) {
    u32 m = (left + right)/2;
    swapNodo (&a[left], &a[m]);
    u32 pivot = a[left].nombre;
    u32 lo = left + 1;
    u32 hi = right;
    while(lo <= hi){
        while(a[hi].nombre > pivot){
            hi = hi - 1;
        } while(lo <= hi && a[lo].nombre<=pivot){
            lo = lo + 1;
        } if(lo<=hi){
            swapNodo(&a[lo], &a[hi]);
            lo = lo + 1;
            hi = hi - 1;
        }
    }
    swapNodo(&a[left], &a[hi]);
    return(hi);
}

static void quickSortRecNodo(struct Nodo a[], u32 izq, u32 der) {
    if(der > izq){
        u32 ppiv = partitionNodo(a, izq, der);
        if (ppiv == 0){
            quickSortRecNodo(a, ppiv+1, der);
        } else {
            quickSortRecNodo(a, ppiv+1, der);
            quickSortRecNodo(a, izq, ppiv-1);
        }
    }
}

static void sortNodo(struct Nodo a[], u32 length) {
    quickSortRecNodo(a, 0, (length == 0) ? 0 : length-1);
}

/*

//Opcional si se quiere la lista de vecinos ordenada naturalmente.    

static void swapVecino(u32 *i, u32 *j){
    u32 t = *i;
    *i = *j;
    *j = t;
}

static u32 partitionVecinos(u32 a[], u32 left, u32 right) {
    u32 m = (left + right)/2;
    swapVecino(&a[left], &a[m]);
    u32 pivot = a[left];
    u32 lo = left + 1;
    u32 hi = right;
    while(lo <= hi){
        while(a[hi] > pivot){
            hi = hi -1;
        }
        while(lo <= hi && a[lo]<=pivot){
            lo = lo +1;
        }
        if(lo<=hi){
            swapVecino(&a[lo], &a[hi]);
            lo = lo+1;
            hi = hi - 1;
        }
    }
    swapVecino(&a[left], &a[hi]);
    return(hi);
}

static void quickSortRecVecinos(u32 a[], u32 izq, u32 der) {
    if(der > izq){
        u32 ppiv = partitionVecinos(a, izq, der);
        if (ppiv == 0){
            quickSortRecVecinos(a, ppiv+1, der);
        } else {
            quickSortRecVecinos(a, ppiv+1, der);
            quickSortRecVecinos(a, izq, ppiv-1);
        }
    }
}

static void sortVecinos(u32 a[], u32 length) {
    quickSortRecVecinos(a, 0, (length == 0) ? 0 : length-1);
}

*/

void ordenarGrafo(GrafoSt* G){
    destruirDatosDeCarga(G);
    sortNodo(G->nodo, G->vertices);
    for(u32 i=0; i < G->vertices; i++){
        u32* direccion = realloc(G->nodo[i].vecinos, sizeof(u32)*(G->nodo[i].grado));
        if(direccion != NULL){
            G->nodo[i].vecinos = direccion;
        }
        for(u32 j=0; j < G->nodo[i].grado; j++){
            u32 nombre = G->nodo[i].vecinos[j];
            u32 modulo = nombre % G->vertices;
            if (nombre == G->nodo[modulo].nombre){
                G->nodo[i].vecinos[j] = modulo;
            } else {
                G->nodo[i].vecinos[j] = indiceVertice(G, G->vertices, nombre);
            }
        }
        //sortVecinos(G->nodo[i].vecinos, G->nodo[i].grado);
    } 
}

u32 getVerticesContados(GrafoSt* G){
    return G->aux->vertices_contados;
}

u32 getVerticesTotales(GrafoSt* G){
    return G->vertices;
}

u32 getMaximoLados(GrafoSt* G){
    return G->lados;
}

u32 getDelta(GrafoSt* G){
    return G->delta;
}

u32 getNombre(u32 i, GrafoSt* G){
    return G->nodo[i].nombre;
}

u32 getGrado(u32 i, GrafoSt* G){
    u32 grado = pow(2,32)-1;
    if (i < G->vertices){
        grado = G->nodo[i].grado;
    }
    return grado;
}

u32 getIndiceVecino(u32 j, u32 i, GrafoSt* G){
    u32 indice = pow(2,32)-1;
    if (i < G->vertices && j < G->nodo[i].grado){
        indice = G->nodo[i].vecinos[j];
    }
    return indice; 
}

void printGrafo(GrafoSt* G){
    for(u32 i=0; i < G->vertices; i++){
        printf("Nombre: %u, Grado: %u, Vecinos: ", G->nodo[i].nombre, G->nodo[i].grado);
        for(u32 j=0; j < G->nodo[i].grado; j++){
            printf("%u, ", G->nodo[i].vecinos[j]);
        }    
        printf("\n");
    }
}