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
#include "../../inc/rom.h"
#include "../../inc/ui.h"
#include <stdlib.h>

/* ------------------------------ Private enums ----------------------------- */

/* ------------------ Private data structures and typedefs ------------------ */
struct gfx {
    imagen_t *cuadro;
    uint16_t *canvas;
    uint16_t buf_figs[N_VALORES_ROM];
    imagen_t *teselas[CANTIDAD_TESELAS];
    double offset_lut[ALTO_PUNTO_DE_FUGA];
};

/* ---------------------- Private constants and macros ---------------------- */

/* ---------------------------- Private typedefs ---------------------------- */

/* -------------------- Private prototypes implementation ------------------- */

/* -------------------- Public prototypes implementation -------------------- */
// Constructor
gfx_t *gfx_crear(uint16_t *canvas) {
    gfx_t *gfx = malloc(sizeof(gfx_t));
    if (gfx == NULL) return NULL;

    gfx->cuadro = NULL;
    gfx->canvas = canvas;

    if (rom_leer_figuras_raw(gfx->buf_figs) == false) {
        free(gfx);
        return NULL;
    }

    if (rom_leer_teselas(gfx->teselas) == false) {
        free(gfx);
        return NULL;
    }

    return gfx;
}

void gfx_destruir(gfx_t *gfx) {
    for (size_t i = 0; i < CANTIDAD_TESELAS; i++)
        imagen_destruir(gfx->teselas[i]);
    free(gfx);
}

void gfx_update_math(gfx_t *gfx, const moto_t *moto) {
    gfx->cuadro = imagen_generar(VENTANA_ANCHO_ORIGINAL, VENTANA_ALTO_ORIGINAL, 0xf);
    offset(gfx->offset_lut, ruta, moto_get_y(moto), moto_get_x(moto));
}

void gfx_display(gfx_t *gfx) {
    if (gfx == NULL || gfx->cuadro == NULL) return;

    imagen_t *scaled = imagen_escalar(gfx->cuadro, VENTANA_ANCHO, VENTANA_ALTO);
    if (scaled == NULL) return;

    imagen_a_textura(scaled, gfx->canvas);
    imagen_destruir(scaled);
    imagen_destruir(gfx->cuadro);
}

// Getters
#define IS_NEW_LINE_FLAG(w)      ((w & 0xf) == 0xf)
#define FIRST_PIXEL(w)           ((w >> 12) & 0xf)
#define SHOULD_BE_TRANSPARENT(w) ((w & 0xf) == 0xf)
#define TRANSPARENT              0

