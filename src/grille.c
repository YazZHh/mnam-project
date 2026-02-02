#include <stdio.h>
#include <stdlib.h>
#include "grille.h"

Grille* init_grille(int x, int y){
    if (x<1 || y<1 || x>MAX_X || y>MAX_Y)
        return NULL;
    Grille *g = malloc(sizeof(struct Grille));
    if (!g)
        return NULL;
    g->x = x;
    g->y = y;

    g->tab = malloc(sizeof(Case*)*y);
    for (int i = 0; i < y; i++) {
        g->tab[i] = malloc(sizeof(Case)*x);
    }
    for (int i=0; i<y; i++){
        for (int j=0; j<x; j++){
            g->tab[i][j].x = j;
            g->tab[i][j].y = i;
            g->tab[i][j].zone = ZONE_1;
            g->tab[i][j].state = '.';
        }
    }
    return g;
}

void afficher_grille(Grille g){
    for (int i=0; i<g.y; i++){
        // for (int k=0; k<g.x*2; k++)
        //     printf("_");
        // printf("\n|");
        for (int j=0; j<g.x; j++){
            printf("%c", g.tab[i][j].state);
        }
        printf("\n");
    }
    // for (int k=0; k<g.x*2; k++)
    //     printf("_");
    // printf("\n");
}