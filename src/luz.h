#ifndef LUZ_H
#define LUZ_H

#include <stdbool.h>
#include "arreglo.h"
#include "vector.h"
#include "color.h"


typedef struct {
    vector_t posicion; // Si es_puntual es una coordenada, si no una dirección
    color_t color;
    bool es_puntual;
} luz_t;

void luz_destruir(void *luz);
luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual);
arreglo_t luces_generar();

#endif
