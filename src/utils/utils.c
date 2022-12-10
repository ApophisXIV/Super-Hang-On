#include "utils.h"

void dump_image_to_txt(const imagen_t *im, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) return;

    for (size_t y = 0; y < imagen_get_alto(im); y++) {
        for (size_t x = 0; x < imagen_get_ancho(im); x++)
            fprintf(f, "%d ", imagen_get_pixel(im, x, y));
        fprintf(f, "\n");
    }
}

void dump_image_to_ppm(const imagen_t *im, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) return;

    fprintf(f, "P3 %zu %zu 255\n", imagen_get_ancho(im), imagen_get_alto(im));
    for (size_t y = 0; y < imagen_get_alto(im); y++) {
        for (size_t x = 0; x < imagen_get_ancho(im); x++) {
            pixel_t pixel = imagen_get_pixel(im, x, y);
            fprintf(f, "%d %d %d ", pixel, pixel, pixel);
        }
        fprintf(f, "\n");
    }
}