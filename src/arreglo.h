#ifndef ARREGLO_H
#define ARREGLO_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void **v;
    size_t n;
} arreglo_t;

bool arreglo_agregar(arreglo_t *a, void *e);
void arreglo_liberar(arreglo_t *a, void (*destruir)(void*));

#endif
