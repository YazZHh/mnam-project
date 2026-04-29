#include <stdio.h>
#include <stdlib.h>
#include "grille.h"

Case *init_case(int x, int y, Zone zone, char state){
    Case *c = malloc(sizeof(struct Case));
    c->x = x;
    c->y = y;
    c->zone = zone;
    c->state = state;
    return c;
}

Grille *init_grille(int x, int y, int obstacle_probability){
    if (x<1 || y<1 || x>MAX_X || y>MAX_Y)
        return NULL;
    Grille *g = malloc(sizeof(struct Grille));
    if (!g)
        return NULL;
    g->x = x;
    g->y = y;

    g->tab = malloc(sizeof(struct Case*)*y);
    for (int i=0; i<y; i++) {
        g->tab[i] = malloc(sizeof(struct Case)*x);
    }
    for (int i=0; i<y; i++){
        for (int j=0; j<x; j++){

            g->tab[i][j].x = j;
            g->tab[i][j].y = i;
            if (j<=((x-1)/2) && i<=((y-1)/2))
                g->tab[i][j].zone = ZONE_1;
            else if (j<=((x-1)/2) && i>((y-1)/2))
                g->tab[i][j].zone = ZONE_2;
            else if (j>((x-1)/2) && i<=((y-1)/2))
                g->tab[i][j].zone = ZONE_3;
            else
                g->tab[i][j].zone = ZONE_4;
            int obstacle = rand()%100 + 1;
            if (obstacle<obstacle_probability && i != 0 && j != 0)
                g->tab[i][j].state = CASE_DANGER;
            else
                g->tab[i][j].state = CASE_VIDE;
        }
    }
    return g;
}

Grille* init_grille_file(FILE* f){
    int tailleX, tailleY;
    fscanf(f, " %d, %d", &tailleX, &tailleY);
    Grille *g = malloc(sizeof(struct Grille));
    if (!g)
        return NULL;
    g->x = tailleX;
    g->y = tailleY;

    g->tab = malloc(sizeof(struct Case*)*tailleY);
    for (int i=0; i<tailleY; i++) {
        g->tab[i] = malloc(sizeof(struct Case)*tailleX);
    }

    fscanf(f, "\n");
    char case_content;
    for (int i=0; i<tailleY; i++){
        for (int j=0; j<tailleX; j++){
            g->tab[i][j].x = j;
            g->tab[i][j].y = i;
            if (j<=((tailleX-1)/2) && i<=((tailleY-1)/2))
                g->tab[i][j].zone = ZONE_1;
            else if (j<=((tailleX-1)/2) && i>((tailleY-1)/2))
                g->tab[i][j].zone = ZONE_2;
            else if (j>((tailleX-1)/2) && i<=((tailleY-1)/2))
                g->tab[i][j].zone = ZONE_3;
            else
                g->tab[i][j].zone = ZONE_4;

            fscanf(f, "%c", &case_content);
            switch (case_content){
                case '#': g->tab[i][j].state = CASE_DANGER; break;
                default: g->tab[i][j].state = CASE_VIDE; break;
            }
        }
        fscanf(f, "\n");
    }
    return g;
}

void afficher_grille(Grille *g){
    for (int i=0; i<g->y; i++){
        for (int j=0; j<g->x; j++){
            printf("%c", g->tab[i][j].state);
        }
        printf("\n");
    }
}

void afficher_zone(Grille *g){
    for (int i=0; i<g->y; i++){
        for (int j=0; j<g->x; j++){
            switch (g->tab[i][j].zone){
                case (ZONE_1):
                    printf("1");
                    break;
                case (ZONE_2):
                    printf("2");
                    break;
                case (ZONE_3):
                    printf("3");
                    break;
                case (ZONE_4):
                    printf("4");
                    break;
                default:
                    break;
            }
        }
        printf("\n");
    }
}

Zone zoneOf(Grille *g, int x, int y){
    return g->tab[y][x].zone;
}

char stateOf(Grille *g, int x, int y){
    return g->tab[y][x].state;
}

int get_danger_case(Grille *g, Case *Danger){
    int ind_danger = 0;
    for (int i=0; i<g->y; i++){
        for (int j=0; j<g->x; j++){
            if (g->tab[i][j].state != CASE_VIDE){
                Danger[ind_danger] = g->tab[i][j];
                ind_danger++;
            }
        }
    }
    return ind_danger;
}