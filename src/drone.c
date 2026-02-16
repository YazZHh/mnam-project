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

void avoid_maneuver(Grille *g, Drone *d){
    if (!d->crashed && d->airborne && !d->docked && d->battery>1){
        Case *cases[4];
        Case *C;
        // int nextX, nextY;
        int indCases = 0;
        for (int i=0; i<4; i++){
            switch(i){
                case 0:
                    if (d->posX>0)
                        C = &(g->tab[d->posY][d->posX-1]);
                    else
                        C = NULL;
                    break;
                case 1:
                    if (d->posY<MAX_Y)
                        C = &(g->tab[d->posY+1][d->posX]);
                    else
                        C = NULL;
                    break;
                case 2:
                    if (d->posX<MAX_X)
                        C = &(g->tab[d->posY][d->posX+1]);
                    else
                        C = NULL;
                    break;
                default:
                    if (d->posY>0)
                        C = &(g->tab[d->posY-1][d->posX]);
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
            d->battery = d->battery-2;
            d->visZones[(zoneOf(g, d->posX, d->posY))] = true;
            d->obstacle_distance = 2;
        } else {
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
            d->posX = nextX;
            d->posY = nextY;
            d->battery = d->battery-2;
            d->visZones[(zoneOf(g, d->posX, d->posY))] = true;

            if (g->tab[d->posY][d->posX].state == CASE_DANGER)
                d->obstacle_distance = 0;
            else
                d->obstacle_distance = 2;
        }
    }
}
