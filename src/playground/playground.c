/**
 * @file playground.c
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Playground Modul, verwaltet das Spielfeld und initialisiert dieses
 * @version 0.1
 * @date 2020-09-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */


/**
 * @brief Includes
 * 
 */

#include <stdlib.h>
#include <time.h>

#include "playground.h"


/**
 * @brief Typdeklarationen
 * 
 */

typedef struct {    // Koordinaten Variable
    uint8_t entity; // 0: Wasser, alles Andere: Id des Schiffs
    state_t state;
} coordinate_t;


/**
 * @brief Variablendeklarationen
 * 
 */

static coordinate_t playground[GROUND_MAX][SIZE_X][SIZE_Y];

#define ASSERT_GROUND(ground)   assert(ground < GROUND_MAX)
#define ASSERT_XY(x, y)         assert(x < SIZE_X && y < SIZE_Y)
#define ASSERT_DIRECTION(dir)   assert(dir < DIRECTION_MAX)
#define ASSERT_PXPY(x, y)       assert(x && y)
#define ASSERT_STATE(state)     asser(state < STATE_MAX)


/**
 * @brief Private Funktionsprototypen
 * 
 */

/**
 * @brief Setze Spielfeld zurück
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 */
static void clear(ground_t ground);

/**
 * @brief Prüfe ob an gegebener Koordinate mit gegebener Ausrichtung das Schiff platzierbar ist
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x Koordinate X
 * @param y Koordinate Y
 * @param dir Ausrichtung des Schiffs
 * @param length Länge des Schiffs 
 * @return true - platzierbar, false - nicht platzierbar
 */
static bool canPlaceShip(ground_t ground, uint8_t x, uint8_t y, direction_t dir, uint8_t length);

/**
 * @brief Eruiert ob eine Koordinate in der Nähe zu einem anderen Schiff ist (3 x 3 Feld)
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x Koordinate X
 * @param y Koordinate Y
 * @return true - Ja, ist einem Schiff nahe, false - Nein, ist keinem Schiff nahe
 */
static bool nearToShip(ground_t ground, uint8_t x, uint8_t y);

/**
 * @brief Platziere Schiff an angegebener Stelle
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x Koordinate X
 * @param y Koordinate Y
 * @param dir Ausrichtung des Schiffs
 * @param id id des Schiffs
 * @param length Länge des Schiffs
 */
static void placeShip(ground_t ground, uint8_t x, uint8_t y, direction_t dir, uint8_t length, uint8_t id);

/**
 * @brief Gebe Referenz auf Spielfeldkoordinate zurück
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x Koordinate X
 * @param y Koordinate Y
 * @return coordinate_t* die gewünschte Koordinate
 */
static coordinate_t* getCoordinate(ground_t ground, uint8_t x, uint8_t y);


/**
 * @brief Implementation Öffentlicher Funktionen
 * 
 */

uint8_t playgroundGetEntity(ground_t ground, uint8_t x, uint8_t y) {
    return getCoordinate(ground, x, y)->entity;
}

void playgroundSetEntity(ground_t ground, uint8_t x, uint8_t y, uint8_t entity) {
    getCoordinate(ground, x, y)->entity = entity;
}

state_t playgroundGetState(ground_t ground, uint8_t x, uint8_t y) {
    return getCoordinate(ground, x, y)->state;
}

void playgroundSetState(ground_t ground, uint8_t x, uint8_t y, state_t state) {
    getCoordinate(ground, x, y)->state = state;
}

void playgroundGetRandomCoordinate(uint8_t *x, uint8_t *y) {
    ASSERT_PXPY(x, y);
    *x = rand() % SIZE_X;
    *y = rand() % SIZE_Y;
}

direction_t playgroundGetRandomDirection() {
    return rand() % DIRECTION_MAX;
}

bool playgroundTranslateCoordinate(uint8_t *x, uint8_t *y, direction_t dir) {
    ASSERT_PXPY(x, y);
    ASSERT_DIRECTION(dir);
    if ((dir == UP && *y == 0)
     || (dir == RIGHT && *x == (SIZE_X - 1))
     || (dir == DOWN && *y == (SIZE_Y - 1))
     || (dir == LEFT && *x == 0)) {
        return true;
    }
    if (dir == UP) (*y)--;
    else if (dir == RIGHT) (*x)++;
    else if (dir == DOWN) (*y)++;
    else if (dir == LEFT) (*x)--;
    return false;
}

void playgroundInit(ground_t ground) {
    ASSERT_GROUND(ground);
    clear(ground);
    // nur beim ersten Aufruf Zufallsgenerator initialisieren
    static bool randInitialized = false;
    if (!randInitialized) {
        srand(time(NULL));
        randInitialized = true;
    }
    // Iteriere über Schiffskonfiguration und erstelle Schiffe gemäss der gegebenen Länge
    // die ID eines Schiffes ergibt sich aus der Position in SHIP_CONFIG + 1 (da 0 = Wasser)
    uint8_t x, y, dir;
    uint8_t shipConfig[] = {SHIP_CONFIG};
    for (uint8_t id = 0; id < sizeof(shipConfig); id++) {
        uint16_t tries = 0;
        while (true) {
            playgroundGetRandomCoordinate(&x, &y);
            dir = playgroundGetRandomDirection();
            if (canPlaceShip(ground, x, y, dir, shipConfig[id])) {
                placeShip(ground, x, y, dir, shipConfig[id], id+1);
                break;
            }
            // die kleinen U-Boote haben manchmal keinen Platz mehr
            // leere das Spielfeld und beginne von vorne
            if (tries++ > 1000) {
                clear(ground);
                id = 0;
                tries = 0;
            }
        }
    }
}

