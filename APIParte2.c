#include "APIParte2.h"
#include "APIG23.h"
#include <math.h>

u32 Greedy(Grafo G,u32* Orden,u32* Color){
    u32 n = NumeroDeVertices(G);
    u32 m = Delta(G)+1;
    u32 colores_usados = 1;
    u32* color_usado = malloc(m*sizeof(u32));
    u32* coloreado = calloc(n, sizeof(u32));

    if(color_usado != NULL && coloreado != NULL){
        u32 i = Orden[0];
        Color[i] = 0;
        coloreado[i] = 1;
        for(u32 i=1 ; i < n ; i++){
            u32 indice_vertice = Orden[i];
            u32 cantidad_vecinos = Grado(indice_vertice, G);
            for(u32 j=0; j <= colores_usados+1; j++){
                color_usado[j] = 0;
            }
            u32 color_actual = 0;
            for(u32 j=0; j < cantidad_vecinos ; j++){
                u32 indice_vecino = IndiceVecino(j, indice_vertice, G);
                if(coloreado[indice_vecino] == 1){
                    u32 color_vecino = Color[indice_vecino];
                    color_usado[color_vecino] = 1;
                    if (color_vecino == color_actual){
                        while(color_usado[color_actual] != 0 && 
                                color_actual <= colores_usados){
                            color_actual++;
                        }
                    }
                    if (colores_usados < color_actual){
                        colores_usados++;
                        break;
                    }
                }
            }
            Color[indice_vertice] = color_actual;
            coloreado[indice_vertice] = 1; 
        }
        colores_usados++;
    } else {
        colores_usados = pow(2,32)-1;
    }
    if(coloreado != NULL){ free(coloreado); }
    if(color_usado != NULL){ free(color_usado); }
    return colores_usados;
}

static void swap(u32 *i, u32 *j){
    u32 t = *i;
    *i = *j;
    *j = t;
}

static u32 partitionDoble(u32 *a1, u32 *a2, u32 left, u32 right) {
    u32 m = (left + right)/2;
    swap(&a1[left], &a1[m]);
    swap(&a2[left], &a2[m]);
    u32 pivot = a1[left];
    u32 lo = left + 1;
    u32 hi = right;
    while(lo <= hi){
        while(a1[hi] < pivot){
            hi = hi - 1;
        } 
        while(lo <= hi && a1[lo] >= pivot){
            lo = lo + 1;
        } 
        if(lo <= hi){
            swap(&a1[lo], &a1[hi]);
            swap(&a2[lo], &a2[hi]);
            lo = lo + 1;
            hi = hi - 1;
        }
    }
    swap(&a1[left], &a1[hi]);
    swap(&a2[left], &a2[hi]);
    return hi;
}

static void quickSortRecDoble(u32 *a1, u32 *a2,  u32 izq, u32 der) {
    if(der > izq){
        u32 ppiv = partitionDoble(a1, a2, izq, der);
        if (ppiv == 0){
            quickSortRecDoble(a1, a2, ppiv+1, der);
        } else {
            quickSortRecDoble(a1, a2, ppiv+1, der);
            quickSortRecDoble(a1, a2, izq, ppiv-1);
        }
    }
}

char OrdenImparPar(u32 n,u32* Orden,u32* Color){
    u32 len_inpar = 0;
    
    for(u32 i = 0; i<n; i++){
        Orden[i] = i;
        if (Color[i]%2 == 1){
            swap(&Color[i], &Color[len_inpar]);
            swap(&Orden[i], &Orden[len_inpar]);
            len_inpar++;
        }
    }
    quickSortRecDoble(Color, Orden, 0, (len_inpar == 0) ? 0 : len_inpar-1);
    quickSortRecDoble(Color, Orden, len_inpar, (n == 0) ? 0 : n-1);
     
    return '0';
}

char OrdenJedi(Grafo G,u32* Orden,u32* Color){
    char r = '0';
    u32 n = NumeroDeVertices(G); 
    u32 maxColor = Delta(G)+1;
    u32 *AcumVertice = malloc((n)*sizeof(u32));
    u32 *ImSetColor = calloc(maxColor, sizeof(u32));

    if(AcumVertice != NULL && ImSetColor != NULL){
        for(u32 i=0; i < n; i++){
            Orden[i] = i;
            u32 color = Color[i];
            ImSetColor[color] = ImSetColor[color] + Grado(i, G)*color;
        }
        for(u32 i = 0; i < n; i++){
            for(u32 j = 0; j < maxColor; j++){
                if(Color[i] == j){ 
                    AcumVertice[i] = ImSetColor[j];
                    break;
                }
            }
        }
        quickSortRecDoble(AcumVertice, Orden, 0, (n== 0) ? 0 : n-1);
    } else {
        r = '1';
    }
    if(ImSetColor != NULL){ free(ImSetColor); }
    if(AcumVertice != NULL){ free(AcumVertice); }
    return r;
} 

