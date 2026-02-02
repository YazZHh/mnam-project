#include <stdio.h>
#include "grille.h"

int main(int argc, char *argv[]){
    Grille *g = init_grille(6, 6);
    afficher_grille(*g);
}