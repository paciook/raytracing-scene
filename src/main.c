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

#define EPS 0.01 // Factor de desplazamiento del punto P
#define ALPHA 10 // Exponente del termino especular
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
    objeto_distancia(obj,o,d,&p,&n);
    
    color_t c = {0,0,0}; // Color que voy a devolver
    objeto_t *obj = (objeto_t*)(objetos->v[n_obj]); // Objeto que interseco
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
                float termino_rebote = obj->ks * pow((prod_lr >= 0 ? prod_lr : 0), ALPHA);

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
    vector_interpolar_recta(p,r,EPS); // Desplazo el vector P en direccion al rebote
    color_t color_rebote = computar_intensidad(profundidad - 1, objetos, luces, ambiente, p, r);
    color_sumar(c, color_rebote, obj->kr);

    return c;
}

// Funcion espantosa que valida la entrada y devuelve por interfaz los parametros
imagen_t *validar_argumentos(int argc, char *argv[], size_t *an, size_t *al, size_t *p, char**n, bool *isB){
    if(argc != 5) {
        fprintf(stderr, "Uso: %s [ancho] [alto] [profundidad] [nombrearchivo]\n", argv[0]);
        return NULL;
    }

    if( ((*an = atoi(argv[1])) <= 0) || ((*al = atoi(argv[2])) <= 0) ){
        fprintf(stderr, "Error en ancho o alto\n");
        return NULL;
    }

    if( (*p = atoi(argv[3])) <= 0 ){
        fprintf(stderr, "Error en profundidad\n");
        return NULL;
    }

    char *nombre_archivo = (*n = argv[4]);
    
    // Busco la extension
    size_t i = 0;
    for(; nombre_archivo[i]; i++);
    char *ext = &nombre_archivo[i-strlen(".ppm")]; // PPM y BMP comparten longitud
    
    // Comparo si es valida
    if(strcmp(ext,".bmp") == 0){
        *isB = true;
    } else if(strcmp(ext,".ppm") == 0){
        *isB = false;
    } else {
        fprintf(stderr, "Extension incorrecta\n");
        return NULL;
    }


    imagen_t *img = imagen_crear(*an, *al);

    if(img == NULL){
        fprintf(stderr, "Error de memoria\n");
        return NULL;
    }

    return img;
}

int main(int argc, char *argv[]){
    size_t ancho,alto,prof;
    char *nombre_archivo;
    bool isBinary; // Si es bmp, true. Si es ppm, false. Si ninguna, error

    // Valido los parametros de la imagen
    imagen_t *img = validar_argumentos(argc,argv,&ancho,&alto,&prof,&nombre_archivo, &isBinary);
    if(img == NULL) return 1;


    // Genero los objetos
    arreglo_t *objetos = objetos_generar(nombre_archivo);

    // Genero las luces
    arreglo_t *luces = luces_generar();

    
    // Genero la imagen
    /*
    color_t ambiente = {.05, .05, .05};
    vector_t origen = {0, 0, 0};

    float vz = ancho / 2 / tan(FOV/ 2 * PI / 180);
    int x,y;
    x = y = 0;
    for(int vy = alto / 2; vy > - alto / 2; vy--){
        for(int vx = - ancho / 2; vx < ancho / 2; vx++){
            vector_t d = vector_normalizar((vector_t){vx, vy, vz}));
            color_t p = computar_intensidad(&objetos, &luces, ambiente, origen, d);
            imagen_set_pixel(img, x, y, p);
            x++;
        }
        x = 0;
        y++;
    }
    */

    printf("Hello%s!\n", (isBinary)?" Fran" : " World");

    imagen_destruir(img);
    return 0;
}
