#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../inc/config.h"
#include "../inc/figuras.h"
#include "../inc/fisica.h"
#include "../inc/fondo.h"
#include "../inc/gfx.h"
#include "../inc/imagen.h"
#include "../inc/moto.h"
#include "../inc/paleta.h"
#include "../inc/rom.h"
#include "../inc/ruta.h"

bool cargar_pasto(imagen_t *cuadro) {
    // TODO Mejorar la "estética" de la función (tal vez ponerla en el modulo de gfx.c)
    imagen_t *pasto           = imagen_generar(1, 96, pixel12_crear(0, 13, 9));
    pixel_t colores_pasto[10] = {0x089, 0x099, 0x099, 0x0a9, 0x0a9, 0x0a9, 0x0b9, 0x0b9, 0x0c9, 0x0c9};
    for (size_t i = 0; i < 10; i++)
        imagen_set_pixel(pasto, 0, i, colores_pasto[i]);

    imagen_t *pasto_estirado = imagen_escalar(pasto, 320, 96);
    imagen_destruir(pasto);
    imagen_pegar(cuadro, pasto_estirado, 0, 128);
    imagen_destruir(pasto_estirado);
    return true;
}
bool cargar_teselas(imagen_t **teselas) {
    // TODO Mejorar la "estética" de la función (tal vez ponerla en el modulo de gfx.c)
    for (size_t i = 0; i < CANTIDAD_TESELAS; i++)
        teselas[i] = imagen_generar(ANCHO_TESELA, ALTO_TESELA, 0);

    if (!rom_leer_teselas(teselas)) {
        for (size_t i = 0; i < CANTIDAD_TESELAS; i++)
            imagen_destruir(teselas[i]);
        return false;
    }
    return true;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Hang-On");

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB444, SDL_TEXTUREACCESS_STATIC, VENTANA_ANCHO, VENTANA_ALTO);
    uint16_t canvas[VENTANA_ALTO * VENTANA_ANCHO];

    int dormir = 0;

    /* -------------------------------------------------------------------------- */

    // BEGIN código del alumno
    double total_time = 0;

    /* ----------------------------- Assets Loading ----------------------------- */

    uint16_t buffer_figuras[N_VALORES_ROM];
    if (!rom_leer_figuras_raw(buffer_figuras)) return 1;

    imagen_t *teselas[CANTIDAD_TESELAS];
    if (!cargar_teselas(teselas)) return 1;

    moto_t *moto_jugador = moto_crear();
    if (moto_jugador == NULL) return 1;

    imagen_t *ruta_img;
    if (!rom_leer_ruta(&ruta_img)) {
        moto_destruir(moto_jugador);
        return 1;
    }

    // END código del alumno

    /* -------------------------------------------------------------------------- */

    unsigned int ticks = SDL_GetTicks();
    while (1) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;
            /* -------------------------------------------------------------------------- */

            // BEGIN código del alumno
            if (event.type == SDL_KEYDOWN) {
                // Se apretó una tecla
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    moto_set_vel_state(moto_jugador, MOTO_ACELERANDO, true);
                    break;
                case SDLK_DOWN:
                    moto_set_vel_state(moto_jugador, MOTO_FRENANDO, true);
                    break;
                case SDLK_RIGHT:
                    moto_set_dir_state(moto_jugador, MOTO_GIRANDO_DERECHA, true);
                    break;
                case SDLK_LEFT:
                    moto_set_dir_state(moto_jugador, MOTO_GIRANDO_IZQUIERDA, true);
                    break;
                }
            } else if (event.type == SDL_KEYUP) {
                // Se soltó una tecla
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    moto_set_vel_state(moto_jugador, MOTO_ACELERANDO, false);
                    break;
                case SDLK_DOWN:
                    moto_set_vel_state(moto_jugador, MOTO_FRENANDO, false);
                    break;
                case SDLK_RIGHT:
                    moto_set_dir_state(moto_jugador, MOTO_GIRANDO_DERECHA, false);
                    break;
                case SDLK_LEFT:
                    moto_set_dir_state(moto_jugador, MOTO_GIRANDO_IZQUIERDA, false);
                    break;
                }
            }
            // END código del alumno

            /* -------------------------------------------------------------------------- */
            continue;
        }
        /* -------------------------------------------------------------------------- */
        /*                                   Fisicas                                  */
        /* -------------------------------------------------------------------------- */
        total_time += DELTA_T;
        update_physics(moto_jugador, ruta, total_time);

        // Llego al final de la ruta
        if (moto_get_x(moto_jugador) >= 4200) {
            printf("Llegaste al final de la ruta! (En esta parte detecta que ganaste. Hay que laburar esto todavia)\n");
            break;
        }

        /* -------------------------------------------------------------------------- */

        /* -------------------------------------------------------------------------- */
        /*                                  Graficos                                  */
        /* -------------------------------------------------------------------------- */

        /* --------------------------------- Lienzo --------------------------------- */
        imagen_t *cuadro = imagen_generar(320, 224, 0xf);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Fondo --------------------------------- */
        gfx_update_bg_over_canvas(cuadro, ruta, moto_jugador, (const imagen_t **) teselas);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Pasto --------------------------------- */
        cargar_pasto(cuadro);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Ruta ---------------------------------- */
        gfx_update_road_over_canvas(cuadro, ruta_img, ruta, moto_jugador);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Moto ---------------------------------- */
        gfx_update_sprite_over_canvas(cuadro, moto_jugador, buffer_figuras);
        /* -------------------------------------------------------------------------- */

        /* --------------------------------- Canvas --------------------------------- */
        // Procedemos a dibujar a pantalla completa:
        imagen_t *cuadro_escalado = imagen_escalar(cuadro, VENTANA_ANCHO, VENTANA_ALTO);
        imagen_a_textura(cuadro_escalado, canvas);
        imagen_destruir(cuadro_escalado);
        imagen_destruir(cuadro);
        // END código del alumno
        /* -------------------------------------------------------------------------- */

        SDL_UpdateTexture(texture, NULL, canvas, VENTANA_ANCHO * sizeof(uint16_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if (dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        } else if (ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        else
            printf("Perdiendo cuadros\n");
        ticks = SDL_GetTicks();
    }

    // BEGIN código del alumno
    // No tengo nada que destruir.
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
