#include <stdio.h>
#include <stdbool.h>
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

void takeoff_cmd(Drone *d, Case Danger[], int Danger_len);