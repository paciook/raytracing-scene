#include "arreglo.h"
#include "color.h"
#include "vector.h"
#include "luz.h"
#include "objetos.h"

#include <float.h>
#include <stdio.h>
#define INFINITO FLT_MAX

color_t fondo = {0,0,0};

vector_t computar_direccion_rebote(vector_t d, vector_t n){
    // d - 2N(D.N)
    vector_t r = vector_estirar(n, 2*vector_producto_interno(d,n));
    r = vector_resta(d, r);
    return r;
}

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
    color_t c = {0,0,0}; // Color que voy a devolver
    objeto_t *obj = (objeto_t*)(objetos->v[n_obj]); // Objeto que intersecto
    objeto_distancia(obj,o,d,&p,&n);
    vector_t r = computar_direccion_rebote(d,n);

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
            float factor_angulo;

            // Calculo factor de angulo
            {
                // Termino del angulo de la normal
                float prod_ln = vector_producto_interno(dir_luz,n);
                float termino_normal = obj->kd *  (prod_ln >= 0 ? prod_ln : 0);

                // Termino del angulo del rebote
                float prod_lr = vector_producto_interno(dir_luz,r);
                float termino_rebote = obj->ks * (prod_lr >= 0 ? prod_lr : 0);

                factor_angulo = termino_normal + termino_rebote;
            }
            color_t abs = color_absorber(luz->color, obj->color);

            // Sumo el color de la luz
            c = color_sumar(c, abs, factor_angulo);
        }
    }
    
    // Sumo la luz ambiente
    c = color_sumar(c, ambiente, objeto->ka);
    
    // Sumo el color recursivo
    color_t color_rebote = computar_intensidad(profundidad - 1, objetos, luces, ambiente, p, r);
    color_sumar(c, color_rebote, kr);

    return c;
}

int main(void){

    printf("Hello world!");
    return 0;
}