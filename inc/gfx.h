#ifndef GFX_H
#define GFX_H

/**
 * @file archivo.h
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
#include "imagen.h"
#include "moto.h"
#include "ruta.h"

/* ------------------- Public data structures and typedefs ------------------ */

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */

/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
// Setters
// Utils
imagen_t *gfx_get_figures(uint16_t *buf_figs, size_t n_offset, size_t w, size_t h);
void gfx_update_bg_over_canvas(imagen_t *canvas, const ruta_t *road, const moto_t *moto, const imagen_t **teselas);
void gfx_update_road_over_canvas(imagen_t *canvas, imagen_t *road_img, const ruta_t *road, const moto_t *moto);
bool gfx_update_sprite_over_canvas(imagen_t *canvas, moto_t *moto, uint16_t *buf_figs);

#endif    // GFX_H