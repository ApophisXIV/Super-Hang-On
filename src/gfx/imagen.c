/**
 * @file imagen.c
 * @author Guido Rodriguez (guerodriguez@fi.uba.ar)
 * @brief Descripcion
 * @version 1.0
 * @date YYYY-MM-DD
 *
 * @copyright Copyright (c) 2021
 *
 */

/* -------------------------------- Includes -------------------------------- */
#include "../../inc/imagen.h"
#include "../../inc/config.h"
#include <stdlib.h>
#include <string.h>

/* ------------------ Private data structures and typedefs ------------------ */
struct imagen {
    pixel_t **pixeles;
    size_t ancho, alto;
};

/* -------------------- Private prototypes implementation ------------------- */
static imagen_t *_imagen_crear(size_t ancho, size_t alto) {

    imagen_t *nueva_im = malloc(sizeof(imagen_t));
    if (nueva_im == NULL) return NULL;

    nueva_im->ancho = ancho;
    nueva_im->alto  = alto;

    nueva_im->pixeles = malloc(alto * sizeof(pixel_t *));
    if (nueva_im->pixeles == NULL) {
        free(nueva_im);
        return NULL;
    }

    for (size_t i = 0; i < alto; i++) {
        nueva_im->pixeles[i] = malloc(ancho * sizeof(pixel_t));
        if (nueva_im->pixeles[i] == NULL) {
            imagen_destruir(nueva_im);
            return NULL;
        }
    }

    return nueva_im;
}

/* -------------------- Public prototypes implementation -------------------- */
// Constructor
imagen_t *imagen_generar(size_t ancho, size_t alto, pixel_t valor) {
    imagen_t *im = _imagen_crear(ancho, alto);
    if (im == NULL) return NULL;
    for (size_t f = 0; f < im->alto; f++)
        for (size_t c = 0; c < im->ancho; c++)
            im->pixeles[f][c] = valor;
    return im;
}

// Destructor
void imagen_destruir(imagen_t *imagen) {
    for (size_t i = 0; i < (imagen->alto); i++) free(imagen->pixeles[i]);
    free(imagen->pixeles);
    free(imagen);
}

// Getters
size_t imagen_get_ancho(const imagen_t *im) {
    return im->ancho;
}
size_t imagen_get_alto(const imagen_t *im) {
    return im->alto;
}

// Setters
pixel_t imagen_get_pixel(const imagen_t *im, size_t x, size_t y) {
    return im->pixeles[y][x];
}

bool imagen_set_pixel(const imagen_t *im, size_t x, size_t y, pixel_t p) {
    if (x >= im->ancho || y >= im->alto) return false;
    im->pixeles[y][x] = p;
    return true;
}

// Utils
void imagen_pegar(imagen_t *destino, const imagen_t *origen, int x, int y) {
    for (int f = y >= 0 ? 0 : -y; f < origen->alto && f + y < destino->alto; f++)
        for (int c = x >= 0 ? 0 : -x; c < origen->ancho && c + x < destino->ancho; c++)
            if (origen->pixeles[f][c] != 0)
                destino->pixeles[f + y][c + x] = origen->pixeles[f][c];
}

imagen_t *imagen_espejar(imagen_t *o) {
    imagen_t *reflejada = _imagen_crear(o->ancho, o->alto);
    if (reflejada == NULL) return NULL;

    for (size_t i = 0; i < (o->alto); i++)
        for (size_t j = 0; j < (o->ancho); j++)
            reflejada->pixeles[i][j] = o->pixeles[i][o->ancho - 1 - j];

    return reflejada;
}

void imagen_pegar_con_paleta(imagen_t *destino, const imagen_t *origen, int x, int y, const pixel_t paleta[], bool strip_mode, size_t strip) {
    if (strip > origen->alto) return;

    for (int f = y >= 0 ? 0 : -y; f < origen->alto && f + y < destino->alto; f++) {
        for (int c = x >= 0 ? 0 : -x; c < origen->ancho && c + x < destino->ancho; c++) {
            if (origen->pixeles[strip_mode ? strip : f][c] != 0)
                destino->pixeles[strip_mode ? y : f + y][c + x] = paleta[origen->pixeles[strip_mode ? strip : f][c]];
        }
    }
}

imagen_t *imagen_escalar(const imagen_t *origen, size_t ancho_destino, size_t alto_destino) {
    imagen_t *destino = _imagen_crear(ancho_destino, alto_destino);
    if (destino == NULL)
        return NULL;

    double prop_alto  = (origen->alto) / (double)alto_destino;
    double prop_ancho = (origen->ancho) / (double)ancho_destino;

    for (size_t i = 0; i < (alto_destino); i++)
        for (size_t j = 0; j < (ancho_destino); j++)
            (destino->pixeles)[i][j] = (origen->pixeles)[(size_t)(i * prop_alto)][(size_t)(j * prop_ancho)];

    return destino;
}

imagen_t *generar_mosaico(const imagen_t *teselas[], const pixel_t paleta[][8], size_t filas, size_t columnas,
                          const uint16_t mosaico_teselas[filas][columnas],
                          const uint8_t mosaico_paletas[filas][columnas],
                          bool hex_str_mode, const char *hex_str) {

    imagen_t *mosaico = imagen_generar(ANCHO_TESELA * columnas, ALTO_TESELA * filas, 0);
    if (mosaico == NULL) return NULL;

    size_t c_hex = 0;
    for (size_t y = 0; y < filas; y++)
        for (size_t x = 0; x < columnas; x++) {
            if (hex_str_mode) {
                char hex[2] = {hex_str[c_hex], hex_str[c_hex + 1]};
                imagen_pegar(mosaico, teselas[strtol(hex, NULL, 16)], x * ANCHO_TESELA, y * ALTO_TESELA);
                c_hex += 2;
            } else
                imagen_pegar_con_paleta(mosaico, teselas[mosaico_teselas[y][x]], x * 8, y * 8, paleta[mosaico_paletas[y][x]], NO_STRIP_MODE, 0);
        }

    return mosaico;
}

void imagen_a_textura(const imagen_t *im, uint16_t *v) {
    for (size_t i = 0; i < (im->alto); i++)
        memcpy(v + i * (im->ancho), im->pixeles[i], im->ancho * sizeof(pixel_t));
}
