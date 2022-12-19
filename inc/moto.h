#ifndef MOTO_H
#define MOTO_H

/**
 * @file moto.h
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
#include <stdbool.h>

/* ------------------- Public data structures and typedefs ------------------ */
typedef struct moto moto_t;

/* ------------------------------- Constructor ------------------------------ */
moto_t *moto_crear();

/* ------------------------------- Destructor ------------------------------- */
void moto_destruir(moto_t *moto);

/* ----------------------- Public constants and macros ---------------------- */

/* ------------------------------ Public enums ------------------------------ */
typedef enum {
    MOTO_GIRANDO_IZQUIERDA,
    MOTO_GIRANDO_DERECHA,
    MOTO_REPOSO,
} moto_dir_state_t;

typedef enum {
    MOTO_FRENANDO,
    MOTO_ACELERANDO,
    MOTO_NEUTRAL,
} moto_vel_state_t;

typedef enum {
    SPIN_INTENSITY_0,
    SPIN_INTENSITY_1,
    SPIN_INTENSITY_2,
    SPIN_INTENSITY_3,
} moto_actions_t;

typedef enum {
    P_AVANCE_1,
    P_AVANCE_2,
    P_FRENADO_1,
    P_FRENADO_2,
} moto_palette_t;

/* ---------------------------- Public prototypes --------------------------- */

// Getters
int moto_get_spin_intensity(const moto_t *moto);
double moto_get_velocity(const moto_t *moto);
double moto_get_x(const moto_t *moto);
double moto_get_last_x(const moto_t *moto);
double moto_get_y(const moto_t *moto);
double moto_get_points(const moto_t *moto);
bool moto_is_colision(const moto_t *moto);
bool moto_gano(const moto_t *moto);
bool moto_perdio(const moto_t *moto);
moto_dir_state_t moto_get_dir_state(const moto_t *moto);
moto_vel_state_t moto_get_vel_state(moto_t *moto);
moto_palette_t moto_get_palette(const moto_t *moto);

// Setters
void moto_set_spin_intensity(moto_t *moto, int intensity);
void moto_set_velocity(moto_t *moto, double velocity);
void moto_set_x(moto_t *moto, double x);
void moto_set_y(moto_t *moto, double y);
void moto_set_points(moto_t *moto, double points);
void moto_set_morder_banquina(moto_t *moto, bool state);
void moto_set_colision(moto_t *moto, bool choque);
void moto_set_gano(moto_t *moto, bool state);
void moto_set_perdio(moto_t *moto, bool state);
void moto_set_dir_state(moto_t *moto, moto_dir_state_t action, bool is_key_down);
void moto_set_vel_state(moto_t *moto, moto_vel_state_t action, bool is_key_down);
void moto_set_last_x(moto_t *moto, double x);
void moto_set_palette(moto_t *moto, moto_palette_t palette);

// Utils

#endif    // MOTO_H