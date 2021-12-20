#include "objetos.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

// Esferas

typedef struct esfera{
    vector_t centro;
    float radio;
} esfera_t;

esfera_t *esfera_crear(vector_t centro, float radio){
    esfera_t *e = malloc(sizeof(esfera_t));

    if(e == NULL)
        return NULL;

    e->centro = centro;
    e->radio = radio;

    return e;
}

float _e_distancia(void *cuerpo, vector_t o, vector_t d,
                        vector_t *punto, vector_t *normal){
    esfera_t *esfera = cuerpo;
    float t = INFINITO;
    
	float disc = 0;
    
    vector_t res = vector_resta(esfera->centro,o);
    float p_resd = vector_producto_interno(res,d);
    float rcuad = (esfera->radio)*(esfera->radio);
	disc = (p_resd*p_resd) - vector_producto_interno(res,res) + rcuad;
    
    if(disc >= 0){
        float sq = sqrt(disc);
        t = p_resd + ((p_resd <= sq) ? sq : -sq);
    }

    if(t <= 0){
        return INFINITO;
    }

    if(punto != NULL)
        *punto = vector_interpolar_recta(o,d,t);
    if(normal != NULL)
        *normal = vector_normalizar(vector_resta(*punto,esfera->centro));

    return t;
}

// Planos

typedef struct {
    vector_t n;
    vector_t p;
} plano_t;

float _p_distancia( void *cuerpo, vector_t o, vector_t d,
                    vector_t *punto, vector_t *normal){
    
    plano_t *plano = cuerpo;

    float prod_esc_dn = vector_producto_interno(d,plano->n);
    if(prod_esc_dn == 0) return INFINITO;

    float t = vector_producto_interno(vector_resta(plano->p,o), plano->n) / prod_esc_dn;
    return t;
}


// Triangulos

typedef struct {
    vector_t vertices[3];
    vector_t normal;
} triangulo_t;

float _t_distancia( void *cuerpo, vector_t o, vector_t d,
                    vector_t *punto, vector_t *normal){
    
    triangulo_t *triang = cuerpo;

    vector_t e1,e2;
    e1 = vector_resta(triang->vertices[1], triang->vertices[0]);
    e2 = vector_resta(triang->vertices[2], triang->vertices[0]);

    vector_t h = vector_producto_vectorial(d, e2);
    float a = vector_producto_interno(e1, h);

    if(a > -EPS && a < EPS) return INFINITO; // D es paralelo al triangulo

    vector_t s = vector_resta(o,triang->vertices[0]);
    float u = vector_producto_interno(s,h) / a;    

    if(u < 0 || u > 1) return INFINITO; // No hay interseccion

    vector_t q = vector_producto_vectorial(s,e1);
    float v = vector_producto_interno(d,q) / a;

    if(v < 0 || u + v > 1) return INFINITO; // No hay interseccion

    float t = vector_producto_interno(e2,q) / a;
    if(t < 0) return INFINITO;

    *normal = triang->normal;
    *punto = vector_interpolar_recta(o,d,t);

    return t;
}


// Mallas

typedef struct {
    triangulo_t *v;
    size_t n; // Número de triángulos
} malla_t;

void _liberar_malla(void *malla){
    malla_t *m = malla;
    free(m->v);
    free(m);
}

float _m_distancia( void *cuerpo, vector_t o, vector_t d,
                    vector_t *punto, vector_t *normal){
    
    malla_t *malla = cuerpo;

    float t = INFINITO;
    vector_t aux_p, aux_n;

    // Hallo el minimo
    for(size_t i = 0; i < malla->n; i++){
        float act = _t_distancia((void*)(&(malla->v[i])),o,d,&aux_p,&aux_n);
        if(act < t){
            t = act;
            *punto = aux_p;
            *normal = aux_n;
        }
    }

    return t;
}

// Tablas de búsqueda

typedef void (*cuerpo_destruir_t)(void*);

cuerpo_destruir_t cuerpo_destruir[] = {
    [(tipo_t)ESF] = &free,
    [(tipo_t)PLANO] = &free,
    [(tipo_t)TRIANG] = &free,
    [(tipo_t)MALLA] = &_liberar_malla,
};

typedef float (*cuerpo_distancia_t)(void*,vector_t o, vector_t d, vector_t *p, vector_t*n);

cuerpo_distancia_t cuerpo_distancia[] = {
    [(tipo_t)ESF] = &_e_distancia,
    [(tipo_t)PLANO] = &_p_distancia,
    [(tipo_t)TRIANG] = &_t_distancia,
    [(tipo_t)MALLA] = &_m_distancia,
};

// Objetos

objeto_t *objeto_crear(void*c, tipo_t t, float kd, float ka, float ks, float kr, color_t color){
    objeto_t *obj = malloc(sizeof(objeto_t));
    if(obj == NULL) return NULL;
    
    obj->cuerpo = c;
    obj->tipo = t;
    obj->kd = kd;
    obj->kr = kr;
    obj->ks = ks;
    obj->ka = ka;
    obj->color = color;

    return obj;
}

float objeto_distancia(objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    cuerpo_distancia_t dist = cuerpo_distancia[objeto->tipo];

    float t = dist(objeto,o,d,punto,normal);

    return t;
}

void objeto_destruir(void *estructura){
    objeto_t *objeto = estructura;
    cuerpo_destruir_t destruir = cuerpo_destruir[objeto->tipo];
    destruir(objeto->cuerpo);
    free(objeto);
}

arreglo_t objetos_generar(char *nombre_archivo){
    arreglo_t objetos = {NULL, 0};

    arreglo_agregar(&objetos, objeto_crear(esfera_crear((vector_t){0, 1, 2.4}, .3), (tipo_t)ESF, 1, 1, 0.16, .33, (color_t){1, 1, 1}));
    /*
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){0, -.4, 3}, 1, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));

    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-2, -.6, 3}, .3, (color_t){1, 0, 0}, 1, .8, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-1.73, -.6, 2}, .3, (color_t){1, 1, 0}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-1, -.6, 1.26}, .3, (color_t){0, 1, 0}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){0, -.6, 1}, .3, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){1, -.6, 1.26}, .3, (color_t){0, 1, 1}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){1.73, -.6, 2}, .3, (color_t){0, 0, 1}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){2, -.6, 3}, .3, (color_t){1, 0, 1}, 1, 1, 0.16, .33)));

    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 0, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .16, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .33, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){0, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .5, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .66, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .83, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));

    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-3, 1.5, 4}, .3, (color_t){1, 1, 1}, 0, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-2, 1.5, 4}, .3, (color_t){1, 1, 1}, .16, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){-1, 1.5, 4}, .3, (color_t){1, 1, 1}, .33, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){0, 1.5, 4}, .3, (color_t){1, 1, 1}, .5, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){1, 1.5, 4}, .3, (color_t){1, 1, 1}, .66, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){2, 1.5, 4}, .3, (color_t){1, 1, 1}, .83, 1, 0.16, .33)));
    assert(arreglo_agregar(&objetos, esfera_crear((vector_t){3, 1.5, 4}, .3, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));

    assert(objetos.n == 23);
    for(size_t i = 0; i < objetos.n; i++)
        assert(objetos.v[i] != NULL);
    */
    return objetos;
}
