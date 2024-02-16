#include "APIG23.h"
#include "APIParte2.h"

int main(){
    Grafo grafo = ConstruirGrafo();
    
    if(grafo != NULL){
        u32 colores_imparPar;
        u32 colores_ordenJedi;
        char exito_impar_par;
        char exito_orden_jedi;
        u32 n = NumeroDeVertices(grafo);
        u32* orden_1 = malloc(sizeof(u32)*n);
        u32* orden_2 = malloc(sizeof(u32)*n);
        u32* color_1 = malloc(sizeof(u32)*n);
        u32* color_2 = malloc(sizeof(u32)*n);
        
        if(orden_1!=NULL && orden_2!=NULL && color_1 != NULL && color_2 != NULL){
            for(u32 i=0; i<n; i++){
                orden_1[i] = i;
                orden_2[i] = i;
            }
            colores_imparPar = Greedy(grafo, orden_1, color_1);
            for(u32 i=0; i<n; i++){
                color_2[i] = color_1[i];
            }
            u32 invertir = 0;
            u32 ronda = 0;
            for(u32 i=0; i<500; i++){
                if(ronda >= 16){
                    if(invertir == 0){ invertir=1; }
                    else{ invertir=0; }
                    ronda=0;
                }
                if(invertir == 0){
                    exito_impar_par = OrdenImparPar(n, orden_1, color_1);
                    colores_imparPar = Greedy(grafo, orden_1, color_1);
                    exito_orden_jedi = OrdenJedi(grafo, orden_2, color_2);
                    colores_ordenJedi = Greedy(grafo, orden_2, color_2);
                }else{
                    exito_impar_par = OrdenImparPar(n, orden_2, color_2);
                    colores_imparPar = Greedy(grafo, orden_2, color_2);
                    exito_orden_jedi = OrdenJedi(grafo, orden_1, color_1);
                    colores_ordenJedi = Greedy(grafo, orden_1, color_1);
                }
                ronda ++;
            }
            printf("Colores ImparPar: %u \n", colores_imparPar);
            printf("Colores OrdenJedi: %u \n", colores_ordenJedi);
            printf("Exito ImparPar: %c \n", exito_impar_par);
            printf("Exito OrdenJedi: %c \n", exito_orden_jedi);
        }       
        if(orden_1 != NULL){ free(orden_1); }
        if(orden_2 != NULL){ free(orden_2); }
        if(color_1 != NULL){ free(color_1); }
        if(color_2 != NULL){ free(color_2); }

        DestruirGrafo(grafo);
    } 
    return 0;
} 
