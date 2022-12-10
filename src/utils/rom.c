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
#include "../../inc/rom.h"
#include "../../inc/config.h"

#include "../../inc/utils.h"    //ANCHOR - DEBUG

/* ---------------------- Private constants and macros ---------------------- */
#define ROM_SIZE            32768
#define ROM_ROAD_CHUNK_SIZE 8192
/* ------------------------------ Private enums ----------------------------- */
typedef enum {
    ROM_6819,
    ROM_6820,
    ROM_6821,
    ROM_6822,
    ROM_6823,
    ROM_6824,
    ROM_6825,
    ROM_6826,
    ROM_6827,
    ROM_6828,
    ROM_6829,
    ROM_6830,
    ROM_6845,
    ROM_6846,
} roms_figures_t;

typedef enum {
    ROM_6840,
} roms_road_t;

typedef enum {
    ROM_6841,
    ROM_6842,
    ROM_6843,
} roms_tiles_t;

// Figures
const char *rom_figures_path[] = {
    [ROM_6819] = "assets/roms/6819.rom",
    [ROM_6820] = "assets/roms/6820.rom",
    [ROM_6821] = "assets/roms/6821.rom",
    [ROM_6822] = "assets/roms/6822.rom",
    [ROM_6823] = "assets/roms/6823.rom",
    [ROM_6824] = "assets/roms/6824.rom",
    [ROM_6825] = "assets/roms/6825.rom",
    [ROM_6826] = "assets/roms/6826.rom",
    [ROM_6827] = "assets/roms/6827.rom",
    [ROM_6828] = "assets/roms/6828.rom",
    [ROM_6829] = "assets/roms/6829.rom",
    [ROM_6830] = "assets/roms/6830.rom",
    [ROM_6845] = "assets/roms/6845.rom",
    [ROM_6846] = "assets/roms/6846.rom",
};

// Road
const char *rom_road_path[] = {
    [ROM_6840] = "assets/roms/6840.rom",
};

// RGB Tiles
const char *rom_tiles_path[] = {
    [ROM_6841] = "assets/roms/6841.rom",
    [ROM_6842] = "assets/roms/6842.rom",
    [ROM_6843] = "assets/roms/6843.rom",
};

/* -------------------- Private prototypes implementation ------------------- */

/* -------------------- Public prototypes implementation -------------------- */
// Getters
// Setters
// Utils
bool rom_leer_teselas(imagen_t **im) {
    // Opening files
    FILE *fr = fopen(rom_tiles_path[ROM_6841], "rb");
    if (fr == NULL) return false;

    FILE *fg = fopen(rom_tiles_path[ROM_6842], "rb");
    if (fg == NULL) {
        fclose(fr);
        return false;
    }

    FILE *fb = fopen(rom_tiles_path[ROM_6843], "rb");
    if (fb == NULL) {
        fclose(fr);
        fclose(fg);
        return false;
    }

    for (size_t i = 0; i < CANTIDAD_TESELAS; i++) {
        for (int y = 0; y < imagen_get_alto(im[i]); y++) {

            uint8_t pixel_r, pixel_g, pixel_b;
            if (fread(&pixel_r, sizeof(uint8_t), 1, fr) != 1) {
                fclose(fr);
                fclose(fg);
                fclose(fb);
                return false;
            };

            if (fread(&pixel_g, sizeof(uint8_t), 1, fg) != 1) {
                fclose(fr);
                fclose(fg);
                fclose(fb);
                return false;
            }

            if (fread(&pixel_b, sizeof(uint8_t), 1, fb) != 1) {
                fclose(fr);
                fclose(fg);
                fclose(fb);
                return false;
            }

            for (int x = imagen_get_ancho(im[i]) - 1; x >= 0; x--) {

                pixel_t pixel_merge = (((pixel_r & 1) << 2)) |
                                      (((pixel_g & 1) << 1)) |
                                      (((pixel_b & 1) << 0));

                pixel_r = pixel_r >> 1;
                pixel_g = pixel_g >> 1;
                pixel_b = pixel_b >> 1;

                if (!imagen_set_pixel(im[i], x, y, pixel_merge)) {
                    fclose(fr);
                    fclose(fg);
                    fclose(fb);
                    return false;
                };
            }
        }
    }

    return true;
}

#define GET_BIT(byte, bit) ((byte >> (7 - bit)) & 1)

