#include "imagen.h"

#define BMASK 0xFF
#define II 255

struct imagen {
    color_t **lienzo;
    size_t ancho, alto;
    size_t profundidad;
};

imagen_t *imagen_crear(size_t ancho, size_t alto) {
    imagen_t *im = malloc(sizeof(imagen_t));
    if(im == NULL) return NULL;

    im->lienzo = malloc(alto * sizeof(color_t *));
    if(im->lienzo == NULL) {
        free(im);
        return NULL;
    }

    for(size_t i = 0; i < alto; i++) {
        im->lienzo[i] = malloc(ancho * sizeof(color_t));
        if(im->lienzo[i] == NULL) {
            imagen_destruir(im);
            return NULL;
        }
    }

    im->ancho = ancho;
    im->alto = alto;

    return im;
}

void imagen_destruir(imagen_t *im) {
    for(size_t i = 0; i < im->alto; i++)
        free(im->lienzo[i]);
    free(im->lienzo);
    free(im);
}

void imagen_dimensiones(const imagen_t *im, size_t *ancho, size_t *alto) {
    *ancho = im->ancho;
    *alto = im->alto;
}


bool imagen_set_pixel(imagen_t *im, size_t x, size_t y, color_t color) {
    if(x >= im->ancho || y >= im->alto)
        return false;

    im->lienzo[y][x] = color;
    return true;
}

color_t imagen_get_pixel(const imagen_t *im, size_t x, size_t y) {
    return im->lienzo[y][x];
}

int ftopixl(float f){
    return ((f > 1) ? 1 :f) * II;
}

void escribir_int16_little_endian(FILE *f, int16_t v){
    for(size_t c = 0; c < 16; c+=8){
        int8_t aux = (v >> c) & BMASK;
        fwrite(&aux, sizeof(int8_t), 1, f);
    }
}

void escribir_int32_little_endian(FILE *f, int32_t v){
    for(size_t c = 0; c < 32; c+=8){
        int8_t aux = (v >> c) & BMASK;
        fwrite(&aux, sizeof(int8_t), 1, f);
    }
}

void escribir_PPM(const imagen_t *imagen, FILE *f){
    size_t ancho,alto;
    imagen_dimensiones(imagen, &ancho, &alto);

    fprintf(f, "P3\n%zu %zu\n255\n", ancho, alto);

    // encabezado escrito
    
    for(size_t i = 0; i < alto; i++){
        for (size_t j = 0; j < ancho; j++){
            color_t p = imagen_get_pixel(imagen, j, i);
            fprintf(f, "%d %d %d ", ftopixl(p.r),ftopixl(p.g),ftopixl(p.b));
        }
    }
}

void escribir_BMP(imagen_t *imagen, FILE *f){
    // escribo el encabezado del archivo
    fwrite("BM", 1, 2, f);

    size_t ancho, alto, extra;
    imagen_dimensiones(imagen, &ancho, &alto);
    {
        uint32_t tamanoEncabezadoA = 14;
        uint32_t tamanoEncabezadoI = 40;
        
        size_t anchoPixel = ancho * 3;
        extra = 4 - (anchoPixel % 4);
        extra = (extra - 4) ? extra : 0;
        size_t scanline = extra ? anchoPixel + extra : anchoPixel;
        uint32_t tamanoPixeles = alto * scanline;

        uint32_t tamanoArchivo = tamanoEncabezadoA + tamanoEncabezadoI + tamanoPixeles;

        escribir_int32_little_endian(f, tamanoArchivo); //tamano del archivo
        escribir_int16_little_endian(f, 0); // reservado
        escribir_int16_little_endian(f, 0); // reservado
        escribir_int32_little_endian(f, 54); //tamano del offset
    }

    
    // escribo encabezado de la imagen
    {
        escribir_int32_little_endian(f, 40); // tamano
        escribir_int32_little_endian(f, ancho);
        escribir_int32_little_endian(f, alto);
        escribir_int16_little_endian(f, 1); // planos
        escribir_int16_little_endian(f, 24); // bits de color
        escribir_int32_little_endian(f,0); // compresion
        escribir_int32_little_endian(f,0); // tamano de imagen
        escribir_int32_little_endian(f,0); // res x
        escribir_int32_little_endian(f,0); // res y
        escribir_int32_little_endian(f,0); // tablas de color
        escribir_int32_little_endian(f,0); // colores importantes
    }


    // escribo los pixeles
    int8_t cero = 0;
    for(size_t y = alto -1; y > 0; y--){
        for(size_t x = 0; x < ancho; x++){
            color_t c = imagen_get_pixel(imagen, x, y);
            uint8_t p[] = {ftopixl(c.b), ftopixl(c.g), ftopixl(c.r)};
            fwrite(p, sizeof(int8_t), 3, f);
        }
        for(size_t i = 0; i < extra; i++)
            fwrite(&cero, sizeof(int8_t), 1, f);
    }
}


bool imagen_imprimir(char n[], bool isBin, imagen_t *img){
    FILE *f;
    typedef void (*impresion_t)(imagen_t*, FILE*);
    impresion_t imprimir;

    if(isBin){
        f = fopen(n, "wb");
        if(f == NULL) return false;

        imprimir = (impresion_t)escribir_BMP;
    } else {
        f = fopen(n, "w");
        if(f == NULL) return false;

        imprimir = (impresion_t)escribir_PPM;
    }

    imprimir(img, f);

    fclose(f);

    return true;
}
