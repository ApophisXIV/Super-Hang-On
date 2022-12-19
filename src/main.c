#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../inc/config.h"
#include "../inc/fisica.h"
#include "../inc/gfx.h"
#include "../inc/imagen.h"
#include "../inc/moto.h"
#include "../inc/rom.h"
#include "../inc/ruta.h"
#include "../inc/semaforo.h"

void destruir_teselas(imagen_t **teselas) {
    for (size_t i = 0; i < CANTIDAD_TESELAS; i++)
        imagen_destruir(teselas[i]);
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

    moto_t *moto_jugador = moto_crear();
    if (moto_jugador == NULL) {
        return 1;
    }

    semaforo_t *semaforo = semaforo_crear();
    if (semaforo == NULL) {
        moto_destruir(moto_jugador);
        return 1;
    }

    imagen_t *ruta_img;
    if (!rom_leer_ruta(&ruta_img)) {
        moto_destruir(moto_jugador);
        semaforo_destruir(semaforo);
        return 1;
    }

    gfx_t *gfx = gfx_crear(canvas);
    if (gfx == NULL) {
        moto_destruir(moto_jugador);
        semaforo_destruir(semaforo);
        imagen_destruir(ruta_img);
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
        if (moto_gano(moto_jugador) || moto_perdio(moto_jugador)) continue;

        if (semaforo_has_started(semaforo)) {
            total_time += DELTA_T;
            phy_update_physics(moto_jugador, total_time);
        }
        /* -------------------------------------------------------------------------- */

        /* -------------------------------------------------------------------------- */
        /*                                  Graficos                                  */
        /* -------------------------------------------------------------------------- */

        /* --------------------------------- Lienzo --------------------------------- */
        gfx_update_math(gfx, moto_jugador);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Fondo --------------------------------- */
        gfx_update_bg_over_canvas(gfx, moto_jugador);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Pasto --------------------------------- */
        gfx_update_grass_over_canvas(gfx);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Ruta ---------------------------------- */
        gfx_update_road_over_canvas(gfx, ruta_img, moto_jugador, semaforo);
        /* -------------------------------------------------------------------------- */

        /* ---------------------------------- Moto ---------------------------------- */
        gfx_update_bike_over_canvas(gfx, moto_jugador);
        /* -------------------------------------------------------------------------- */

        /* -------------------------------- Semaforo -------------------------------- */
        gfx_update_semaforo_over_canvas(gfx, moto_jugador, semaforo);
        /* -------------------------------------------------------------------------- */

        /* ----------------------------------- UI ----------------------------------- */
        gfx_update_ui_over_canvas(gfx, moto_jugador, total_time);
        /* -------------------------------------------------------------------------- */

        /* --------------------------------- Canvas --------------------------------- */
        gfx_display(gfx);
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
    moto_destruir(moto_jugador);
    imagen_destruir(ruta_img);
    semaforo_destruir(semaforo);
    gfx_destruir(gfx);
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
