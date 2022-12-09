#ifndef RUTA_H
#define RUTA_H

#include <stdbool.h>
#include <stddef.h>

struct figura_en_ruta {
    int figura;    // Se deja libertad para definir esto como un enum donde corresponda
    size_t paleta;
    int y;
    bool reflejar;
};

typedef enum {
    P_0,
    P_1,
    P_2,
    P_3,
} road_palette_t;

typedef struct {
    float radio_curva;
    size_t indice_figura;
} ruta_t;

extern const struct figura_en_ruta figuras_en_ruta[];
extern const ruta_t ruta[4200 + 70];

static road_palette_t road_palette = P_0;
static size_t path_traveled        = 0;

#endif
