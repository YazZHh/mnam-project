#include <stdlib.h>
#include "drone.h"

Drone *init_drone(Grille *g){
    Drone* d = malloc(sizeof(struct Drone));
    d->posX = 0;
    d->posY = 0;
    d->baseX = 0;
    d->baseY = 0;
    d->battery = 100;
    d->obstacle_distance = 2;
    d->docked = false;
    d->airborne = false;
    d->crashed = false;
    d->visZones[zoneOf(g, 0, 0)] = true;
    return d;
}

void takeoff_cmd(Grille *g, Drone *d){
    if (!(d->crashed) && !(d->airborne) && !(d->docked) && d->battery > 0){
        d->airborne = 1;

        if (g->tab[d->posY][d->posX].state == CASE_DANGER)
            d->obstacle_distance = 0;
        else
            d->obstacle_distance = 2;
    }
}

void move_step(Grille *g, Drone *d){
    if (!d->crashed && d->airborne && !d->docked && d->battery>0){
        int nextX, nextY;
        
        if (rand()%2 == 0){
            if (d->posX == 0)
                nextX = d->posX+1;
            else if (d->posX == MAX_X)
                nextX = d->posX-1;
            else if (rand()%2 == 0)
                nextX = d->posX+1;
            else 
                nextX = d->posX-1;
            d->posX = nextX;
        } else {
            if (d->posY == 0)
                nextY = d->posY+1;
            else if (d->posY == MAX_Y)
                nextY = d->posY-1;
            else if (rand()%2 == 0)
                nextY = d->posY+1;
            else 
                nextY = d->posY-1;
            d->posY = nextY;
        }

        d->battery--;
        d->visZones[zoneOf(g, nextX, nextY)] = true;

        if (g->tab[nextY][nextX].state == CASE_DANGER)
            d->obstacle_distance = 0;
        else
            d->obstacle_distance = 2;
    }
}