imagen_t *gfx_get_figures(gfx_t *gfx, sprites_cfg_t cfg) {

    if (gfx->buf_figs == NULL || cfg.w == 0 || cfg.h == 0) return NULL;

    imagen_t *im = imagen_generar(cfg.w, cfg.h, TRANSPARENT);
    if (im == NULL) return NULL;

    bool just_started_the_line = 0;
    for (size_t chunk = 0, x = 0, y = 0; chunk < (cfg.w * cfg.h - cfg.w) / 4; chunk++) {

        uint16_t word = gfx->buf_figs[cfg.mem_offset + chunk];

        if (just_started_the_line && IS_NEW_LINE_FLAG(word)) continue;
        just_started_the_line = 0;

        for (size_t i = 0; i < 4; i++) {
            pixel_t p = (word >> (12 - i * 4)) & 0xf;
            imagen_set_pixel(im, x + 1 >= cfg.w ? (x = 0) : x++, y, p == 0xf ? TRANSPARENT : p);
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

void gfx_update_grass_over_canvas(gfx_t *gfx) {
    imagen_t *pasto = imagen_generar(1, 96, pixel12_crear(0, 13, 9));
    for (size_t i = 0; i < 10; i++) imagen_set_pixel(pasto, 0, i, colores_pasto[i]);
    imagen_t *pasto_estirado = imagen_escalar(pasto, 320, 96);
    imagen_destruir(pasto);
    imagen_pegar(gfx->cuadro, pasto_estirado, 0, 128);
    imagen_destruir(pasto_estirado);
}

void gfx_update_bg_over_canvas(gfx_t *gfx, const moto_t *moto) {

    int acc_road_rho = 0;
    for (size_t i = moto_get_last_x(moto); i < moto_get_x(moto); i++) acc_road_rho += ruta[i].radio_curva;

    static int fondo1_x_offset = 320;
    static int fondo2_x_offset = 320;

    fondo1_x_offset -= acc_road_rho;
    fondo2_x_offset -= 0.75 * acc_road_rho;

    imagen_t *fondo1 = generar_mosaico((const imagen_t **)gfx->teselas, paleta_3, FONDO1_FILAS, FONDO1_COLUMNAS, fondo1_mosaico, fondo1_paleta, NOT_USED, NOT_USED);
    imagen_t *fondo2 = generar_mosaico((const imagen_t **)gfx->teselas, paleta_3, FONDO2_FILAS, FONDO2_COLUMNAS, fondo2_mosaico, fondo2_paleta, NOT_USED, NOT_USED);

    imagen_pegar(gfx->cuadro, fondo2, fondo2_x_offset, 64);
    imagen_pegar(gfx->cuadro, fondo1, fondo1_x_offset, 112);

    imagen_destruir(fondo1);
    imagen_destruir(fondo2);
}

static void moto_update_palette(moto_t *moto) {

    if (moto_get_x(moto) - moto_get_last_x(moto) < 2) return;
    moto_set_last_x(moto, moto_get_x(moto));

    moto_palette_t act_palette = moto_get_palette(moto);
    moto_vel_state_t vel_state = moto_get_vel_state(moto);

    // Toggling palette for animation. If its set P_AVANCE_1, set P_AVANCE_2 and viceversa
    if (vel_state == MOTO_ACELERANDO || vel_state == MOTO_NEUTRAL) {
        moto_set_palette(moto, act_palette == P_AVANCE_1 ? P_AVANCE_2 : P_AVANCE_1);
        return;
    }

    // If its set P_FRENADO_1, set P_FRENADO_2 and viceversa
    moto_set_palette(moto, act_palette == P_FRENADO_1 ? P_FRENADO_2 : P_FRENADO_1);
}

void gfx_update_bike_over_canvas(gfx_t *gfx, moto_t *moto) {

    // Get sprite config
    int spin_intensity             = moto_get_spin_intensity(moto);
    const sprites_cfg_t sprite_cfg = moto_sprites_cfg[spin_intensity < 0 ? -spin_intensity : spin_intensity];

    imagen_t *sprite = gfx_get_figures(gfx, sprite_cfg);

    // If its turning left, flip the sprite
    if (moto_get_spin_intensity(moto) < 0) {
        imagen_t *flipped_sprite = imagen_espejar(sprite);
        imagen_destruir(sprite);
        sprite = flipped_sprite;
    }

    // Update sprite in canvas
    imagen_pegar_con_paleta(gfx->cuadro, sprite, sprite_cfg.img_x, sprite_cfg.img_y, paleta_4[moto_get_palette(moto)], NO_STRIP_MODE, NO_STRIP_MODE);
    moto_update_palette(moto);
    // Free sprite
    imagen_destruir(sprite);
}

void gfx_update_road_over_canvas(gfx_t *gfx, imagen_t *road_img, const moto_t *moto, semaforo_t *semaforo) {

    // El desplazamiento del centro de la ruta va a ser la suma ur(v) = ul(v) + uc(v), la idea es
    // computar este vector de 96 posiciones una vez por cada instante dado que el mismo no sólo sirve
    // para dibujar la ruta sino, como ahora veremos, también sirve para posicionar el resto de los objetos
    // en la pantalla.
    // Cada 4 metros de ruta, cambia la paleta de colores (mirar ecuacion provista por el TP)

    /* ---------------------------------- Road ---------------------------------- */
    for (size_t v = 0, p_i = 0; v < imagen_get_alto(road_img); v++, p_i = (size_t)(moto_get_x(moto) + map_d(v)) % 4)
        imagen_pegar_con_paleta(gfx->cuadro, road_img, gfx->offset_lut[v] - 64, VENTANA_ALTO_ORIGINAL - 1 - v, colores_ruta[p_i], STRIP_MODE, ALTO_PUNTO_DE_FUGA - 1 - v);
    // El coeficiente de desplazamiento -64 es para centrar la ruta en la pantalla (obtenido empíricamente)

    /* ----------------------------- Road obstacles ----------------------------- */
    for (size_t i = 60; i > 0; i--) {

        size_t f_i = ruta[((size_t)moto_get_x(moto)) + i].indice_figura;
        if (f_i == NO_OBSTACLE) continue;

        // Isolate the figure
        figura_en_ruta_t f  = figuras_en_ruta[f_i];
        sprites_cfg_t f_cfg = road_sprites_cfg[f.figura];

        // Get the sprite
        imagen_t *f_act_sprite = gfx_get_figures(gfx, f_cfg);

        double v        = map_v(i);
        double resize_h = map_h(f_cfg.h, v, WITHOUT_CHECK);
        double resize_w = map_h(f_cfg.w, v, WITH_CHECK);

        imagen_t *f_act_sprite_escalado = imagen_escalar(f_act_sprite, resize_w, resize_h);
        imagen_destruir(f_act_sprite);

        if (f.reflejar) {
            imagen_t *flipped_sprite = imagen_espejar(f_act_sprite_escalado);
            imagen_destruir(f_act_sprite_escalado);
            f_act_sprite_escalado = flipped_sprite;
        }

        imagen_pegar_con_paleta(gfx->cuadro, f_act_sprite_escalado, map_u_p(f.y, v, resize_w, gfx->offset_lut), map_v_p(v, resize_h), paleta_4[f.paleta], NO_STRIP_MODE, 0);
        imagen_destruir(f_act_sprite_escalado);
    }
}

void gfx_update_semaforo_over_canvas(gfx_t *gfx, const moto_t *moto, semaforo_t *semaforo) {

    semaforo_update(semaforo, moto_get_x(moto));

    if (!semaforo_is_visible(semaforo)) return;

    for (semaforo_elements_t e = SEMAFORO_1; e <= BANNER; e++) {

        // Get sprite config
        const sprites_cfg_t sprite_cfg = semaforo_sprites_cfg[e];

        imagen_t *sprite_raw = gfx_get_figures(gfx, sprite_cfg);

        // Solo para la viga y el banner
        bool is_magic_sprite = e == VIGA_1 || e == VIGA_2 || e == BANNER;
        if (is_magic_sprite) {
            imagen_t *magic_sprite = imagen_generar(sprite_cfg.w, 249, 0);
            imagen_pegar(magic_sprite, sprite_raw, 0, 0);
            imagen_destruir(sprite_raw);
            sprite_raw = magic_sprite;
        }

        // Scale sprite_raw
        double v        = map_v(semaforo_get_distance(semaforo));
        double resize_h = map_h(is_magic_sprite ? 249 : sprite_cfg.h, v, WITHOUT_CHECK);
        double resize_w = map_h(sprite_cfg.w, v, WITH_CHECK);

        imagen_t *sprite_escalado = imagen_escalar(sprite_raw, resize_w, resize_h);
        imagen_destruir(sprite_raw);

        double u_p = map_u_p(sprite_cfg.img_y, v, resize_w, gfx->offset_lut);
        double v_p = map_v_p(v, resize_h);

        imagen_pegar_con_paleta(gfx->cuadro, sprite_escalado, u_p, v_p, paleta_4[semaforo_get_palette(semaforo, e)], NO_STRIP_MODE, NO_STRIP_MODE);
        imagen_destruir(sprite_escalado);
    }
}

void gfx_update_ui_over_canvas(gfx_t *gfx, const moto_t *moto, double time) {
    ui_update_text_over_canvas(gfx->cuadro, (const imagen_t **)gfx->teselas, moto, time);
}
