#include "../../../inc/fisica.h"
#include "../../../inc/config.h"
#include <math.h>

#define CRUISE_SPEED 80
#define MAX_SPEED    279

#define OFF_ROAD_LIMIT 435

#define ORIGEN_X  0
#define KMH_TO_MS 1.0 / 3.6

const int intensidades_de_giro[] = {0, 3, 9, 15};

// PRE: velocidad should be in m/s
double posicion_x(double velocidad, double tiempo) {
    if (velocidad < 0) return ORIGEN_X;
    return velocidad * tiempo;
}

double aceleracion(double velocidad, double tiempo) {
    return (MAX_SPEED - (MAX_SPEED - velocidad) * exp(-0.224358 * tiempo));
}
double frenado(double velocidad, double tiempo) {
    if (velocidad > CRUISE_SPEED) return velocidad - 300 * tiempo;
    return velocidad;
}
double desaceleracion(double velocidad, double tiempo) {
    if (velocidad < CRUISE_SPEED) return aceleracion(velocidad, tiempo);
    if (velocidad >= CRUISE_SPEED) return velocidad - 90.0 * tiempo;
    return velocidad;
}

double morder_banquina(double y, double velocidad) {
    if (fabs(y) > 215 && velocidad > 92) return velocidad - 3;
    return velocidad;
}

int giro_derecha(int intensidad) {
    if (++intensidad > 3) intensidad = 3;
    return intensidad;
}
int giro_izquierda(int intensidad) {
    if (--intensidad < -3) intensidad = -3;
    return intensidad;
}
int reposo(int intensidad) {
    // Se tiene que llevar a 0 la intensidad
    if (intensidad > 0) return --intensidad;
    if (intensidad < 0) return ++intensidad;
    return intensidad;
}

double posicion_y(double y, int intensidad, double radio_ruta, double velocidad) {

    double new_y = y + intensidades_de_giro[intensidad <= 0 ? -intensidad : intensidad] * (intensidad >= 0 ? 1 : -1);

    // Affected by the road curvature ("centripetal force") ...
    new_y = new_y - 2.5 * posicion_x(KMH_TO_MS * velocidad, DELTA_T) * radio_ruta;

    // Has gone too far (off-road)? ...
    if (new_y > OFF_ROAD_LIMIT) new_y = OFF_ROAD_LIMIT;
    if (new_y < -OFF_ROAD_LIMIT) new_y = -OFF_ROAD_LIMIT;

    return new_y;
}

double puntaje(double y, double velocidad, double tiempo, double puntos) {
    if (fabs(y) > 215) return puntos;
    if (velocidad < 117) return puntos + 125 * posicion_x(velocidad, tiempo);
    return puntos + posicion_x(velocidad, tiempo) * (3.13 * velocidad - 240);
}

bool ganar(double posicion_x) {
    return posicion_x >= 4200;
}
// revisar
bool perder(double tiempo) {
    return tiempo >= 75;
}
// falta terminar
bool choque(double x, double y, double ancho) {
    return true;    // falta teminar
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
#include <stdio.h>
const char *state_vel_a_string(int estado) {
    switch (estado) {
    case MOTO_ACELERANDO: return "MOTO_ACELERANDO";
    case MOTO_FRENANDO: return "MOTO_FRENANDO";
    case MOTO_NEUTRAL: return "MOTO_NEUTRAL";
    default: return "ESTADO_INVALIDO";
    }
}
const char *state_dir_a_string(int estado) {
    switch (estado) {
    case MOTO_GIRANDO_DERECHA: return "MOTO_GIRANDO_DERECHA";
    case MOTO_GIRANDO_IZQUIERDA: return "MOTO_GIRANDO_IZQUIERDA";
    case MOTO_REPOSO: return "MOTO_REPOSO";
    default: return "ESTADO_INVALIDO";
    }
}
const char *gano_o_perdio(bool gano, bool perdio) {
    if (gano) return "GANO";
    if (perdio) return "PERDIO";
    return "JUGANDO";
}

void update_physics(moto_t *moto, ruta_t *ruta, double tiempo_total) {


    // Velocidad modificada por el estado de la moto
    moto_set_velocity(moto, vel_action_lut[moto_get_vel_state(moto)](moto_get_velocity(moto), DELTA_T));

    // Velocidad modificada por la banquina
    moto_set_velocity(moto, morder_banquina(moto_get_y(moto), moto_get_velocity(moto)));

    moto_set_spin_intensity(moto, dir_action_lut[moto_get_dir_state(moto)](moto_get_spin_intensity(moto)));

    moto_set_x(moto, moto_get_x(moto) + posicion_x(KMH_TO_MS * moto_get_velocity(moto), DELTA_T));
    moto_set_y(moto, posicion_y(moto_get_y(moto), moto_get_spin_intensity(moto), ruta[(size_t)moto_get_x(moto)].radio_curva, moto_get_velocity(moto)));

    moto_set_points(moto, puntaje(moto_get_y(moto), moto_get_velocity(moto), DELTA_T, moto_get_points(moto)));

    moto_set_gano(moto, ganar(moto_get_x(moto)));

    moto_set_perdio(moto, perder(tiempo_total));

    // TODO: Implement (crash)

    // ANCHOR - DEBUG
    printf("t_tot: %1.1f | vel: %01.1f | x: %1.0f | y: %1.0f | puntos: %1.0f | giro: %d  | state_vel: %s | state_dir: %s | state_game: %s\n", tiempo_total, moto_get_velocity(moto), moto_get_x(moto), moto_get_y(moto), moto_get_points(moto), moto_get_spin_intensity(moto), state_vel_a_string(moto_get_vel_state(moto)), state_dir_a_string(moto_get_dir_state(moto)), gano_o_perdio(moto_is_gano(moto), moto_is_perdio(moto)));
}