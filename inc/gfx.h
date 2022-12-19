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
#include "semaforo.h"
#include "ui.h"
#include "gfx_cfg.h"

/* ------------------- Public data structures and typedefs ------------------ */
typedef struct gfx gfx_t;
typedef void (*collision_fn_t)(moto_t *moto, double obs_w);

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */

/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Constructor
gfx_t *gfx_crear(uint16_t *canvas);

// Destructor
void gfx_destruir(gfx_t *gfx);

// Getters
imagen_t *gfx_get_figures(gfx_t *gfx, sprites_cfg_t cfg);

// Setters

// Utils
void gfx_display(gfx_t *gfx);
void gfx_update_math(gfx_t *gfx, const moto_t *moto);

void gfx_update_bg_over_canvas(gfx_t *gfx, const moto_t *moto);
void gfx_update_grass_over_canvas(gfx_t *gfx);

void gfx_update_bike_over_canvas(gfx_t *gfx, moto_t *moto);

void gfx_update_semaforo_over_canvas(gfx_t *gfx, const moto_t *moto, semaforo_t *semaforo);
void gfx_update_road_over_canvas(gfx_t *gfx, imagen_t *road_img, const moto_t *moto, semaforo_t *semaforo);

void gfx_update_ui_over_canvas(gfx_t *gfx, const moto_t *moto, double time);

#endif    // GFX_H