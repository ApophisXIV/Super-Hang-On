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
#include "../../inc/game_math.h"
#include "../../inc/paleta.h"

/* ------------------------------ Private enums ----------------------------- */

/* ------------------ Private data structures and typedefs ------------------ */

/* ---------------------- Private constants and macros ---------------------- */

/* ---------------------------- Private typedefs ---------------------------- */

/* -------------------- Private prototypes implementation ------------------- */

/* -------------------- Public prototypes implementation -------------------- */
// Getters
// Setters
// Utils

#include <stdio.h>    // ANCHOR DEBUG

void update_road_over_canvas(imagen_t *canvas, imagen_t *road_img, ruta_t *road, moto_t *moto) {

    // El desplazamiento del centro de la ruta va a ser la suma ur(v) = ul(v) + uc(v), la idea es
    // computar este vector de 96 posiciones una vez por cada instante dado que el mismo no sólo sirve
    // para dibujar la ruta sino, como ahora veremos, también sirve para posicionar el resto de los objetos
    // en la pantalla.

    // First, we get the offset table
    double offset_lut[ALTO_PUNTO_DE_FUGA];
    offset(offset_lut, road, moto_get_y(moto), moto_get_x(moto));

    // Cada 4 metros de ruta, cambia la paleta de colores
    // La paleta se debe aplicar cíclicamente cada 4 metros de esa posición.

    for (size_t v = 0, p_i = 0; v < imagen_get_alto(road_img); v++, p_i = (size_t)(moto_get_x(moto) + map_d(v)) % 4) {


        // Only we need "strips" of the image. The vertical position is given externally
        imagen_pegar_con_paleta(canvas, road_img, offset_lut[v], VENTANA_ALTO_ORIGINAL - 1 - v, colores_ruta[p_i], STRIP_MODE, ALTO_PUNTO_DE_FUGA - 1 - v);

        // imagen_pegar_fila_con_paleta(canvas, road_img, offset_lut[v], VENTANA_ALTO_ORIGINAL - 1 - v, colores_ruta[p_i], ALTO_PUNTO_DE_FUGA - 1 - v);


        // Cada metro de la ruta se colorea con cada una de las cuatro paletas
        // dadas, repitiendo los colores cada cuatro metros.
        // Al dibujar la ruta se sabe la posición que corresponde
        // al xm en ese instante, y para cada franja de la ruta se puede
        // computar xm + map_d(v) con lo cual se obtiene la posición exacta que se
        // corresponde con esa franja.

        // Stripe palette
        // palette_index = (int)(moto_get_x(moto) + map_d(v)) % 4;
    }
}
