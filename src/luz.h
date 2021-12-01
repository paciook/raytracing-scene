#ifndef LUZ_H
#define LUZ_H

#include <stdbool.h>
#include "vector.h"
#include "color.h"


typedef struct {
    vector_t posicion; // Si es_puntual es una coordenada, si no una dirección
    color_t color;
    bool es_puntual;
} luz_t;

void luz_destruir(luz_t *luz);
luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual);

#endif