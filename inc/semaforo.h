#ifndef SEMAFORO_H
#define SEMAFORO_H

/**
 * @file semaforo.h
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
#include <stddef.h>

/* ------------------------------ Public enums ------------------------------ */
typedef enum {
    SEMAFORO_1,
    SEMAFORO_2,
    VIGA_1,
    VIGA_2,
    BANNER,
} semaforo_elements_t;

typedef enum {
    SEMAFORO_STANDBY,
    SEMAFORO_ROJO,
    SEMAFORO_AMARILLO,
    SEMAFORO_VERDE,
} semaforo_state_t;

/* ------------------- Public data structures and typedefs ------------------ */
typedef struct semaforo semaforo_t;

/* ------------------------------- Constructor ------------------------------ */

/* ------------------------------- Destructor ------------------------------- */

/* ----------------------- Public constants and macros ---------------------- */

/* ---------------------------- Public prototypes --------------------------- */
// Getters
semaforo_state_t semaforo_get_estado(const semaforo_t *semaforo);
bool semaforo_has_started(const semaforo_t *semaforo);
bool semaforo_is_visible(const semaforo_t *semaforo);

// Setters
void semaforo_set_estado(semaforo_t *semaforo, semaforo_state_t estado);

// Utils
semaforo_t *semaforo_crear();
void semaforo_update(semaforo_t *semaforo, double moto_x);
void semaforo_destruir(semaforo_t *semaforo);
double semaforo_get_distance(const semaforo_t *semaforo);
size_t semaforo_get_palette(const semaforo_t *semaforo, semaforo_elements_t e);

#endif    // SEMAFORO_H