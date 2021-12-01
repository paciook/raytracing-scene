#ifndef OBJETOS_H
#define OBJETOS_H

#include "vector.h"

typedef struct {
    void *cuerpo;
    int tipo;
    float kd,ka,ks,kr; // ks especular, kr reflexion
} objeto_t;

objeto_t *objeto_crear(); // completar
float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal);
void objeto_destruir(objeto_t *objeto);

#endif