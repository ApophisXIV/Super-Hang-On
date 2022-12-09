/**
 * @file pixel.c
 * @author Guido Rodriguez (guerodriguez@fi.uba.ar)
 * @brief Descripcion
 * @version 1.0
 * @date YYYY-MM-DD
 *
 * @copyright Copyright (c) 2021
 *
 */

/* -------------------------------- Includes -------------------------------- */
#include "../../inc/pixel.h"

/* ---------------------- Private constants and macros ---------------------- */
#define R_P3_SHIFT 2
#define G_P3_SHIFT 1
#define B_P3_SHIFT 0

#define R_P4_SHIFT 3
#define G_P4_SHIFT 2
#define B_P4_SHIFT 1
#define A_P4_SHIFT 0

#define R_P12_SHIFT 8
#define G_P12_SHIFT 4
#define B_P12_SHIFT 0

#define R_P12_MASK 0x0f00
#define G_P12_MASK 0x00f0
#define B_P12_MASK 0x000f

/* -------------------- Public prototypes implementation -------------------- */
// Utils
pixel_t pixel3_crear(bool r, bool g, bool b) {
    return (r << R_P3_SHIFT) | (g << G_P3_SHIFT) | (b << B_P3_SHIFT);
}

void pixel3_a_rgb(pixel_t pixel3, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = ((pixel3 >> R_P3_SHIFT) & 1) * 255;
    *g = ((pixel3 >> G_P3_SHIFT) & 1) * 255;
    *b = ((pixel3 >> G_P3_SHIFT) & 1) * 255;
}

pixel_t pixel4_crear(bool r, bool g, bool b, bool i) {
    return (r << R_P4_SHIFT) | (g << G_P4_SHIFT) | (b << B_P4_SHIFT) | (i << A_P4_SHIFT);
}

void pixel4_a_rgbi(pixel_t pixel4, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *i) {
    *r = ((pixel4 >> R_P4_SHIFT) & 1) * 255;
    *g = ((pixel4 >> G_P4_SHIFT) & 1) * 255;
    *b = ((pixel4 >> B_P4_SHIFT) & 1) * 255;
    *i = ((pixel4 >> A_P4_SHIFT) & 1) * 255;
}

pixel_t pixel12_crear(uint8_t r, uint8_t g, uint8_t b) {
    return (r << R_P12_SHIFT) | (g << G_P12_SHIFT) | (b << B_P12_SHIFT);
}

void pixel12_a_rgb(pixel_t pixel12, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = (pixel12 & R_P12_MASK) >> 4;
    *g = (pixel12 & G_P12_MASK) >> 0;
    *b = (pixel12 & B_P12_MASK) << 4;
}