#include <stdbool.h>
#include <stdlib.h>
#include "arreglo.h"

bool arreglo_agregar(arreglo_t *a, void *e){
    void *aux = realloc((a->v), ((a->n) + 1)*sizeof(void*));
    if(aux==NULL){   
        return false;
    }

    a->v = aux;
    (a->v)[a->n] = e;
    (a->n)++;
    return true;
}

void arreglo_liberar(arreglo_t *a, void (*destruir)(void*)){
    for(size_t i = 0; i < a->n; i++)
        destruir((a->v)[i]);
    free(a->v);
}

