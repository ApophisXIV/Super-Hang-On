
#include "../../../inc/game_math.h"
#include <math.h>

double map_v(double d) {
    // Notar que objetos con  están detrás de la visión y no dan valores válidos de  mientras que lo demás está entre 0 y 95.
    // Restringiremos el campo de visión a 60 metros, por lo que todo lo que tenga
    if (d > 60) return INFINITY; // TODO revisar esto
    return 96.0 - 96.0 * exp(-0.11 * d);
}

double map_d(double v) {
    return -log((96.0 - v) / 96.0) * (1.0 / 0.11);
}

double map_h(double h0, double v) {
    double h = h0 * (((96.0 - v) / 96.0) + (5.0 * v / 96.0));
    return h < 3.0 ? 3.0 : h;
}

// La idea es mapear los efectos de desplazamiento lateral y curvatura de la ruta
// por separado, y luego "sumarlos" para obtener el desplazamiento total.
// Para esto, hay que mapear el efecto sobre todo el alto del subespacio de visión (0 a 95)
static void lateral_offset(double ym, double *offset_table) {
    for (size_t i = 0; i < 96; i++)
        offset_table[i] = -ym * ((96.0 - i) / 96.0);
}

static void curve_offset(double *offset_table, const ruta_t *ruta, double xm) {
    offset_table[0] = 0;
    for (size_t i = 1; i < 96; i++)
        offset_table[i] = offset_table[i - 1] + ruta[(int)(xm + map_d(i))].radio_curva * exp(0.105 * i - 8.6);
}

void offset(double *offset_table, const ruta_t *ruta, double ym, double xm) {
    // Merge the effects of lateral offset and curve offset into a single offset table
    double lateral[96];
    double curve[96];
    lateral_offset(ym, lateral);
    curve_offset(curve, ruta, xm);

    for (size_t i = 0; i < 96; i++)
        offset_table[i] = lateral[i] + curve[i];
}