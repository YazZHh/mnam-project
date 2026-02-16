#include <stdio.h>
#include <stdlib.h>
#include "drone.h"

int main(int argc, char *argv[]){
    srand(time(NULL));
    Grille *g = init_grille(16, 6);
    afficher_grille(g);
    afficher_zone(g);
    Drone *d = init_drone(g);
    takeoff_cmd(d);
}