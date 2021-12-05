#include "objetos.h"
#include <stdlib.h>

// Esferas

typedef struct esfera{
    vector_t centro;
    float radio;
} esfera_t;

vector_t e_normal(void *cuerpo, vector_t p){
    esfera_t *c = cuerpo;
    vector_t n = vector_normalizar(vector_resta(p,c->centro));
    return n;
}

// Planos

typedef struct {
    vector_t normal;
    vector_t p;
} plano_t;

vector_t p_normal(void *cuerpo, vector_t p){
    return ((plano_t*)(cuerpo))->normal;
}

// Triangulos

typedef struct {
    vector_t vertices[3];
    vector_t normal;
} triangulo_t;

vector_t t_normal(void *cuerpo, vector_t p){
    return ((triangulo_t*)(cuerpo))->normal;
}

// Mallas

typedef struct {
    triangulo_t *triangulos;
    size_t n; // Número de triángulos
} malla_t;

vector_t m_normal(void *cuerpo, vector_t p){
    return (vector_t){0,0,0};
}

void liberar_malla(void *malla){
    malla_t *m = malla;
    free(m->triangulos);
    free(m);
}

// Tablas de búsqueda

typedef vector_t (*cuerpo_normal_t)(void*, vector_t);

cuerpo_normal_t cuerpo_normal[] = {
    [(tipo_t)ESF] = &e_normal,
    [(tipo_t)PLANO] = &p_normal,
    [(tipo_t)TRIANG] = &t_normal,
    [(tipo_t)MALLA] = &m_normal,
};

typedef void (*cuerpo_destruir_t)(void*);

cuerpo_destruir_t cuerpo_destruir[] = {
    [(tipo_t)ESF] = &free,
    [(tipo_t)PLANO] = &free,
    [(tipo_t)TRIANG] = &free,
    [(tipo_t)MALLA] = &liberar_malla,
};

// Objetos

objeto_t *objeto_crear(); // completar

float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    return 0;
}

void objeto_destruir(objeto_t *objeto){
    cuerpo_destruir_t destruir = cuerpo_destruir[objeto->tipo];
    destruir(objeto);
}

arreglo_t objetos_generar(char *nombre_archivo);