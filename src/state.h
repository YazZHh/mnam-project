#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "grille.h"

#define MAX_STATES 50000

typedef struct Drone {
    Grille *g;
    int posX, posY;
    int baseX, baseY;
    int battery;
    int obstacle_distance;
    bool docked;
    bool airborne;
    bool crashed;
    bool visZones[ZONE_COUNT];
} Drone;

typedef struct State {
    int posX, posY;
    int baseX, baseY;
    int battery;
    int obstacle_distance;
    bool docked;
    bool airborne;
    bool crashed;
    bool visZones[ZONE_COUNT];
} State;

typedef struct StateList {
    State states[MAX_STATES];
    int nbstates;
} StateList;

// À utuliser ou à virer
typedef struct Transition {
    State start;
    int action;
    State finish;
    float p;
    float reward;
} Transition;

StateList* init_statelist();

State* init_state_drone(Drone *d);

void append_state(StateList *sl, State *s);

bool precondition_verif(State *s, int action);

State* perform_action(Drone *d, int action, int nextX, int nextY);

int get_state_id(StateList *sl, State *s);

float get_reward(State *s_old, State *s_new);

bool is_already_visited(StateList *sl, State *s);

void explore(Grille *g, StateList *sl, State *s, int maxDepth, int maxBreadth, FILE* f);

#endif