#ifndef IMAGEN_H
#define IMAGEN_H

/**
 * @file imagen.h
 * @author Guido Rodriguez (guerodriguez@fi.uba.ar)
 * @brief
 * @version 0.1
 * @date 2022-12-03
 *
 * @copyright Copyright (c) 2022. All rights reserved.
 *
 * Licensed under the MIT License, see LICENSE for details.
 * SPDX-License-Identifier: MIT
 *
 */

/* -------------------------------- Includes -------------------------------- */
#include "pixel.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------------- Public data structures and typedefs ------------------ */
typedef struct imagen imagen_t;

/* ------------------------------- Constructor ------------------------------ */
imagen_t *imagen_generar(size_t ancho, size_t alto, pixel_t valor);

/* ------------------------------- Destructor ------------------------------- */
void imagen_destruir(imagen_t *imagen);

/* ----------------------- Public constants and macros ---------------------- */
#define STRIP_MODE    1
#define NO_STRIP_MODE 0

/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
size_t imagen_get_ancho(const imagen_t *im);
size_t imagen_get_alto(const imagen_t *im);
pixel_t imagen_get_pixel(const imagen_t *im, size_t x, size_t y);

// Setters
bool imagen_set_pixel(const imagen_t *im, size_t x, size_t y, pixel_t p);

// Utils
void imagen_pegar(imagen_t *destino, const imagen_t *origen, int x, int y);
void imagen_pegar_con_paleta(imagen_t *destino, const imagen_t *origen, int x, int y, const pixel_t paleta[], bool strip_mode, size_t strip);

void imagen_a_textura(const imagen_t *im, uint16_t *v);
imagen_t *generar_mosaico(imagen_t *teselas[], const pixel_t paleta[][8], size_t filas, size_t columnas, const uint16_t mosaico_teselas[filas][columnas], const uint8_t mosaico_paletas[filas][columnas]);

imagen_t *imagen_espejar(imagen_t *o);
imagen_t *imagen_escalar(const imagen_t *origen, size_t ancho_destino, size_t alto_destino);
imagen_t *imagen_recortar_filas(const imagen_t *origen, size_t fila_inicial, size_t fila_final);

bool leer_teselas(imagen_t *teselas[]);

// ANCHOR - DEBUG
void imagen_escribir_ppm(const imagen_t *im);
void imagen_pegar_fila_con_paleta(imagen_t *destino, const imagen_t *origen, int x, int y, const pixel_t paleta[], size_t f);

#endif    // IMAGEN_H
