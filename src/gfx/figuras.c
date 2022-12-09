#include "../../inc/pixel.h"
#include "../../inc/rom.h"

#define IS_NEW_LINE_FLAG(w)      ((w & 0xf) == 0xf)
#define FIRST_PIXEL(w)           ((w >> 12) & 0xf)
#define SHOULD_BE_TRANSPARENT(w) ((w & 0xf) == 0xf)
#define TRANSPARENT              0

imagen_t *get_figures(uint16_t *buf_figs, size_t n_offset, size_t w, size_t h) {

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
