#include "color.h"

#include <stdio.h>
#define II 255

int ftoppm(float f){
    return ((f > 1) ? 1 :f) * II;
}

color_t color_sumar(color_t c, color_t m, float p){
    return (color_t){c.r + p*(m.r), c.g + p*(m.g), c.b + p*(m.b)};
}

color_t color_absorber(color_t b, color_t c){
    return (color_t){b.r*c.r, b.g*c.g, b.b*c.b};
}

void color_imprimir(color_t c){
    printf("%d %d %d ", ftoppm(c.r),ftoppm(c.g),ftoppm(c.b));
}
