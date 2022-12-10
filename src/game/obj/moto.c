/**
 * @file archivo.c
 * @author Guido Rodriguez (guerodriguez@fi.uba.ar)
 * @brief Descripcion
 * @version 1.0
 * @date YYYY-MM-DD
 *
 * @copyright Copyright (c) 2021
 *
 */
/* -------------------------------- Includes -------------------------------- */
#include "../../../inc/moto.h"
#include "../../../inc/paleta.h"
#include <stdint.h>
#include <stdlib.h>

/* ---------------------- Private constants and macros ---------------------- */

/* ------------------------------ Private enums ----------------------------- */

/* ------------------ Private data structures and typedefs ------------------ */

typedef struct {
    double points;
    bool mordiendo_banquina, colisionando, gano, perdio;
} moto_player_data_t;

typedef struct {
    double velocity;
    double x, last_x, y;
    int spin_intensity;
} moto_physics_data_t;

typedef bool moto_vel_t;
typedef struct {
    moto_palette_t palette;
    moto_dir_state_t dir_state;
    moto_vel_t vel_states[2];
} moto_gfx_t;

struct moto {
    // Player data
    moto_player_data_t player_data;

    // Physics data
    moto_physics_data_t physics_data;

    // Graphics data
    moto_gfx_t gfx_data;
};

/* ---------------------------- Private typedefs ---------------------------- */

/* -------------------- Private prototypes implementation ------------------- */

/* -------------------- Public prototypes implementation -------------------- */
// Constructor
moto_t *moto_crear() {

    moto_t *moto = malloc(sizeof(moto_t));
    if (!moto) return NULL;

    moto->physics_data.x              = 0.0;
    moto->physics_data.last_x         = 0.0;
    moto->physics_data.y              = 0.0;
    moto->physics_data.velocity       = 1.0;
    moto->physics_data.spin_intensity = 0;

    moto->gfx_data.palette                     = P_AVANCE_1;
    moto->gfx_data.dir_state                   = MOTO_REPOSO;
    moto->gfx_data.vel_states[MOTO_ACELERANDO] = false;
    moto->gfx_data.vel_states[MOTO_FRENANDO]   = false;

    moto->player_data.points             = 0;
    moto->player_data.mordiendo_banquina = false;
    moto->player_data.colisionando       = false;
    moto->player_data.gano               = false;
    moto->player_data.perdio             = false;

    return moto;
}
// Destructor
void moto_destruir(moto_t *moto) {
    free(moto);
}

// Getters
int moto_get_spin_intensity(const moto_t *moto) {
    return moto->physics_data.spin_intensity;
}
double moto_get_velocity(const moto_t *moto) {
    return moto->physics_data.velocity;
}
double moto_get_x(const moto_t *moto) {
    return moto->physics_data.x;
}
double moto_get_last_x(const moto_t *moto){
    return moto->physics_data.last_x;
}

double moto_get_y(const moto_t *moto) {
    return moto->physics_data.y;
}

double moto_get_points(const moto_t *moto) {
    return moto->player_data.points;
}

bool moto_is_morder_banquina(const moto_t *moto) {
    return moto->player_data.mordiendo_banquina;
}
bool moto_is_colision(const moto_t *moto) {
    return moto->player_data.colisionando;
}
bool moto_is_gano(const moto_t *moto) {
    return moto->player_data.gano;
}

bool moto_is_perdio(const moto_t *moto) {
    return moto->player_data.perdio;
}

moto_dir_state_t moto_get_dir_state(const moto_t *moto) {
    return moto->gfx_data.dir_state;
}


moto_vel_state_t moto_get_vel_state(moto_t *moto) {
    moto_vel_state_t vel_state = MOTO_REPOSO;
    if (moto->gfx_data.vel_states[MOTO_ACELERANDO] && !(moto->gfx_data.vel_states[MOTO_FRENANDO])) vel_state = MOTO_ACELERANDO;
    if (moto->gfx_data.vel_states[MOTO_FRENANDO]) vel_state = MOTO_FRENANDO;

    if (vel_state == MOTO_FRENANDO) moto->gfx_data.palette = P_FRENADO_1;
    else moto->gfx_data.palette = P_AVANCE_1;
    
    return vel_state;
}

moto_palette_t moto_get_palette(const moto_t *moto) {
    return moto->gfx_data.palette;
}

// Setters
void moto_set_spin_intensity(moto_t *moto, int intensity) {
    moto->physics_data.spin_intensity = intensity;
}
void moto_set_velocity(moto_t *moto, double velocity) {
    moto->physics_data.velocity = velocity;
}
void moto_set_x(moto_t *moto, double x) {
    moto->physics_data.last_x = moto->physics_data.x;
    moto->physics_data.x = x;
}
void moto_set_y(moto_t *moto, double y) {
    moto->physics_data.y = y;
}

void moto_set_points(moto_t *moto, double points) {
    moto->player_data.points = points;
}

void moto_set_dir_state(moto_t *moto, moto_dir_state_t action, bool is_key_down) {
    is_key_down ? (moto->gfx_data.dir_state = action) : (moto->gfx_data.dir_state = MOTO_REPOSO);
}
void moto_set_vel_state(moto_t *moto, moto_vel_state_t action, bool is_key_down) {
    is_key_down ? (moto->gfx_data.vel_states[action] = true) : (moto->gfx_data.vel_states[action] = false);
}

void moto_set_morder_banquina(moto_t *moto, bool state) {
    moto->player_data.mordiendo_banquina = state;
}
void moto_set_colision(moto_t *moto, bool state) {
    moto->player_data.colisionando = state;
}
void moto_set_gano(moto_t *moto, bool state) {
    moto->player_data.gano = state;
}
void moto_set_perdio(moto_t *moto, bool state) {
    moto->player_data.perdio = state;
}

void moto_set_palette(moto_t *moto, moto_palette_t palette) {
    moto->gfx_data.palette = palette;
}

// Utils
