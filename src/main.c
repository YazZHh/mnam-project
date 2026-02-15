#include <stdio.h>
#include <stdlib.h>
#include "drone.h"

int main(int argc, char *argv[]){
    srand(time(NULL));
    Grille *g = init_grille(16, 6);
    afficher_grille(g);
    afficher_zone(g);
    Case *Danger = malloc(sizeof(struct Case)*(g->x)*(g->y));
    int Danger_len = get_danger_case(g, Danger);
    Drone *d = init_drone(g);
    takeoff_cmd(g, d);
}