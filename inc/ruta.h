#ifndef RUTA_H
#define RUTA_H

/**
 * @file ruta.h
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
#include <stdbool.h>
#include <stddef.h>

/* ------------------- Public data structures and typedefs ------------------ */
struct figura_en_ruta {
    int figura;    // Se deja libertad para definir esto como un enum donde corresponda
    size_t paleta;
    int y;
    bool reflejar;
};

typedef struct {
    float radio_curva;
    size_t indice_figura;
} ruta_t;

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */
extern const struct figura_en_ruta figuras_en_ruta[];
extern const ruta_t ruta[4200 + 70];

/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
// Setters
// Utils

#endif    // RUTA_H
