#include "state.h"
#include "drone.h"

StateList* init_statelist(){
    StateList* sl = malloc(sizeof(StateList));
    sl->nbstates = 0;
    return sl;
}

State *init_state_drone(Drone *d){
    State *s = malloc(sizeof(struct State));
    s->posX = d->posX;
    s->posY = d->posY;
    s->baseX = d->baseX;
    s->baseY = d->baseY;
    s->battery = d->battery;
    s->obstacle_distance = d->obstacle_distance;
    s->docked = d->docked;
    s->airborne = d->airborne;
    s->crashed = d->crashed;
    for (int i=ZONE_1; i<=ZONE_COUNT; i++)
        s->visZones[i] = d->visZones[i];
    return s;
}

void append_state(StateList *sl, State *s){
    sl->states[sl->nbstates] = *s;
    sl->nbstates++;
}

bool precondition_verif(State *s, int action){
    switch (action){
        case 1: return (!s->crashed && !s->airborne && !s->docked && s->battery > 0);
        case 2: return (!s->crashed && s->airborne && !s->docked && s->battery > 0);
        case 3: return (!s->crashed && s->airborne && !s->docked && s->battery > 1);
        case 4: return (!s->crashed && s->airborne && !s->docked && s->battery > 0);
        case 5: return (!s->crashed && s->airborne && !s->docked && s->posX == s->baseX && s->posY == s->baseY);
        case 6: return (!s->crashed && !s->airborne && s->docked && s->battery > 0);
        case 7: return (!s->crashed && s->docked);
        case 8: return true;
        default: return false;
    }
}

State* perform_action(Drone *d, int action, int nextX, int nextY){
    switch (action){
        case 1: takeoff_cmd(d); break;
        case 2: move_step(d, nextX, nextY); break;
        case 3: avoid_maneuver(d); break;
        case 4: return_home(d); break;
        case 5: dock_cmd(d); break;
        case 6: undock_cmd(d); break;
        case 7: charge_step(d); break;
        case 8: emergency_stop(d); break;
        default: break;
    }
    return init_state_drone(d);
}

int get_state_id(StateList *sl, State *s){
    for (int i=0; i<sl->nbstates; i++) {
        State *current = &(sl->states[i]);
        if (current->posX == s->posX &&
            current->posY == s->posY &&
            current->battery == s->battery &&
            current->docked == s->docked &&
            current->airborne == s->airborne &&
            current->crashed == s->crashed) {
            
            bool same_zones = true;
            int j=0;
            while (j<ZONE_COUNT && same_zones == true){
                if (current->visZones[j] != s->visZones[j])
                    same_zones = false;
                j++;
            }
            if (same_zones)
                return i;
        }
    }
    return -1;
}

float get_reward(State *s_old, State *s_new){
    if (s_new->crashed)
        return -100.0f;

    int zones_count = 0;
    for (int i=0; i<ZONE_COUNT; i++) {
        if (s_new->visZones[i])
            zones_count++;
    }
    
    if (s_new->docked && zones_count == ZONE_COUNT) {
        return 500.0f;
    }

    int old_zones = 0;
    for (int i=0; i<ZONE_COUNT; i++) {
        if (s_old->visZones[i])
            old_zones++;
    }

    if (zones_count > old_zones)
        return 50.0f;
    return -1.0f;
}

bool is_already_visited(StateList *sl, State *s) {
    return (get_state_id(sl, s) != -1);
}

void explore(Grille *g, StateList *sl, State *s, int maxDepth, int maxBreadth, FILE* f){
    if (is_already_visited(sl, s))
        return;
    
    append_state(sl, s);
    int id_s = sl->nbstates-1;
    if (id_s > MAX_STATES)
        return;

    if (maxDepth != -1 && maxDepth <= 0)
        return;

    int count = 0;
    for (int action=1; action<=8; action++){
        count++;

        if (maxBreadth != -1 && count >= maxBreadth)
            break;

        if (precondition_verif(s, action)){
            
            int d_prime;
            if (maxDepth == -1)
                d_prime = -1;
            else
                d_prime = maxDepth -1;

            if (action == 2){
                int nbCases;
                Drone *d_helper = init_drone_state(g, s);
                Case** cases = moves_possibles(d_helper, &nbCases);
                float p = 1.0 / nbCases;
                free(d_helper);

                State *s_prime;

                for (int i=0; i<nbCases; i++){
                    Drone *d_exec = init_drone_state(g, s);
                    s_prime = perform_action(d_exec, action, cases[i]->x, cases[i]->y);
                    
                    float reward = get_reward(s, s_prime);

                    explore(g, sl, s_prime, d_prime, maxBreadth, f);
                    int id_s_prime = get_state_id(sl, s_prime);
                    
                    fprintf(f, "%d;\t%d;\t%d;\t%d;\t%d;\t%f;\t%f\n", id_s, action, cases[i]->x, cases[i]->y, id_s_prime, reward, p);

                    free(s_prime);
                    free(d_exec);
                }
                free(cases);
            } else {
                Drone *d_exec = init_drone_state(g, s);
                State *s_prime = perform_action(d_exec, action, 0, 0);
                
                float reward = get_reward(s, s_prime);
                float p = 1.0;

                explore(g, sl, s_prime, d_prime, maxBreadth, f);
                int id_s_prime = get_state_id(sl, s_prime);

                fprintf(f, "%d;\t%d;\tNULL;\tNULL;\t%d;\t%f;\t%f\n", id_s, action, id_s_prime, reward, p);

                free(s_prime);
                free(d_exec);
            }
        }
    }
}