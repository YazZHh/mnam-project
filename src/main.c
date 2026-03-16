#include <stdio.h>
#include <stdlib.h>
#include "drone.h"

int main(int argc, char *argv[]){
    int mode;
    printf("Choisissez un mode d'utilisation du drone (1: manuel, 2: automatique, 3: exhaustif) : ");
    scanf("%d", &mode);
    while (mode != 1 && mode != 2 && mode != 3){
        printf("Entrée incorrecte ! Choisissez un mode d'utilisation du drone (1: manuel, 2: automatique, 3: exhaustif) : ");
        scanf("%d", &mode);
    }

    int tailleX, tailleY;
    printf("Choisissez une taille de grille (2 min, 30 max)\n");
    printf("abscisse : ");
    scanf("%d", &tailleX);
    while (tailleX < 2 || tailleX > 30){
        printf("Entrée incorrecte ! (2 min, 30 max)\n");
        printf("abscisse : ");
        scanf("%d", &tailleX);
    }
    printf("ordonnée : ");
    scanf("%d", &tailleY);
    while (tailleY < 2 || tailleY > 30){
        printf("Entrée incorrecte ! (2 min, 30 max)\n");
        printf("ordonnée : ");
        scanf("%d", &tailleY);
    }

    srand(time(NULL));
    Grille *g = init_grille(tailleX, tailleY);
    Drone *d = init_drone(g);
    
    if (mode == 1){
        int action;
        bool have_actioned;
        while (!d->crashed){
            printf("\n");
            afficher_zone(g);
            afficher_grille_drone(d);
            printf("État du drone : (posX,poxY)=(%d,%d), battery=%d, docked=%d, airbone=%d, crashed=%d\n", d->posX, d->posY, d->battery, d->docked, d->airborne, d->crashed);
            printf("Zones visitées : ");
            for (int i=0; i<4; i++){
                if (d->visZones[i])
                    printf("%d ", i+1);
            }
            printf("\n");

            have_actioned = false;
            while (!have_actioned){
                printf("Choisissez un action à faire (1: takeoff, 2: move, 3: avoid, 4: return home, 5: dock, 6: undock, 7: charge step, 8: emergency stop) : ");
                scanf("%d", &action);
                while (action < 1 || action > 8){
                    printf("Entrée incorrecte ! Choisissez un action à faire (1: takeoff, 2: move, 3: avoid, 4: return home, 5: dock, 6: undock, 7: charge step, 8: emergency stop)");
                    scanf("%d", &action);
                }
                switch (action){
                    case 1:
                        if (!d->crashed && !d->airborne && !d->docked && d->battery > 0){
                            takeoff_cmd(d);
                            have_actioned = true;
                        } else
                            printf("Action impossible dans l'état actuel du drone !\n");
                        break;
                    case 2:
                        if (!d->crashed && d->airborne && !d->docked && d->battery > 0){
                            int indCases;
                            Case** cases = moves_possibles(d, &indCases);
                            
                            printf("Choisissez une case parmis les suivantes (");
                            int i, choix;
                            for (i=0; i<indCases-1; i++){
                                printf("%d: (%d,%d), ", i+1, cases[i]->x, cases[i]->y);
                            }
                            printf("%d: (%d,%d)) : ", i+1, cases[i]->x, cases[i]->y);
                            scanf("%d", &choix);
                            while (choix < 1 || choix > i+1){
                                printf("Entrée incorrecte ! Choisissez une case : ");
                                scanf("%d", &choix);
                            }
                            int nextX = cases[choix-1]->x;
                            int nextY = cases[choix-1]->y;

                            move_step(d, nextX, nextY);
                            have_actioned = true;
                        } else
                            printf("Action impossible dans l'état actuel du drone !\n");
                        break;
                    case 3:
                        if (!d->crashed && d->airborne && !d->docked && d->battery > 1){
                            avoid_maneuver(d);
                            have_actioned = true;
                        } else
                            printf("Action impossible dans l'état actuel du drone !\n");
                        break;
                    case 4:
                        if (!d->crashed && d->airborne && !d->docked && d->battery > 0){
                            return_home(d);
                            have_actioned = true;
                        } else
                            printf("Action impossible dans l'état actuel du drone !\n");
                        break;
                    case 5:
                        if (!d->crashed && d->airborne && !d->docked && d->posX == d->baseX && d->posY == d->baseY){
                            dock_cmd(d);
                            have_actioned = true;
                        } else
                            printf("Action impossible dans l'état actuel du drone !\n");
                        break;
                    case 6:
                        if (!d->crashed && !d->airborne && d->docked && d->battery > 0){
                            undock_cmd(d);
                            have_actioned = true;
                        } else
                            printf("Action impossible dans l'état actuel du drone !\n");
                        break;
                    case 7:
                        if (!d->crashed && d->docked){
                            charge_step(d);
                            have_actioned = true;
                        } else
                            printf("Action impossible dans l'état actuel du drone !\n");
                        break;
                    case 8:
                        emergency_stop(d);
                        have_actioned = true;
                        break;
                    default:
                        break;
                }
            }
        }
    } else if (mode == 2){

    }
    
    afficher_grille(g);
    afficher_zone(g);
    
    takeoff_cmd(d);
    // move_step(d);
    
    printf("\n");
    afficher_grille_drone(d);
}