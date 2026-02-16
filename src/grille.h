#define MAX_X 100
#define MAX_Y 100
#define ZONE_COUNT 4

#define CASE_VIDE '.'
#define CASE_DANGER '#'

typedef enum {
    ZONE_1 = 0,
    ZONE_2 = 1,
    ZONE_3 = 2,
    ZONE_4 = 3
} Zone;

typedef struct Case {
    int x, y;
    Zone zone;
    char state;
} Case;

typedef struct Grille {
    int x, y;
    Case **tab;
} Grille;

Case* init_case(int x, int y, Zone zone, char state);

Grille* init_grille(int x, int y);

void afficher_grille(Grille *g);

void afficher_zone(Grille *g);

Zone zoneOf(Grille *g, int x, int y);

char stateOf(Grille *g, int x, int y);

int get_danger_case(Grille *g, Case *Danger);