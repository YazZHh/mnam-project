#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "grille.h"

#define BATTERY_FULL 100

typedef struct Drone {
    Grille *g;
    int posX, posY;
    int baseX, baseY;
    int battery;
    int obstacle_distance;
    bool docked;
    bool airborne;
    bool crashed;
    bool visZones[ZONE_COUNT];
} Drone;

Drone* init_drone(Grille *g);

void afficher_grille_drone(Drone *d);

void takeoff_cmd(Drone *d);

void move_step(Drone *d);

void avoid_maneuver(Drone *d);

void return_home(Drone *d);

void dock_cmd(Drone *d);

void undock_cmd(Drone *d);

void charge_step(Drone *d);

void emergency_stop(Drone *d);