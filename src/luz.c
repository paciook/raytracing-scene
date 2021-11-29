#include "luz.h"
#include "color.h"
#include "vector.h"

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual){
    luz_t *l = malloc(sizeof(luz_t));
    if(l == NULL)
        return NULL;

    l->posicion = posicion;
    l->color = color;
    l->es_puntual = es_puntual;

    return l;
}

void luz_destruir(luz_t *luz){
    free(luz);
}
