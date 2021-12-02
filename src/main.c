#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arreglo.h"
#include "color.h"
#include "vector.h"
#include "luz.h"
#include "objetos.h"
#include "imagen.h"

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
    c = color_sumar(c, ambiente, obj->ka);
    
    // Sumo el color recursivo
    color_t color_rebote = computar_intensidad(profundidad - 1, objetos, luces, ambiente, p, r);
    color_sumar(c, color_rebote, obj->kr);

    return c;
}

imagen_t *validar_argumentos(int argc, char *argv[]){
    if(argc != 5) {
        fprintf(stderr, "Uso: %s [ancho] [alto] [profundidad] [nombrearchivo]\n", argv[0]);
        return NULL;
    }

    int ancho,alto;
    if( ((ancho = atoi(argv[1])) <= 0) || ((alto = atoi(argv[2])) <= 0) ){
        fprintf(stderr, "Error en ancho o alto\n");
        return NULL;
    }

    int profundidad;
    if( (profundidad = atoi(argv[3])) <= 0 ){
        fprintf(stderr, "Error en profundidad\n");
        return NULL;
    }

    imagen_t *img = imagen_crear(ancho, alto, profundidad);

    if(img == NULL){
        fprintf(stderr, "Error de memoria\n");
        return NULL;
    }

    return img;
}

int main(int argc, char *argv[]){
    // Valido los parametros de la imagen
    imagen_t *img = validar_argumentos(argc,argv);
    if(img == NULL) return 1;

    // Verifico el modo de impresion
    bool isBinary; // True si bmp, false si ppm, error si ninguno.
    char *nombre_archivo = argv[4];
    {
        size_t i = 0;
        
        for(; nombre_archivo[i]; i++);
        char *ext = &nombre_archivo[i-strlen(".ppm")]; // PPM y BMP comparten longitud
        
        if(strcmp(ext,".bmp") == 0){
            isBinary = true;
        } else if(strcmp(ext,".ppm") == 0){
            isBinary = false;
        } else {
            fprintf(stderr, "Extension incorrecta\n");
            return 4;
        }
    }

    printf("Hello%s!\n", (isBinary)?" Fran" : " World");

    imagen_destruir(img);
    return 0;
}
