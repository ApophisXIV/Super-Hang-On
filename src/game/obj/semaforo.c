/**
 * @file semaforo.c
 * @author Guido Rodriguez (guerodriguez@fi.uba.ar)
 * @brief Descripcion
 * @version 1.0
 * @date YYYY-MM-DD
 *
 * @copyright Copyright (c) 2021
 *
 */
/* -------------------------------- Includes -------------------------------- */
#include "../../../inc/semaforo.h"
#include "../../../inc/config.h"
#include <stdlib.h>

/* ---------------------- Private constants and macros ---------------------- */
#define INITIAL_X 8.0
/* ------------------------------ Private enums ----------------------------- */

/* ------------------ Private data structures and typedefs ------------------ */
struct semaforo {
    semaforo_state_t estado;
    double dx;
    bool has_started, is_visible;
    double self_timer;
};

/* ---------------------------- Private typedefs ---------------------------- */

/* -------------------- Private prototypes implementation ------------------- */

/* -------------------- Public prototypes implementation -------------------- */
// Constructor
semaforo_t *semaforo_crear() {
    semaforo_t *semaforo = malloc(sizeof(semaforo_t));
    if (!semaforo) return NULL;
    semaforo->estado      = SEMAFORO_STANDBY;
    semaforo->dx          = INITIAL_X;
    semaforo->has_started = false;
    semaforo->is_visible  = true;
    semaforo->self_timer  = 0.0;

    return semaforo;
}

// Destructor
void semaforo_destruir(semaforo_t *semaforo) {
    free(semaforo);
}

// Getters
semaforo_state_t semaforo_get_estado(const semaforo_t *semaforo) {
    return semaforo->estado;
}

#include <stdio.h>
bool semaforo_has_started(const semaforo_t *semaforo) {
    return semaforo->has_started;
}

bool semaforo_is_visible(const semaforo_t *semaforo) {
    return semaforo->is_visible;
}

// Setters
void semaforo_set_estado(semaforo_t *semaforo, semaforo_state_t estado) {
    semaforo->estado = estado;
}

// ANCHOR Debug
// static const char *lut_semaforo_state[] = {
//     [SEMAFORO_STANDBY]  = "STANDBY",
//     [SEMAFORO_ROJO]     = "ROJO",
//     [SEMAFORO_AMARILLO] = "AMARILLO",
//     [SEMAFORO_VERDE]    = "VERDE",
// };

void semaforo_update(semaforo_t *semaforo, double moto_x) {

    semaforo->dx         = INITIAL_X - moto_x <= 0 ? (4200 - 2 - moto_x) : INITIAL_X - moto_x;
    semaforo->is_visible = semaforo->dx <= 60;
    semaforo->self_timer += DELTA_T;

    // ANCHOR Debug
    // printf("dx: %f | estado: %s | has_started: %d | is_visible: %d | self_timer: %f | moto_x: %f \n", semaforo->dx, lut_semaforo_state[semaforo->estado], semaforo->has_started, semaforo->is_visible, semaforo->self_timer, moto_x);

    static const semaforo_state_t lut_time_state[] = {
        [0] = SEMAFORO_STANDBY,
        [1] = SEMAFORO_ROJO,
        [2] = SEMAFORO_AMARILLO,
        [3] = SEMAFORO_AMARILLO,
        [4] = SEMAFORO_VERDE,
    };
    semaforo_set_estado(semaforo, semaforo->has_started ? SEMAFORO_VERDE : lut_time_state[(int)semaforo->self_timer]);
    if (semaforo->estado == SEMAFORO_VERDE) semaforo->has_started = true;
}

double semaforo_get_distance(const semaforo_t *semaforo) {
    return semaforo->dx;
}

size_t semaforo_get_palette(const semaforo_t *semaforo, semaforo_elements_t e) {
    static const size_t lut_semaforo_palette[] = {
        [BANNER] = 45,
        [VIGA_1] = 45,
        [VIGA_2] = 45,
    };
    static const size_t lut_semaforo_luz_palette[] = {
        [SEMAFORO_STANDBY]  = 41,
        [SEMAFORO_ROJO]     = 42,
        [SEMAFORO_AMARILLO] = 43,
        [SEMAFORO_VERDE]    = 44,
    };

    if (e == BANNER && semaforo->has_started && semaforo->dx > INITIAL_X) return 47;
    if (e == SEMAFORO_1 || e == SEMAFORO_2) return lut_semaforo_luz_palette[semaforo_get_estado(semaforo)];
    return lut_semaforo_palette[e];
}
