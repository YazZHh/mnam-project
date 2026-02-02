#define MAX_X 100
#define MAX_Y 100
#define ZONE_COUNT 4

typedef enum {
    ZONE_1 = 1,
    ZONE_2 = 2,
    ZONE_3 = 3,
    ZONE_4 = 4
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

Grille* init_grille(int x, int y);

void afficher_grille(Grille g);

void afficher_zone(Grille g);

Zone zoneOf(Grille g, int x, int y);

Case* get_danger_case(Grille g);