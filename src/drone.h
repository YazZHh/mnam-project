#ifndef DRONE_H
#define DRONE_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "state.h"

#define BATTERY_FULL 100

Drone* init_drone(Grille *g);

Drone* init_drone_state(Grille *g, State *s);

void afficher_grille_drone(Drone *d);

void takeoff_cmd(Drone *d);

void move_step(Drone *d, int nextX, int nextY);

Case** moves_possibles(Drone *d, int *nbCases);

void avoid_maneuver(Drone *d);

void return_home(Drone *d);

void dock_cmd(Drone *d);

void undock_cmd(Drone *d);

void charge_step(Drone *d);

void emergency_stop(Drone *d);

#endif