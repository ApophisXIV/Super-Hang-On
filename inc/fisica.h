#ifndef FISICA_H
#define FISICA_H

#include "moto.h"
#include "ruta.h"
#include <stdbool.h>
#include <stddef.h>

// TODO - Revisar visibilidad de funciones

double posicion_x(double velocidad, double tiempo);
double aceleracion(double velocidad, double tiempo);
double frenado(double velocidad, double tiempo);
double desaceleracion(double velocidad, double tiempo);

double morder_banquina(double y, double velocidad);

int giro_derecha(int intensidad);
int giro_izquierda(int intensidad);
int reposo(int intensidad);

double posicion_y(double y, int intensidad, double radio_ruta, double velocidad);

double puntaje(double y, double velocidad, double tiempo, double puntos);
bool ganar(double posicion_x);
bool perder(double tiempo);
bool choque(double x, double y, double ancho);

void update_physics(moto_t *moto, ruta_t *ruta, double tiempo_total);

#endif    // FISICA_H