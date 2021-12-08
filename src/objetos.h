#ifndef OBJETOS_H
#define OBJETOS_H

#include "vector.h"
#include "arreglo.h"
#include "color.h"
#include <float.h>

#define INFINITO FLT_MAX
#define EPS 0.00001


typedef enum {ESF, PLANO, TRIANG, MALLA} tipo_t;

typedef struct {
    void *cuerpo;
    tipo_t tipo;
    float kd,ka,ks,kr;
    color_t color;
} objeto_t;

objeto_t *objeto_crear(void*c, tipo_t t, float kd, float ka, float ks, float kr, color_t color); // completar
float objeto_distancia(objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal);
void objeto_destruir(void *objeto);
arreglo_t objetos_generar(char *nombre_archivo);

#endif