void playgroundPrint(ground_t ground, uint8_t x, uint8_t y) {
    // generiere Spaltentitel e.g. 1 2 3 4 usw. gem. SIZE_X
    printf("C ");
    for (uint8_t xi = 1; xi <= SIZE_X; xi++) {
        printf("%d", xi);
        if (xi < 10) printf(" ");
    }
    printf("\t\t\tS ");
    for (uint8_t xi = 1; xi <= SIZE_X; xi++) {
        printf("%d", xi);
        if (xi < 10) printf(" ");
    }
    printf("\n");
    // generiere Zeile um Zeile
    for (uint8_t yi = 0; yi < SIZE_Y; yi++) {
        // Computer
        printf("%c ", yi + 'A'); // generiere Zeilentitel e.g. A B C usw. gem. SIZE_Y
        for (uint8_t xi = 0; xi < SIZE_X; xi++) {
            coordinate_t *coordinate = getCoordinate(COMPUTER, xi, yi);
            char entity;
            uint8_t color = 37; // Standartmässig Weiss
            if (coordinate->state == HIDDEN) {
                entity = '.';
            } else {
                entity = coordinate->entity ? '#' : '~';
                color = coordinate->entity ? 33 : 34; // # - gelb, ~ - blau
            }
            if (ground == COMPUTER && x == xi && y == yi) { // aktueller Treffer
                color = 31; // rot
            }
            printf("\033[0;%dm%c\033[0m ", color, entity);
        }
        printf("\t\t\t");
        // Spieler
        printf("%c ", yi + 'A'); // generiere Zeilentitel
        for (uint8_t xi = 0; xi < SIZE_X; xi++) {
            coordinate_t *coordinate = getCoordinate(PLAYER, xi, yi);
            char entity = coordinate->entity ? '#' : '~';
            uint8_t color = 37; // Standartmässig Weiss
            if (coordinate->state == FOUND) { // wenn getroffen, dann färbe ein
                color = coordinate->entity ? 33 : 34; // # - gelb, ~ - blau
            }
            if (ground == PLAYER && x == xi && y == yi) { // aktueller Treffer
                color = 31; // rot
            }
            printf("\033[0;%dm%c\033[0m ", color, entity);
        }
        printf("\n");
    }
    printf("\n");
}


/**
 * @brief Implementation Privater Funktionen
 * 
 */

static void clear(ground_t ground) {
    ASSERT_GROUND(ground);
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        for (uint8_t x = 0; x < SIZE_X; x++) {
            playgroundSetEntity(ground, x, y, 0);
            playgroundSetState(ground, x, y, HIDDEN);
        }
    }
}

static bool canPlaceShip(ground_t ground, uint8_t x, uint8_t y, direction_t dir, uint8_t length) {
    // Platzbedarf prüfen
    if ((dir == UP && length > (y + 1))
     || (dir == RIGHT && length > (SIZE_X - x))
     || (dir == DOWN && length > (SIZE_Y - y))
     || (dir == LEFT && length > (x + 1))) {
        return false;
    }
    // Nähe zu anderen Schiffen prüfen
    for (uint8_t i = 0; i < length; i++) {
        if (nearToShip(ground, x, y)
         || playgroundTranslateCoordinate(&x, &y, dir)) return false;
    }
    return true;
}

static bool nearToShip(ground_t ground, uint8_t x, uint8_t y) {
    // an Koordinate
    if (playgroundGetEntity(ground, x, y)) return true;
    // oberhalb
    if (y > 0) {
        if (((x > 0) && playgroundGetEntity(ground, x-1, y-1))
         || playgroundGetEntity(ground, x, y-1)
         || ((x < (SIZE_X-1)) && playgroundGetEntity(ground, x+1, y-1))) {
            return true;
        }
    }
    // links & rechts
    if (((x > 0) && playgroundGetEntity(ground, x-1, y))
     ||((x < (SIZE_X-1)) && playgroundGetEntity(ground, x+1, y))) {
        return true;
    }
    // unterhalb
    if (y < (SIZE_Y-1)) {
        if (((x > 0) && playgroundGetEntity(ground, x-1, y+1))
         || playgroundGetEntity(ground, x, y+1)
         || ((x < (SIZE_X-1)) && playgroundGetEntity(ground, x+1, y+1))) {
            return true;
        }
    }
    return false;
}

static void placeShip(ground_t ground, uint8_t x, uint8_t y, direction_t dir, uint8_t length, uint8_t id) {
    for (uint8_t i = 0; i < length; i++) {
        playgroundSetEntity(ground, x, y, id);
        playgroundTranslateCoordinate(&x, &y, dir);
    }
}

static coordinate_t* getCoordinate(ground_t ground, uint8_t x, uint8_t y) {
    ASSERT_GROUND(ground);
    ASSERT_XY(x, y);
    return &playground[ground][x][y];
}
