#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "grille.h"

typedef struct Drone {
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

void takeoff_cmd(Grille *g, Drone *d);

void move_step(Grille *g, Drone *d);

void avoid_maneuver(Grille *g, Drone *d);