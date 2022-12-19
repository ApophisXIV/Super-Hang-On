#include "ui.h"
#include "../../inc/config.h"
#include "../../inc/figuras.h"
#include "../../inc/paleta.h"

#include <stdio.h>

#define TYPO_W 8
#define TYPO_H 8

typedef struct {
    bool is_tessellation;
    const char *text;

    size_t x, y, w, h;

    const pixel_t *palette;
} ui_element_cfg_t;

static void str_to_text_img(imagen_t *canvas, ui_element_cfg_t e, const imagen_t **tiles) {

    if (e.is_tessellation) {
        imagen_t *t = generar_mosaico(tiles, NOT_USED, e.h, e.w, NOT_USED, NOT_USED, true, e.text);
        imagen_pegar_con_paleta(canvas, t, e.x - TYPO_W, e.y - TYPO_H, e.palette, NO_STRIP_MODE, NO_STRIP_MODE);
        imagen_destruir(t);
        return;
    }

    for (size_t i = 0; e.text[i] != '\0'; i++)
        imagen_pegar_con_paleta(canvas, tiles[(uint8_t)e.text[i]], i * TYPO_W + e.x, e.y, e.palette, NO_STRIP_MODE, NO_STRIP_MODE);
}

typedef enum {
    TOP_LABEL,
    FAKE_TOP,
    TIME_LABEL,
    SCORE_LABEL,
    STAGE_LABEL,
    STAGE,
    SPEED_LABEL,
    KM_LABEL,
    TIME,
    SCORE,
    SPEED,
    GAME_OVER,
    GOAL,
    // AUTHOR_LABEL,
    // AUTHOR,
} ui_element_t;

static ui_element_cfg_t ui_elements_lut[] = {
    // NOTE: Las paletas las encontre a base de prueba y error, al igual que las posiciones de los elementos
    [TOP_LABEL]   = {.text = "94959595969798999a9b9c9d9d9d9e", .x = 24, .y = 16, .w = 5, .h = 3, .palette = paleta_3[5], .is_tessellation = true},
    [FAKE_TOP]    = {.text = "1000000", .x = 64, .y = 16, .palette = paleta_3[8], .is_tessellation = false},
    [TIME_LABEL]  = {.text = "a0a1a1a1a1a2a3a4a5a6a7a8a9aaaaaaaaab", .x = 136, .y = 16, .w = 6, .h = 3, .palette = paleta_3[5], .is_tessellation = true},
    [TIME]        = {.x = 152, .y = 40, .w = 2, .h = 2, .palette = paleta_3[5], .is_tessellation = true},
    [SCORE_LABEL] = {.text = "b0b1b1b1b1b1b2b3b4b5b6b7b8b9babbbbbbbbbbbc", .x = 192, .y = 16, .w = 7, .h = 3, .palette = paleta_3[5], .is_tessellation = true},
    [SCORE]       = {.x = 256, .y = 16, .palette = paleta_3[7]},
    [STAGE_LABEL] = {.text = "STAGE", .x = 24, .y = 32, .palette = paleta_3[5]},
    [STAGE]       = {.text = " 1", .x = 64, .y = 32, .palette = paleta_3[6]},
    [SPEED_LABEL] = {.text = "SPEED", .x = 200, .y = 32, .palette = paleta_3[6]},
    [SPEED]       = {.x = 256, .y = 32, .palette = paleta_3[6]},
    [KM_LABEL]    = {.text = "KM", .x = 288, .y = 32, .palette = paleta_3[6]},
    [GAME_OVER]   = {.text = "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f", .x = 104, .y = 88, .w = 16, .h = 2, .palette = paleta_3[5], .is_tessellation = true},
    [GOAL]        = {.text = "e0e1e2e3e4e5e6e7e8e9eaebecedeeef", .x = 136, .y = 88, .w = 8, .h = 2, .palette = paleta_3[5], .is_tessellation = true},
    // [AUTHOR_LABEL] = {.text = "MADE BY ", .x = 24, .y = 208, .palette = paleta_3[3]},
    // [AUTHOR]      = {.text = "GUIDO", .x = 80, .y = 208, .palette = paleta_3[3]},
};

static char *number_to_str(char *buffer, size_t n) {
    sprintf(buffer, "%zu", n);
    return buffer;
}

static char *time_to_str(char *buffer, size_t n) {
    static const char *lut_hs[] = {"80", "82", "84", "86", "88", "8a", "8c", "8e", "90", "92"};
    static const char *lut_ls[] = {"81", "83", "85", "87", "89", "8b", "8d", "8f", "91", "93"};
    const size_t d = n / 10, u = n % 10;
    sprintf(buffer, "%s%s%s%s", lut_hs[d], lut_hs[u], lut_ls[d], lut_ls[u]);
    return buffer;
}

void ui_update_text_over_canvas(imagen_t *canvas, const imagen_t **teselas, const moto_t *moto, size_t time) {
    char aux_buffer[10];
    for (ui_element_t e = TOP_LABEL; e < sizeof(ui_elements_lut) / sizeof(ui_element_cfg_t); e++) {
        if (e == TIME) ui_elements_lut[e].text = time_to_str(aux_buffer, MAX_TIEMPO_JUEGO - time);
        if (e == SCORE) ui_elements_lut[e].text = number_to_str(aux_buffer, moto_get_points(moto));
        if (e == SPEED) ui_elements_lut[e].text = number_to_str(aux_buffer, moto_get_velocity(moto));
        if ((e == GAME_OVER && !moto_perdio(moto)) || (e == GOAL && !moto_gano(moto))) continue;
        str_to_text_img(canvas, ui_elements_lut[e], teselas);
    }
}