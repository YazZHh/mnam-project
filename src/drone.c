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

void takeoff_cmd(Drone *d, Case Danger[], int Danger_len){
    if (!(d->crashed) && !(d->airborne) && !(d->docked) && d->battery > 0){
        d->airborne = 1;
        int i=0;
        Case c;
        if(Danger_len>0){
            c = Danger[i];
            while (i<Danger_len && c.x != d->posX && c.y != d->posY){
                c = Danger[i];
                i++;
            }
            if (i<Danger_len)
                d->obstacle_distance = 2;
        }
        d->obstacle_distance = 0;
    }
}

