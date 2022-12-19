#ifndef FISICA_H
#define FISICA_H

/**
 * @file fisica.h
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
#include "moto.h"
#include "ruta.h"
/* ---------------------------- Public prototypes --------------------------- */
// Utils
void phy_update_physics(moto_t *moto, double tiempo_total);
#endif    // FISICA_H