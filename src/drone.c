#include <stdlib.h>
#include "drone.h"

Drone *init_drone(Grille *g){
    Drone* d = malloc(sizeof(struct Drone));
    d->g = g;
    d->posX = 0;
    d->posY = 0;
    d->baseX = 0;
    d->baseY = 0;
    d->battery = BATTERY_FULL;
    d->obstacle_distance = 2;
    d->docked = false;
    d->airborne = false;
    d->crashed = false;
    d->visZones[zoneOf(g, 0, 0)] = true;
    return d;
}

void takeoff_cmd(Drone *d){
    if (!(d->crashed) && !(d->airborne) && !(d->docked) && d->battery > 0){
        d->airborne = 1;
        if (stateOf(d->g, d->posX, d->posY) == CASE_DANGER)
            d->obstacle_distance = 0;
        else
            d->obstacle_distance = 2;
    }
}

void move_step(Drone *d){
    if (!d->crashed && d->airborne && !d->docked && d->battery > 0){
        int nextX = d->posX;
        int nextY = d->posY;
        
        if (rand()%2 == 0){
            if (d->posX == 0)
                nextX = d->posX+1;
            else if (d->posX == MAX_X)
                nextX = d->posX-1;
            else if (rand()%2 == 0)
                nextX = d->posX+1;
            else 
                nextX = d->posX-1;
        } else {
            if (d->posY == 0)
                nextY = d->posY+1;
            else if (d->posY == MAX_Y)
                nextY = d->posY-1;
            else if (rand()%2 == 0)
                nextY = d->posY+1;
            else 
                nextY = d->posY-1;
        }

        d->posX = nextX;
        d->posY = nextY;
        d->battery--;
        d->visZones[zoneOf(d->g, nextX, nextY)] = true;
        if (stateOf(d->g, nextX, nextY) == CASE_DANGER)
            d->obstacle_distance = 0;
        else
            d->obstacle_distance = 2;
    }
}

void avoid_maneuver(Drone *d){
    if (!d->crashed && d->airborne && !d->docked && d->battery > 1){
        Case *cases[4];
        Case *C;
        int indCases = 0;
        for (int i=0; i<4; i++){
            switch(i){
                case 0:
                    if (d->posX>0)
                        C = &(d->g->tab[d->posY][d->posX-1]);
                    else
                        C = NULL;
                    break;
                case 1:
                    if (d->posY<MAX_Y)
                        C = &(d->g->tab[d->posY+1][d->posX]);
                    else
                        C = NULL;
                    break;
                case 2:
                    if (d->posX<MAX_X)
                        C = &(d->g->tab[d->posY][d->posX+1]);
                    else
                        C = NULL;
                    break;
                default:
                    if (d->posY>0)
                        C = &(d->g->tab[d->posY-1][d->posX]);
                    else
                        C = NULL;
                    break;
            }
            if (C != NULL && C->state == CASE_VIDE){
                cases[indCases] = C;
                indCases++;
            }
        }

        if (indCases > 0){
            C = cases[rand()%indCases];
            d->posX = C->x;
            d->posY = C->y;
            d->battery =- 2;
            d->visZones[(zoneOf(d->g, d->posX, d->posY))] = true;
            d->obstacle_distance = 2;
        } else {
            move_step(d);
        }
    }
}

void return_home(Drone *d){
    if (!d->crashed && d->airborne && !d->docked && d->battery > 0){
        int nextX = d->posX;
        int nextY = d->posY;

        if (d->posX < d->baseX)
            nextX = d->posX+1;
        else if (d->posX > d->baseX)
            nextX = d->posX-1;
        if (d->posY < d->baseY)
            nextX = d->posY+1;
        else if (d->posY > d->baseY)
            nextX = d->posY-1;

        d->posX = nextX;
        d->posY = nextY;
        d->battery--;
        d->visZones[zoneOf(d->g, nextX, nextY)] = true;

        if (stateOf(d->g, nextX, nextY) == CASE_DANGER)
            d->obstacle_distance = 0;
        else
            d->obstacle_distance = 2;
    }
}

void dock_cmd(Drone *d){
    if (!d->crashed && d->airborne && !d->docked && d->posX == d->baseX && d->posY == d->baseY){
        d->airborne = false;
        d->docked = true;
    }
}

void undock_cmd(Drone *d){
    if (!d->crashed && !d->airborne && d->docked && d->battery > 0){
        d->docked = false;
        d->airborne = true;
        d->visZones[zoneOf(d->g, d->posX, d->posY)] = true;
    }
}

void charge_step(Drone *d){
    if (!d->crashed && d->docked){
        if (d->battery <= 95)
            d->battery += 5;
        else
            d->battery = 100;
    }
}

void emergency_stop(Drone *d){
    d->crashed = true;
    d->airborne = false;
    d->docked = false;
    d->obstacle_distance = 0;
}
