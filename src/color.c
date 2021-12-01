#include "color.h"

#include <stdio.h>
#define II 255

int ftoppm(float f){
    return ((f > 1) ? 1 :f) * II;
}

void color_imprimir(color_t c){
    printf("%d %d %d ", ftoppm(c.r),ftoppm(c.g),ftoppm(c.b));
}
