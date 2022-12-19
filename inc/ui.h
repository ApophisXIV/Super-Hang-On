#ifndef UI_H
#define UI_H

/**
 * @file ui.h
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
#include <stdint.h>

/* ------------------- Public data structures and typedefs ------------------ */

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */

/* ------------------------------ Public enums ------------------------------ */
// typedef enum { GOAL } ui_text_t;
/* ---------------------------- Public prototypes --------------------------- */
// Getters
// Setters
// Utils
void ui_update_text_over_canvas(imagen_t *canvas, const imagen_t **teselas, const moto_t *moto, size_t time);

#endif    // UI_H