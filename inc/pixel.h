#ifndef PIXEL_H
#define PIXEL_H

/**
 * @file pixel.h
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
#include <stdint.h>

/* ------------------- Public data structures and typedefs ------------------ */
typedef uint16_t pixel_t;

/* ----------------------- Public constants and macros ---------------------- */
#define MAXIMO 255

/* ---------------------------- Public prototypes --------------------------- */
// Utils
pixel_t pixel3_crear(bool r, bool g, bool b);
void pixel3_a_rgb(pixel_t pixel3, uint8_t *r, uint8_t *g, uint8_t *b);

pixel_t pixel4_crear(bool r, bool g, bool b, bool i);
void pixel4_a_rgbi(pixel_t pixel4, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *i);

pixel_t pixel12_crear(uint8_t r, uint8_t g, uint8_t b);
void pixel12_a_rgb(pixel_t pixel12, uint8_t *r, uint8_t *g, uint8_t *b);

#endif    // PIXEL_H