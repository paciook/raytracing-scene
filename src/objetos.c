#include "objetos.h"

objeto_t *objeto_crear(); // completar

float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    return 0;
}

void objeto_destruir(objeto_t *objeto);

arreglo_t objetos_generar(char *nombre_archivo);

// Esferas

typedef struct esfera{
    vector_t centro;
    float radio;
} esfera_t;

vector_t e_normal(esfera_t *cuerpo, vector_t p){
    vector_t n = vector_normalizar(vector_resta(p,cuerpo->centro));
    return n;
}

// Planos

typedef struct {
    vector_t normal;
    vector_t p;
} plano_t;

vector_t p_normal(plano_t *cuerpo, vector_t p){
    return cuerpo->normal;
}

// Triangulos

typedef struct {
    vector_t vertices[3];
    vector_t normal;
} triangulo_t;

vector_t t_normal(triangulo_t *cuerpo, vector_t p){
    return cuerpo->normal;
}

// Mallas

typedef struct {
    triangulo_t *triangulos;
    size_t n; // Número de triángulos
} malla_t;

vector_t m_normal(malla_t *cuerpo, vector_t p);

// Tablas de búsqueda

typedef vector_t (*cuerpo_normal_t)(void*, vector_t);

cuerpo_normal_t cuerpo_normal[] = {
    [ESF] = &e_normal,
    [PLANO] = &p_normal,
    [TRIANG] = &t_normal,
    [MALLA] = &m_normal,
};