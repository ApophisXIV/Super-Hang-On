#ifndef GAME_MATH_H
#define GAME_MATH_H

/**
 * @file game_math.h
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
#include "ruta.h"

/* ------------------- Public data structures and typedefs ------------------ */

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */
#define WITH_CHECK    1
#define WITHOUT_CHECK 0
/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
// Setters
// Utils
double map_v(double d);
double map_d(double v);
double map_h(double h0, double v, bool check);
double map_u_p(double yx, size_t v, double w, double *offset_table);
double map_v_p(double v, double h);
void offset(double *offset_table, const ruta_t *ruta, double ym, double xm);

#endif    // GAME_MATH_H