// PRE: Recibe un puntero imagen_t vacio
bool rom_leer_ruta(imagen_t **road_img) {

    // Auxiliar image
    imagen_t *im = imagen_generar(512, 128, 0);
    if (im == NULL) return false;

    // Opening file
    FILE *fp = fopen(rom_road_path[ROM_6840], "rb");
    if (fp == NULL) {
        imagen_destruir(im);
        return false;
    }

    // Read 4 chunks of 8192 bytes ("Memory Map 0x0000 - 0x7FFF")
    uint8_t buf[4 * ROM_ROAD_CHUNK_SIZE];
    if (fread(buf, sizeof(uint8_t), ROM_ROAD_CHUNK_SIZE * 4, fp) != ROM_ROAD_CHUNK_SIZE * 4) {
        fclose(fp);
        return false;
    }
    fclose(fp);

    size_t index_mem = 0;
    for (size_t y = 0; y < imagen_get_alto(im); y++) {
        for (size_t x = 0; x < imagen_get_ancho(im) / 8; x++, index_mem++) {

            uint8_t byte  = buf[index_mem];                              // Get the byte (0x0000 - 0x1FFF)
            uint8_t byte2 = buf[ROM_ROAD_CHUNK_SIZE + index_mem];        // Get the byte (0x2000 - 0x3FFF)
            uint8_t byte3 = buf[ROM_ROAD_CHUNK_SIZE * 2 + index_mem];    // Get the byte (0x4000 - 0x5FFF)
            uint8_t byte4 = buf[ROM_ROAD_CHUNK_SIZE * 3 + index_mem];    // Get the byte (0x6000 - 0x7FFF)

            for (size_t bit = 0; bit < 8; bit++)
                // Merge and set the pixel
                if (!imagen_set_pixel(im, x * 8 + bit, y, pixel4_crear(GET_BIT(byte4, bit), GET_BIT(byte3, bit), GET_BIT(byte2, bit), GET_BIT(byte, bit)))) {
                    imagen_destruir(im);
                    return false;
                }
        }
    }

    // Off-road transparent
    for (size_t y = 0; y < imagen_get_alto(im); y++) {
        for (size_t x = 0; x < imagen_get_ancho(im); x++) {
            if (imagen_get_pixel(im, x, y) != 0xf) break;
            if (!imagen_set_pixel(im, x, y, 0)) {
                imagen_destruir(im);
                return false;
            }
        }
    }

    // NOTE -  image to raw_road.txt (Con la informacion de los valores posibles de la ruta construí la paleta de colores)
    // 4 - 5 - 7 - 12 - 13 - 15
    // 4 y 5 son los cuadros rojo y blanco de la ruta
    // 7 es la linea lateral de pista
    // 12 y 13 son el asfalto
    // 15 es la linea de carril
    // Con esto me doy cuenta que la paleta va a tener que tener 16 colores [0 - 15]

    // FILE *f = fopen("docs/raw_road.txt", "w");
    // for (size_t y = 0; y < imagen_get_alto(im); y++) {
    //     for (size_t x = 0; x < imagen_get_ancho(im); x++) {
    //         fprintf(f, "%d ", imagen_get_pixel(im, x, y));
    //     }
    //     fprintf(f, "\n");
    // }

    imagen_t *ruta_img = imagen_generar(512, 96, 0);
    if (ruta_img == NULL) {
        imagen_destruir(im);
        return false;
    }

    // NOTE - No utilizo las coordenadas provistas directamente por el TP pues no se ajustan a la imagen recortandola por izquierda
    // Las coordenadas -284 y -63 fueron obtenidas empiricamente de la siguiente manera:
    // 1. Se dibujó la imagen de la ruta sin espejar
    // 2. Se la escribio en un archivo de texto
    // 3. Se contaron cuantos pixeles hay por izquierda y se desplazaron las coordenadas de la imagen de la ruta
    // 4. Se dibujó la imagen de la ruta espejada
    // 5. Se repitió el proceso
    imagen_pegar(ruta_img, im, -284, ALTO_PUNTO_DE_FUGA - 1 - VENTANA_ALTO_ORIGINAL / 2);    // Recorta las primeras 16 filas de la imagen (mirar la imagen docs/road_raw.ppm para entender que hay que recortar)
    imagen_pegar(ruta_img, imagen_espejar(ruta_img), -63, 0);

    // NOTE - DEBUG de la imagen
    // dump_image_to_ppm(ruta_img, "docs/ruta.ppm");
    // dump_image_to_txt(ruta_img, "docs/ruta.txt");

    imagen_destruir(im);

    *road_img = ruta_img;

    return true;
}

bool rom_leer_figuras_raw(uint16_t *buf_figs) {

    for (roms_figures_t rom = ROM_6819; rom <= ROM_6846; rom += 2) {

        // Opening ROM pair
        fprintf(stderr, "Opening ROM pair [LS] %s [HS] %s ... \n", rom_figures_path[rom], rom_figures_path[rom + 1]);
        FILE *rom_ls = fopen(rom_figures_path[rom], "rb");
        if (rom_ls == NULL) return false;

        FILE *rom_hs = fopen(rom_figures_path[rom + 1], "rb");
        if (rom_hs == NULL) {
            fclose(rom_ls);
            return false;
        }

        // Reading ROM pair
        for (size_t i = 0; i < ROM_SIZE; i++) {

            uint8_t byte_ls, byte_hs;
            if (fread(&byte_ls, sizeof(uint8_t), 1, rom_ls) != 1) {
                fclose(rom_ls);
                fclose(rom_hs);
                return false;
            }
            if (fread(&byte_hs, sizeof(uint8_t), 1, rom_hs) != 1) {
                fclose(rom_ls);
                fclose(rom_hs);
                return false;
            }

            buf_figs[i + (rom - ROM_6819) * ROM_SIZE] = (byte_hs << 8) | byte_ls;
        }

        fclose(rom_ls);
        fclose(rom_hs);
    }

    return true;
}
