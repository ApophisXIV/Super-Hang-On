#include "../../../inc/fisica.h"
#include "../../../inc/config.h"
#include "../../../inc/gfx_cfg.h"
#include <math.h>

#define CRUISE_SPEED 80
#define MAX_SPEED    279

#define OFF_ROAD_LIMIT 435

#define ORIGEN_X  0
#define KMH_TO_MS 1.0 / 3.6

static const int intensidades_de_giro[] = {0, 3, 9, 15};

static double posicion_x(double velocidad, double tiempo) {
    if ((velocidad) < 0) return ORIGEN_X;
    return velocidad * tiempo;
}

static double aceleracion(double velocidad, double tiempo) {
    return (MAX_SPEED - (MAX_SPEED - velocidad) * exp(-0.224358 * tiempo));
}
static double frenado(double velocidad, double tiempo) {
    if (velocidad > CRUISE_SPEED) return velocidad - 300 * tiempo;
    return velocidad;
}
static double desaceleracion(double velocidad, double tiempo) {
    if (velocidad < CRUISE_SPEED) return aceleracion(velocidad, tiempo);
    if (velocidad > CRUISE_SPEED) return velocidad - 90.0 * tiempo;
    return velocidad;
}

static double morder_banquina(double y, double velocidad) {
    if (fabs(y) > 215 && velocidad > 92) return velocidad - 3;
    return velocidad;
}

static int giro_derecha(int intensidad) {
    if (intensidad + 1 > 3) return 3;
    return intensidad + 1;
}
static int giro_izquierda(int intensidad) {
    if (intensidad - 1 < -3) return -3;
    return intensidad - 1;
}
static int reposo(int intensidad) {
    // Se tiene que llevar a 0 la intensidad
    if (intensidad > 0) return --intensidad;
    if (intensidad < 0) return ++intensidad;
    return intensidad;
}

static double posicion_y(double y, int intensidad, double radio_ruta, double velocidad) {

    double new_y = y + intensidades_de_giro[intensidad <= 0 ? -intensidad : intensidad] * (intensidad >= 0 ? 1 : -1);

    // Affected by the road curvature ("centripetal force") ...
    new_y = new_y - 2.5 * posicion_x(KMH_TO_MS * velocidad, DELTA_T) * radio_ruta;

    // Has gone too far (off-road)? ...
    if (new_y > OFF_ROAD_LIMIT) new_y = OFF_ROAD_LIMIT;
    if (new_y < -OFF_ROAD_LIMIT) new_y = -OFF_ROAD_LIMIT;

    return new_y;
}

static double puntaje(double y, double velocidad, double tiempo, double puntos) {
    if (fabs(y) > 215.0) return puntos;
    if (velocidad < 117.0) return puntos + 125.0 * posicion_x(velocidad, tiempo);
    return puntos + posicion_x(velocidad, tiempo) * (3.13 * velocidad - 240.0);
}

static bool ganar(double posicion_x) {
    return posicion_x >= 4200.0;
}
static bool perder(double tiempo) {
    return tiempo >= MAX_TIEMPO_JUEGO;
}
static bool choque(double xm, double ym) {
    // Si la posición de la moto está entre el lado izquierdo y el derecho de una
    // figura(no nos olvidemos de que la y de las figuras está en el centro) y la
    // figura está a menos de (77.5 * DELTA_T + 1) metros de distancia de la
    // moto se produce un choque. Cuando se choca la moto vuelve a ym = 0 con
    // velocidad 0 y el juego se queda detenido durante 5 segundos.

    // (Esta distancia en metros se toma siendo la velocidad máxima
    // de 279 km / h, por ejemplo a una tasa de 30 cuadros por segundo
    // esta cuenta da 3 metros siendo que la moto recorre
    // como máximo 2, 58 metros por cuadro.)

    size_t f_i = ruta[(size_t)(xm)].indice_figura;
    if (f_i == NO_OBSTACLE) return false;

    figura_en_ruta_t f = figuras_en_ruta[f_i];

    // Check if the bike is in range of the figure
    if (fabs(ym - f.y) < road_sprites_cfg[f.figura].w / 2.0) return true;
    return false;
}

