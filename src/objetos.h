#ifndef OBJETOS_H
#define OBJETOS_H

#include "vector.h"
#include "arreglo.h"
#include "color.h"

typedef enum {ESF, PLANO, TRIANG, MALLA} tipo_t;

typedef struct {
    void *cuerpo;
    tipo_t tipo;
    float kd,ka,ks,kr; // ks especular, kr reflexion
    color_t color;
} objeto_t;

objeto_t *objeto_crear(); // completar
float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal);
void objeto_destruir(objeto_t *objeto);
arreglo_t objetos_generar(char *nombre_archivo);

#endif
