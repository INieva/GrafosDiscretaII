#include <string.h>
#include <math.h>

#include "APIG23.h"
#include "EstructuraGrafo23.h"

Grafo ConstruirGrafo(void){
    Grafo grafo = NULL;
    u32 vertices = 0;
    u32 lados = 0;
    u32 vertice_dest = 0;
    u32 vertice_src = 0;
    u32 lados_leidos = 0;
    char line[1024];

    while (fgets(line, 1024, stdin)){
        if (line[0] == 'p'){
            char edge[1024];
            if(sscanf(line, "%*c %s %u %u", edge, &vertices, &lados) == 3){
                if (!strcmp(edge, "edge") && grafo == NULL){
                    grafo = nuevoGrafo(vertices, lados);
                } else { break; }
            }else{ break; }
        } else if(line[0]=='e' ){
            if(sscanf(line, "%*c %u %u", &vertice_src, &vertice_dest) == 2 && grafo != NULL){
                if (getVerticesContados(grafo) <= vertices){
                    u32 indice_src = agregaVertice(grafo, vertice_src);
                    u32 indice_dest = agregaVertice(grafo, vertice_dest);
                    if(indice_src != pow(2,32)-1 && indice_dest != pow(2,32)-1){
                        agregarLado(grafo, indice_src, vertice_dest);
                        agregarLado(grafo, indice_dest, vertice_src);
                    } else { break; }
                } else { break; } 
            } else { break; }
            lados_leidos++;
            if (lados_leidos == lados){ break; }            
        }
    }
    if (grafo != NULL){
        if(lados_leidos == lados){
            ordenarGrafo(grafo);
        } else {
            destruirGrafoSt(grafo);
            grafo = NULL;
        }
    }    
    return grafo;
} 

void DestruirGrafo(Grafo G){
    destruirGrafoSt(G);
}

u32 NumeroDeVertices(Grafo G){
    return getVerticesTotales(G);
}

u32 NumeroDeLados(Grafo G){
    return getMaximoLados(G);
}

u32 Delta(Grafo G){
    return getDelta(G);
}

u32 Nombre(u32 i,Grafo G){
    return getNombre(i,G);
}

u32 Grado(u32 i,Grafo G){
    return getGrado(i, G);
}

u32 IndiceVecino(u32 j,u32 i,Grafo G){
    return getIndiceVecino(j, i, G);
}