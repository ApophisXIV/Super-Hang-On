#ifndef FONDO_H
#define FONDO_H

/**
 * @file fondo.h
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
#include <stdint.h>

/* ------------------- Public data structures and typedefs ------------------ */

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */
#define FONDO1_FILAS    2
#define FONDO1_COLUMNAS 216
#define FONDO2_FILAS    8
#define FONDO2_COLUMNAS 152

extern const uint8_t fondo1_paleta[FONDO1_FILAS][FONDO1_COLUMNAS];
extern const uint16_t fondo1_mosaico[FONDO1_FILAS][FONDO1_COLUMNAS];
extern const uint8_t fondo2_paleta[FONDO2_FILAS][FONDO2_COLUMNAS];
extern const uint16_t fondo2_mosaico[FONDO2_FILAS][FONDO2_COLUMNAS];

/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
// Setters
// Utils

#endif    // FONDO_H