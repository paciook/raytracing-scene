#define INFINITO FLT_MAX

color_t fondo = {0,0,0};

color_t computar_intensidad(int profundidad, const arreglo_t *objetos, const arreglo_t *luces, color_t ambiente, vector_t o, vector_t d){
    if(!profundidad) return (color_t){0,0,0};


    size_t n_obj;
    float t = INFINITO;
    for(size_t i = 0; i < objetos->n; i++){
        ant = objeto_distancia(objetos->v[i],o,d,NULL,NULL);
        if(ant<t){
            t = ant;
            n_obj = i;
        }
    }

    if(t == INFINITO){
        return fondo;
    }

    vector_t p,n;
    objeto_t *obj = (objeto_t*)(objetos->[n_obj]);
    objeto_distancia(obj,o,d,&p,&n);

    for(size_t i = 0; i < luces->n; i++){
        luz_t *lux = (luz_t*)(luces->v[i]);

        float dist_luz;
        vector_t l;
        if(luz->es_puntual){
            vector_t p_to_l = vector_resta(luz->posicion,p);
            l_luz = vector_normalizar(p_to_l);
            dist_luz = vector_norma(p_to_l);
        } else {
            l_luz = luz->posicion;
            dist_luz = INFINITO;
        }
        vector_t p_luz,n_luz;
        for(size_t j = 0; j < objetos->n; j++){
            if(j == n_obj) continue;
            if(objeto_distancia(objetos->v[j],p,l_luz,p_luz,n_luz) < dist_luz) break;
        }

        // Calcular el color - ambiente
    }

    return (color_t){0,0,0};
}
