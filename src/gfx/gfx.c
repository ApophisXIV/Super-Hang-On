/**
 * @file gfx.c
 * @author Guido Rodriguez (guerodriguez@fi.uba.ar)
 * @brief Descripcion
 * @version 1.0
 * @date YYYY-MM-DD
 *
 * @copyright Copyright (c) 2021
 *
 */
/* -------------------------------- Includes -------------------------------- */
#include "../../inc/gfx.h"
#include "../../inc/config.h"
#include "../../inc/fondo.h"
#include "../../inc/game_math.h"
#include "../../inc/paleta.h"
#include "../../inc/pixel.h"

/* ------------------------------ Private enums ----------------------------- */

/* ------------------ Private data structures and typedefs ------------------ */
typedef struct {
    size_t mem_offset, w, h, img_x, img_y;
} sprites_cfg_t;

/* ---------------------- Private constants and macros ---------------------- */
// LUT de Sprites
static const sprites_cfg_t sprites_cfg[] = {
    [SPIN_INTENSITY_0] = {.mem_offset = 532, .w = 36, .h = 73, .img_x = 144, .img_y = 150},
    [SPIN_INTENSITY_1] = {.mem_offset = 5670, .w = 36, .h = 70, .img_x = 126, .img_y = 153},
    [SPIN_INTENSITY_2] = {.mem_offset = 11284, .w = 46, .h = 63, .img_x = 126, .img_y = 163},
    [SPIN_INTENSITY_3] = {.mem_offset = 17215, .w = 60, .h = 54, .img_x = 126, .img_y = 172},
};

/* ---------------------------- Private typedefs ---------------------------- */

/* -------------------- Private prototypes implementation ------------------- */

/* -------------------- Public prototypes implementation -------------------- */
// Getters
#define IS_NEW_LINE_FLAG(w)      ((w & 0xf) == 0xf)
#define FIRST_PIXEL(w)           ((w >> 12) & 0xf)
#define SHOULD_BE_TRANSPARENT(w) ((w & 0xf) == 0xf)
#define TRANSPARENT              0

imagen_t *gfx_get_figures(uint16_t *buf_figs, size_t n_offset, size_t w, size_t h) {

    if (buf_figs == NULL || w == 0 || h == 0) return NULL;

    imagen_t *im = imagen_generar(w, h, TRANSPARENT);
    if (im == NULL) return NULL;

    bool just_started_the_line = 0;
    for (size_t chunk = 0, x = 0, y = 0; chunk < (w * h - w) / 4; chunk++) {

        uint16_t word = buf_figs[n_offset + chunk];

        if (just_started_the_line && IS_NEW_LINE_FLAG(word)) continue;
        just_started_the_line = 0;

        for (size_t i = 0; i < 4; i++) {
            pixel_t p = (word >> (12 - i * 4)) & 0xf;
            imagen_set_pixel(im, x + 1 >= w ? (x = 0) : x++, y, p == 0xf ? TRANSPARENT : p);
        }

        if (IS_NEW_LINE_FLAG(word)) {
            x = 0;
            y++;
            just_started_the_line = 1;
            continue;
        }
    }

    return im;
}

// Setters
// Utils
void gfx_update_bg_over_canvas(imagen_t *canvas, const ruta_t *road, const moto_t *moto, const imagen_t **teselas) {

    // TODO: Revisar si es necesario el last_x de moto

    int acc_road_rho = 0;
    for (size_t i = moto_get_last_x(moto); i < moto_get_x(moto); acc_road_rho += road[i++].radio_curva);

    static int fondo1_x_offset = 320;
    static int fondo2_x_offset = 320;

    fondo1_x_offset -= acc_road_rho;
    fondo2_x_offset -= 0.75 * acc_road_rho;

    imagen_t *fondo1 = generar_mosaico(teselas, paleta_3, FONDO1_FILAS, FONDO1_COLUMNAS, fondo1_mosaico, fondo1_paleta);
    imagen_t *fondo2 = generar_mosaico(teselas, paleta_3, FONDO2_FILAS, FONDO2_COLUMNAS, fondo2_mosaico, fondo2_paleta);

    imagen_pegar(canvas, fondo2, fondo2_x_offset, 64);
    imagen_pegar(canvas, fondo1, fondo1_x_offset, 112);

    imagen_destruir(fondo1);
    imagen_destruir(fondo2);
}

void gfx_update_road_over_canvas(imagen_t *canvas, imagen_t *road_img, const ruta_t *road, const moto_t *moto) {

    // El desplazamiento del centro de la ruta va a ser la suma ur(v) = ul(v) + uc(v), la idea es
    // computar este vector de 96 posiciones una vez por cada instante dado que el mismo no sólo sirve
    // para dibujar la ruta sino, como ahora veremos, también sirve para posicionar el resto de los objetos
    // en la pantalla.
    // Cada 4 metros de ruta, cambia la paleta de colores (mi1rar ecuacion provista por el TP)

    // First, we get the offset table
    double offset_lut[ALTO_PUNTO_DE_FUGA];
    offset(offset_lut, road, moto_get_y(moto), moto_get_x(moto));

    for (size_t v = 0, p_i = 0; v < imagen_get_alto(road_img); v++, p_i = (size_t)(moto_get_x(moto) + map_d(v)) % 4)
        // Only we need "strips" of the image. The vertical position is given externally
        // El coeficiente de desplazamiento -56 es para centrar la ruta en la pantalla (obtenido empíricamente)
        imagen_pegar_con_paleta(canvas, road_img, offset_lut[v] - 56, VENTANA_ALTO_ORIGINAL - 1 - v, colores_ruta[p_i], STRIP_MODE, ALTO_PUNTO_DE_FUGA - 1 - v);
}

static void moto_update_palette(moto_t *moto) {

    moto_palette_t act_palette = moto_get_palette(moto);

    // Toggling palette for animation. If its set P_AVANCE_1, set P_AVANCE_2 and viceversa
    if (act_palette == P_AVANCE_1)
        moto_set_palette(moto, P_AVANCE_2);
    else if (act_palette == P_AVANCE_2)
        moto_set_palette(moto, P_AVANCE_1);

    // If its set P_FRENADO_1, set P_FRENADO_2 and viceversa
    if (act_palette == P_FRENADO_1)
        moto_set_palette(moto, P_FRENADO_2);
    else if (act_palette == P_FRENADO_2)
        moto_set_palette(moto, P_FRENADO_1);
}

bool gfx_update_sprite_over_canvas(imagen_t *canvas, moto_t *moto, uint16_t *buf_figs) {

    // Get sprite config
    int spin_intensity = moto_get_spin_intensity(moto);
    const sprites_cfg_t sprite_cfg = sprites_cfg[spin_intensity < 0 ? -spin_intensity : spin_intensity];

    imagen_t *sprite = gfx_get_figures(buf_figs, sprite_cfg.mem_offset, sprite_cfg.w, sprite_cfg.h);
    if (!sprite) return false;

    // If its turning left, flip the sprite
    if (moto_get_spin_intensity(moto) < 0) {
        imagen_t *flipped_sprite = imagen_espejar(sprite);
        if (!flipped_sprite) {
            imagen_destruir(sprite);
            return false;
        }
        imagen_destruir(sprite);
        sprite = flipped_sprite;
    }

    // Update sprite in canvas
    imagen_pegar_con_paleta(canvas, sprite, sprite_cfg.img_x, sprite_cfg.img_y, paleta_4[moto_get_palette(moto)], NO_STRIP_MODE, NO_STRIP_MODE);
    moto_update_palette(moto);
    // Free sprite
    imagen_destruir(sprite);

    return true;
}
