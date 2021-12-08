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

    assert(arreglo_agregar(&luces, luz_crear(vector_normalizar((vector_t){0, 1, 0}), (color_t){.2, .2, .2}, false)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){-2, 10, 2.5}, (color_t){.9, 0, 0}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){2, 10, 2.5}, (color_t){0, .9, 0}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){0, 10, 5}, (color_t){0, 0, .9}, true)));

    assert(luces.n == 4);
    for(size_t i = 0; i < luces.n; i++)
        assert(luces.v[i] != NULL);

    return luces;
}

void luz_destruir(void *luz){
    free(luz);
}
