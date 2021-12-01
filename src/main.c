#include "arreglo.h"
#include "color.h"
#include "vector.h"
#include "luz.h"
#include "objetos.h"

#include <float.h>
#include <stdio.h>
#define INFINITO FLT_MAX

color_t fondo = {0,0,0};

color_t computar_intensidad(int profundidad, const arreglo_t *objetos, const arreglo_t *luces, color_t ambiente, vector_t o, vector_t d){
    // Caso base: no mas pasos recursivos restantes.
    if(!profundidad) return (color_t){0,0,0};

    size_t n_obj; // Numero del objeto que intersecto
    float t = INFINITO; // Distancia a la interseccion

    // Obtengo el objeto mas cercano en la direccion en la que miro

    for(size_t i = 0; i < objetos->n; i++){
        float distancia_anterior = objeto_distancia(objetos->v[i],o,d,NULL,NULL);
        if(distancia_anterior < t){
            t = distancia_anterior;
            n_obj = i;
        }
    }

    if(t == INFINITO){
        return fondo;
    }

    // Guardo el punto de interseccion y la normal correspondientes
    
    vector_t p,n;
    objeto_t *obj = (objeto_t*)(objetos->v[n_obj]); // Objeto que intersecto
    objeto_distancia(obj,o,d,&p,&n);

    // Verifico cada luz en ese punto

    for(size_t i = 0; i < luces->n; i++){
        luz_t *luz = (luz_t*)(luces->v[i]);

        vector_t dir_luz;
        float distancia_luz;

        // Calculo la direccion de la luz segun es_puntual
        if(luz->es_puntual){
            vector_t pl = vector_resta(luz->posicion,p);
            dir_luz = vector_normalizar(pl);
            distancia_luz = vector_norma(pl);
        } else {
            dir_luz = luz->posicion;
            distancia_luz = INFINITO;
        }

        bool hay_sombra = false; // No hay sombra hasta que demuestre lo contrario!
        
        // Compruebo si hay sombra sobre esa luz
        for(size_t j = 0; j < objetos->n; j++){
            if(j == n_obj) continue;

            float distancia_objeto = objeto_distancia(objetos->v[j],p,dir_luz,NULL,NULL);
            if( (hay_sombra = (distancia_objeto < distancia_luz)) ) break; // If true: tengo sombra de esa luz 
        }

        if(!hay_sombra){
            // Agrego el color de la luz en cuestion
        }
    }
    // Sumo el color recursivo
    // Sumo la luz ambiente

    return (color_t){0,0,0};
}

int main(void){

    printf("Hello world!");
    return 0;
}