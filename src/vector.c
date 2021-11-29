#include <math.h>
#include "vector.h"

float vector_producto_interno(vector_t a, vector_t b){
    float prod = 0;
    prod += a.x * b.x;
    prod += a.y * b.y;
    prod += a.z * b.z;
    return prod;
}

float vector_norma(vector_t a){
    return sqrt(vector_producto_interno(a,a));
}

vector_t vector_resta(vector_t a, vector_t b){
    return (vector_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vector_t vector_interpolar_recta(vector_t o, vector_t d, float t){
    return (vector_t){o.x + (d.x)*t, o.y + (d.y)*t, o.z + (d.z)*t};
}

vector_t vector_normalizar(vector_t a){
    float m = vector_norma(a);
    return (vector_t){a.x/m, a.y/m, a.z/m};
}
