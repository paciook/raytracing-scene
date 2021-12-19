#include "luz.h"

#include <stdlib.h>
#include <assert.h>

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual){
    luz_t *l = malloc(sizeof(luz_t));
    if(l == NULL)
        return NULL;

    l->posicion = posicion;
    l->color = color;
    l->es_puntual = es_puntual;

    return l;
}

arreglo_t luces_generar(){
    arreglo_t luces = {NULL, 0};

    assert(arreglo_agregar(&luces, luz_crear((vector_t){2, 1.4, 4}, (color_t){1, 1, 1}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){-2, 1.4, 4}, (color_t){1, 1, 1}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){0, .5, .5}, (color_t){1, 1, 1}, true)));

    for(size_t i = 0; i < luces.n; i++)
        assert(luces.v[i] != NULL);

    return luces;
}

void luz_destruir(void *luz){
    free(luz);
}