// Tabla de funciones
typedef int (*dir_action_t)(int);
static dir_action_t dir_action_lut[] = {
    [MOTO_REPOSO]            = reposo,
    [MOTO_GIRANDO_IZQUIERDA] = giro_izquierda,
    [MOTO_GIRANDO_DERECHA]   = giro_derecha,
};

typedef double (*vel_action_t)(double, double);
static vel_action_t vel_action_lut[] = {
    [MOTO_NEUTRAL]    = desaceleracion,
    [MOTO_ACELERANDO] = aceleracion,
    [MOTO_FRENANDO]   = frenado,
};

// ANCHOR - DEBUG
// #include <stdio.h>
// const char *state_vel_a_string(int estado) {
//     switch (estado) {
//     case MOTO_ACELERANDO: return "MOTO_ACELERANDO";
//     case MOTO_FRENANDO: return "MOTO_FRENANDO";
//     case MOTO_NEUTRAL: return "MOTO_NEUTRAL";
//     default: return "ESTADO_INVALIDO";
//     }
// }
// const char *state_dir_a_string(int estado) {
//     switch (estado) {
//     case MOTO_GIRANDO_DERECHA: return "MOTO_GIRANDO_DERECHA";
//     case MOTO_GIRANDO_IZQUIERDA: return "MOTO_GIRANDO_IZQUIERDA";
//     case MOTO_REPOSO: return "MOTO_REPOSO";
//     default: return "ESTADO_INVALIDO";
//     }
// }
// const char *gano_o_perdio(bool gano, bool perdio) {
//     if (gano) return "GANO";
//     if (perdio) return "PERDIO";
//     return "JUGANDO";
// }

void phy_update_physics(moto_t *moto, double tiempo_total) {

    // ANCHOR - DEBUG - Borrar despues (te recomiendo dejarlo hasta terminar todo)
    // fprintf(stderr, "t_tot: %1.1f | vel: %01.1f | x: %1.0f | y: %1.0f | col: %s | giro: %d  | vel: %s | dir: %s | game: %s | pal: %d\n",
    //         tiempo_total,
    //         moto_get_velocity(moto),
    //         moto_get_x(moto),
    //         moto_get_y(moto),
    //         moto_is_colision(moto) ? "CHOCO" : "TODO BIEN",
    //         moto_get_spin_intensity(moto),
    //         state_vel_a_string(moto_get_vel_state(moto)),
    //         state_dir_a_string(moto_get_dir_state(moto)),
    //         gano_o_perdio(moto_gano(moto), moto_perdio(moto)),
    //         moto_get_palette(moto));

    static double timer = 0.0;
    if (moto_is_colision(moto) && timer < 5.0) {
        timer += DELTA_T;
        moto_set_velocity(moto, 0.0);
        moto_set_y(moto, moto_get_y(moto) - moto_get_y(moto) / (5.0 * 5.0 + 1.0));
        return;
    }
    // Velocidad modificada por el estado de la moto
    moto_set_velocity(moto, vel_action_lut[moto_get_vel_state(moto)](moto_get_velocity(moto), DELTA_T));

    // Velocidad modificada por la banquina
    moto_set_velocity(moto, morder_banquina(moto_get_y(moto), moto_get_velocity(moto)));

    moto_set_spin_intensity(moto, dir_action_lut[moto_get_dir_state(moto)](moto_get_spin_intensity(moto)));

    moto_set_x(moto, moto_get_x(moto) + posicion_x(KMH_TO_MS * moto_get_velocity(moto), DELTA_T));
    moto_set_y(moto, posicion_y(moto_get_y(moto), moto_get_spin_intensity(moto), ruta[(size_t)moto_get_x(moto)].radio_curva, moto_get_velocity(moto)));

    moto_set_points(moto, puntaje(moto_get_y(moto), KMH_TO_MS * moto_get_velocity(moto), DELTA_T, moto_get_points(moto)));

    moto_set_gano(moto, ganar(moto_get_x(moto)));

    moto_set_perdio(moto, perder(tiempo_total));

    moto_set_colision(moto, choque(moto_get_x(moto), moto_get_y(moto)));

    timer = 0.0;
}
