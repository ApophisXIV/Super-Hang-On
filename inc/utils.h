#ifndef UTILS_H
#define UTILS_H

/**
 * @file utils.h
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
#include <stdio.h>

/* ------------------- Public data structures and typedefs ------------------ */

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */

/* ------------------------------ Public enums ------------------------------ */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
// Setters
// Utils
void dump_image_to_ppm(const imagen_t *im, const char *filename);
void dump_image_to_txt(const imagen_t *im, const char *filename);

#endif    // UTILS_H