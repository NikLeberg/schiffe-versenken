/**
 * @file logic.c
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Logik Modul, Funktionen der Spiellogik
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

#include "logic.h"


/**
 * @brief Typdeklarationen
 * 
 */


/**
 * @brief Variablendeklarationen
 * 
 */


/**
 * @brief Private Funktionsprototypen
 * 
 */

/**
 * @brief Eruiert ob ein Treffer der letzte auf ein Schiff war
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param id ID des Schiffs
 * @return true - war der letzte Treffer, false - war nicht der letzte Treffer
 */
static bool logicWasLastHit(ground_t ground, uint8_t id);

/**
 * @brief Decke benachbartes Wasser um ein Schiff herum auf
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param id ID des Schiffs
 */
static void logicUnhideSurrounding(ground_t ground, uint8_t id);


/**
 * @brief Implementation Öffentlicher Funktionen
 * 
 */

bool logicFinished(ground_t ground) {
    // für jede Koordinate
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        for (uint8_t x = 0; x < SIZE_X; x++) {
            // prüfe ob dort ein Schiff ist, welches noch nicht gefunden wurde
            if (playgroundGetEntity(ground, x, y)
             && playgroundGetState(ground, x, y) == HIDDEN) {
                return false;
            }
        }
    }
    return true;
}

uint8_t logicComputeHit(ground_t ground, uint8_t x, uint8_t y) {
    // Melden was getroffen wurde
    playgroundSetState(ground, x, y, FOUND);
    uint8_t entity = playgroundGetEntity(ground, x, y);
    bool last;
    if (entity) {
        last = logicWasLastHit(ground, entity);
    } else {
        last = false;
    }
    if (last) logicUnhideSurrounding(ground, entity);
    printf("%s schiesst auf %s: %c%d -> \033[0;%s\033[0m%s\n\n",
        ground ? "Computer" : "Spieler",
        ground ? "Spieler" : "Computer",
        y + 'A', x + 1,
        entity ? "33mTreffer!" : "34mFehlschuss.",
        last ? " - \033[0;31mversenkt.\033[0m" : "");
    if (!entity) assert(!last); // nie Wasser und letzter Schuss
    if (!entity) return 0; // nichts getroffen
    else if (!last) return 1; // getroffen aber nicht versenkt
    else return 2; // getroffen und versenkt
}


/**
 * @brief Implementation Privater Funktionen
 * 
 */

static bool logicWasLastHit(ground_t ground, uint8_t id) {
    // iteriere über alle Koordinaten
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        for (uint8_t x = 0; x < SIZE_X; x++) {
            // suche nach anderen noch nicht gefundenen Schiffsteilen
            if (playgroundGetEntity(ground, x, y) == id
             && playgroundGetState(ground, x, y) == HIDDEN) {
                return false; // Teil gefunden, war nicht letzter Treffer
            }
        }
    }
    return true; // war letzter Treffer
}

static void logicUnhideSurrounding(ground_t ground, uint8_t id) {
    // für jede Koordinate
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        for (uint8_t x = 0; x < SIZE_X; x++) {
            if (playgroundGetEntity(ground, x, y) == id) {
                // oberhalb
                if (y > 0) {
                    if (x > 0) playgroundSetState(ground, x-1, y-1, FOUND);
                    playgroundSetState(ground, x, y-1, FOUND);
                    if (x < (SIZE_X-1)) playgroundSetState(ground, x+1, y-1, FOUND);
                }
                // links & rechts
                if (x > 0) playgroundSetState(ground, x-1, y, FOUND);
                if (x < (SIZE_X-1)) playgroundSetState(ground, x+1, y, FOUND);
                // unterhalb
                if (y < (SIZE_Y-1)) {
                    if (x > 0) playgroundSetState(ground, x-1, y+1, FOUND);
                    playgroundSetState(ground, x, y+1, FOUND);
                    if (x < (SIZE_X-1)) playgroundSetState(ground, x+1, y+1, FOUND);
                }
            }
        }
    }
}
