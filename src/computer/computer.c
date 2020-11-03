/**
 * @file computer.c
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Computer Modul, realisiert die Logik des Computerspielers
 * @version 0.1
 * @date 2020-09-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */


/**
 * @brief Includes
 * 
 */

#include "../playground/playground.h"
#include "computer.h"


/**
 * @brief Typdeklarationen
 * 
 */

typedef struct {
    uint8_t step;
    uint8_t startX, startY;
    direction_t dir;
    bool foundOnce;
    uint8_t triedDirections;
} stateMachine_t;


/**
 * @brief Variablendeklarationen
 * 
 */

static stateMachine_t stateMachine[GROUND_MAX];


/**
 * @brief Private Funktionsprototypen
 * 
 */

/**
 * @brief Erhalte eine pseudo zufällige Koordinate welche noch nicht getroffen wurde und
 *        ähnlich zu einem Schachbrett nur auf Schwartz oder Weiss ist. (per Spiel wird Farbe gewählt)
 * 
 * @param x[out] Pointer auf X Koordinate
 * @param y[out] Pointer auf Y Koordinate
 */
static void pseudoRandomXY(ground_t ground, uint8_t *x, uint8_t *y);

/**
 * @brief Erhalte eine zufällige Richtung welche noch nicht versucht wurde
 * 
 * @param x[in,out] // toDo
 * @param y[in,out] // toDo
 * @param firstGuess muss true sein wenn die Ausrichtung von einem neuen Schiff gesucht wird
 * @return direction_t eine mögliche Ausrichtung
 */
static direction_t guessDir(ground_t player, uint8_t x, uint8_t y, bool firstGuess);


/**
 * @brief Implementation Öffentlicher Funktionen
 * 
 */

void computer(ground_t player, uint8_t *x, uint8_t *y, bool lastWasHit, bool lastHitWasLast) {
    stateMachine_t *s = &stateMachine[player];
    if (lastWasHit) ++s->step; // Schrittkette bei Treffer vorwärtsschieben
    if (lastHitWasLast) s->step = 0; // Schrittkette bei komplett-Abschuss neustarten
    switch (s->step) {
        case (0): // zufällige Koordinate auswählen
            pseudoRandomXY(!player, x, y);
            break;
        case (1): // erster Treffer
            s->startX = *x; // Koordinaten des ersten Treffers speichern
            s->startY = *y;
            s->foundOnce = false;
            s->dir = guessDir(player, *x, *y, true); // schätzen und anhand Koordinatenlage gewisse Richtungen ausschliessen
            playgroundTranslateCoordinate(x, y, s->dir);
            s->step = 2;
            break;
        case (2): // Richtung noch nicht gefunden
            *x = s->startX;
            *y = s->startY;
            if (!s->foundOnce) { // wenn zuvor nicht im Schritt 3 gewesen
                s->dir = guessDir(player, *x, *y, false); // weitere Richtung schätzen
            } else {
                // zuvor wurde bereits im Schritt 3 ein Schiffsteil abgeschossen wenn wir anschliessend wieder
                // hier landen bedeutet dies, dass das Schiff in der gegenüberliegenden Richtung weitergeht
                s->dir += 2;
                s->dir %= DIRECTION_MAX;
            }
            playgroundTranslateCoordinate(x, y, s->dir);
            break;
        case (3): // Richtung gefunden, weitermachen
            s->foundOnce = true;
            playgroundTranslateCoordinate(x, y, s->dir); // Koordinate in funktionierende Richtung schieben
            // wenn nächste Koordinate bereits aufgedeckt, dann stelle Startkoordinate her
            // und suche in die andere Richtung
            if (playgroundGetState(!player, *x, *y) == FOUND) {
                *x = s->startX;
                *y = s->startY;
                s->dir += 2;
                s->dir %= DIRECTION_MAX;
                playgroundTranslateCoordinate(x, y, s->dir);
            }
            s->step = 2;
            break;
    }
}


/**
 * @brief Implementation Privater Funktionen
 * 
 */

static void pseudoRandomXY(ground_t ground, uint8_t *x, uint8_t *y) {
    static uint8_t parity = 3; // pro Spiel einmal eine Parität bestimmen
    if (parity == 3) parity = rand() % 2; // 0 - jedes gerade Feld, 1 - jedes ungerade Feld
    bool skip = false;
    do {
        playgroundGetRandomCoordinate(x, y);
        skip = playgroundGetState(ground, *x, *y) == FOUND; // überspringe bereits aufgedeckte
        skip |= *y % 2 == (*x + parity) % 2; // nur jedes zweite Feld relevant (Schachbrettmuster)
    } while (skip); // überspringe bereits aufgedeckte
}

static direction_t guessDir(ground_t player, uint8_t x, uint8_t y, bool firstGuess) {
    stateMachine_t *s = &stateMachine[player];
    if (firstGuess) { // bei Neuanfang
        s->triedDirections = 0; // Versuchte Richtungen zurücksetzen
        // Wenn Koordinate am Rand des Feldes ist, dann kann in dieser Richtung kein Schiff sein
        if (x == 0) {
            s->triedDirections |= (0x1 << LEFT);
        } else if (x == (SIZE_X - 1)) {
            s->triedDirections |= (0x1 << RIGHT);
        }
        if (y == 0) {
            s->triedDirections |= (0x1 << UP);
        } else if (y == (SIZE_Y - 1)) {
            s->triedDirections |= (0x1 << DOWN);
        }
    }
    direction_t dir;
    bool skip = false;
    do {
        uint8_t copyX = x; // arbeite nur mit der Kopie
        uint8_t copyY = y;
        dir = playgroundGetRandomDirection();
        // übergehe Richtung wenn:
        if ((s->triedDirections >> dir) & 0x1                       // bereits probiert
        || playgroundTranslateCoordinate(&copyX, &copyY, dir)       // ungültig
        || playgroundGetState(!player, copyX, copyY) == FOUND) {    // zuvor bereits besucht
            skip = true;
        } else {
            skip = false;
        }
    } while (skip);
    s->triedDirections |= (0x1 << dir);
    return dir;
}
