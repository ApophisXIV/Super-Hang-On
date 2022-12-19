#ifndef GFX_CFG_H
#define GFX_CFG_H

/**
 * @file gfx_cfg.h
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

/* ------------------- Public data structures and typedefs ------------------ */
typedef struct {
    int mem_offset, w, h, img_x, img_y;
} sprites_cfg_t;

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */
extern const sprites_cfg_t moto_sprites_cfg[];
extern const sprites_cfg_t road_sprites_cfg[];
extern const sprites_cfg_t semaforo_sprites_cfg[];
/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
// Setters
// Utils

#endif    // GFX_CFG_H