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

