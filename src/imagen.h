#ifndef IMAGEN_H
#define IMAGEN_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "color.h"

struct imagen;
typedef struct imagen imagen_t;

imagen_t *imagen_crear(size_t ancho, size_t alto);
void imagen_destruir(imagen_t *im);
void imagen_dimensiones(const imagen_t *im, size_t *ancho, size_t *alto);
bool imagen_set_pixel(imagen_t *im, size_t x, size_t y, color_t color);
color_t imagen_get_pixel(const imagen_t *im, size_t x, size_t y);
bool imagen_imprimir(char n[], bool isBin, imagen_t *img);

#endif
