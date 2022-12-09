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
#include "../../../inc/figuras.h"
#include "../../../inc/paleta.h"
#include <stdint.h>
#include <stdlib.h>

/* ---------------------- Private constants and macros ---------------------- */

/* ------------------------------ Private enums ----------------------------- */

/* ------------------ Private data structures and typedefs ------------------ */

typedef enum {
    SPIN_INTENSITY_0,
    SPIN_INTENSITY_1,
    SPIN_INTENSITY_2,
    SPIN_INTENSITY_3,
} moto_actions_t;

// Tabla de Sprites
typedef struct {
    size_t mem_offset, w, h, img_x, img_y;
} sprites_cfg_t;

typedef struct {
    double points;
    bool mordiendo_banquina, colisionando, gano, perdio;
} moto_player_data_t;

typedef struct {
    double velocity;
    double x, y;
    int spin_intensity;
} moto_physics_data_t;

typedef enum {
    P_AVANCE_1,
    P_AVANCE_2,
    P_FRENADO_1,
    P_FRENADO_2,
} moto_palette_t;

typedef struct {
    moto_palette_t palette;
    moto_dir_state_t dir_state;
    moto_vel_state_t vel_state;
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
    moto->physics_data.y              = 0.0;
    moto->physics_data.velocity       = 1.0;
    moto->physics_data.spin_intensity = 0;

    moto->gfx_data.palette   = P_AVANCE_1;
    moto->gfx_data.dir_state = MOTO_REPOSO;
    moto->gfx_data.vel_state = MOTO_NEUTRAL;

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
moto_vel_state_t moto_get_vel_state(const moto_t *moto) {
    return moto->gfx_data.vel_state;
}

// Setters
void moto_set_spin_intensity(moto_t *moto, int intensity) {
    moto->physics_data.spin_intensity = intensity;
}
void moto_set_velocity(moto_t *moto, double velocity) {
    moto->physics_data.velocity = velocity;
}
void moto_set_x(moto_t *moto, double x) {
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
    is_key_down ? (moto->gfx_data.vel_state = action) : (moto->gfx_data.vel_state = MOTO_NEUTRAL);

    // Si no esta frenando, seteo la paleta de avance
    if (moto->gfx_data.vel_state != MOTO_FRENANDO) moto->gfx_data.palette = P_AVANCE_1;

    // Si esta frenando, seteo la paleta de frenado
    if (moto->gfx_data.vel_state == MOTO_FRENANDO) moto->gfx_data.palette = P_FRENADO_1;
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

// Utils
// LUT de Sprites
static const sprites_cfg_t sprites_cfg[] = {
    [SPIN_INTENSITY_0] = {.mem_offset = 532, .w = 36, .h = 73, .img_x = 144, .img_y = 150},
    [SPIN_INTENSITY_1] = {.mem_offset = 5670, .w = 36, .h = 70, .img_x = 126, .img_y = 153},
    [SPIN_INTENSITY_2] = {.mem_offset = 11284, .w = 46, .h = 63, .img_x = 126, .img_y = 163},
    [SPIN_INTENSITY_3] = {.mem_offset = 17215, .w = 60, .h = 54, .img_x = 126, .img_y = 172},
};

static moto_update_palette(moto_t *moto) {
    // Toggling palette for animation. If its set P_AVANCE_1, set P_AVANCE_2 and viceversa
    if (moto->gfx_data.palette == P_AVANCE_1)
        moto->gfx_data.palette = P_AVANCE_2;
    else if (moto->gfx_data.palette == P_AVANCE_2)
        moto->gfx_data.palette = P_AVANCE_1;

    // If its set P_FRENADO_1, set P_FRENADO_2 and viceversa
    if (moto->gfx_data.palette == P_FRENADO_1)
        moto->gfx_data.palette = P_FRENADO_2;
    else if (moto->gfx_data.palette == P_FRENADO_2)
        moto->gfx_data.palette = P_FRENADO_1;
}

bool update_sprite_over_canvas(imagen_t *canvas, moto_t *moto, uint16_t *buf_figs) {

    // Get sprite config
    const sprites_cfg_t sprite_cfg = sprites_cfg[abs(moto->physics_data.spin_intensity)];

    imagen_t *sprite = get_figures(buf_figs, sprite_cfg.mem_offset, sprite_cfg.w, sprite_cfg.h);
    if (!sprite) return false;

    // If its turning left, flip sprite
    if (moto->physics_data.spin_intensity < 0) {
        imagen_t *img = imagen_espejar(sprite);
        if (!img) return false;

        imagen_destruir(sprite);

        sprite = img;
    }

    // Update sprite in canvas
    moto_update_palette(moto);
    imagen_pegar_con_paleta(canvas, sprite, sprite_cfg.img_x, sprite_cfg.img_y, paleta_4[moto->gfx_data.palette], NO_STRIP_MODE, NO_STRIP_MODE);    // TODO Terminar tema de las paletas dinamicas

    // Free sprite
    imagen_destruir(sprite);

    return true;
}
