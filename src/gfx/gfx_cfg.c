
/**
 * @file gfx_cfg.c
 * @author Guido Rodriguez (guerodriguez@fi.uba.ar)
 * @brief Descripcion
 * @version 1.0
 * @date YYYY-MM-DD
 *
 * @copyright Copyright (c) 2021
 *
 */
/* -------------------------------- Includes -------------------------------- */
#include "../../inc/gfx_cfg.h"
#include "../../inc/moto.h"
#include "../../inc/ruta.h"
#include "../../inc/semaforo.h"

/* ------------------ Private data structures and typedefs ------------------ */

/* ---------------------- Private constants and macros ---------------------- */
// LUT de Sprites
const sprites_cfg_t moto_sprites_cfg[] = {
    [SPIN_INTENSITY_0] = {.mem_offset = 532, .w = 36, .h = 73, .img_x = 144, .img_y = 150},
    [SPIN_INTENSITY_1] = {.mem_offset = 5670, .w = 36, .h = 70, .img_x = 126, .img_y = 153},
    [SPIN_INTENSITY_2] = {.mem_offset = 11284, .w = 46, .h = 63, .img_x = 126, .img_y = 163},
    [SPIN_INTENSITY_3] = {.mem_offset = 17215, .w = 60, .h = 54, .img_x = 126, .img_y = 172},
};

// NOTE: The x and y coordinates are given externally to the sprite
const sprites_cfg_t road_sprites_cfg[] = {
    [ARBOL]  = {.mem_offset = 50116, .w = 46, .h = 188},
    [CARTEL] = {.mem_offset = 37390, .w = 96, .h = 112},
    [ROCA]   = {.mem_offset = 69464, .w = 136, .h = 76},
    [BELL]   = {.mem_offset = 82922, .w = 63, .h = 146},
    [FORUM]  = {.mem_offset = 89102, .w = 118, .h = 114},
    [DELFIN] = {.mem_offset = 119280, .w = 144, .h = 110},
};

const sprites_cfg_t semaforo_sprites_cfg[] = {
    [SEMAFORO_1] = {.mem_offset = 108724, .w = 66, .h = 201, .img_y = -215},
    [SEMAFORO_2] = {.mem_offset = 108724, .w = 66, .h = 201, .img_y = 215},
    [VIGA_1]     = {.mem_offset = 127098, .w = 198, .h = 48, .img_y = 145},
    [VIGA_2]     = {.mem_offset = 127098, .w = 198, .h = 48, .img_y = -145},
    [BANNER]     = {.mem_offset = 114518, .w = 244, .h = 48, .img_y = 0},
};
/* ------------------------------ Private enums ----------------------------- */

/* ---------------------------- Private typedefs ---------------------------- */

/* -------------------- Private prototypes implementation ------------------- */

/* -------------------- Public prototypes implementation -------------------- */
// Getters
// Setters
// Utils
