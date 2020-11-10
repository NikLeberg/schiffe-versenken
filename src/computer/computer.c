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

// Schrittkette des Suchalgorythmus
typedef enum {
    SEARCH_FOR_SHIP = 0,
    FIRST_HIT,
    SEARCH_DIRECTION,
    DIRECTION_FOUND
} step_t;


/**
 * @brief Variablendeklarationen
 * 
 */


/**
 * @brief Private Funktionsprototypen
 * 
 */

/**
 * @brief Erhalte eine pseudo zufällige Koordinate welche noch nicht getroffen wurde und
 *        ähnlich zu einem Schachbrett nur auf Schwarz oder Weiss ist (per Spiel wird Farbe gewählt).
 *        Denn das kleinste Schiff ist zwei Felder lang und ist somit immer auf einem schwarzen und weissen Feld.
 * 
 * @param x[out] Pointer auf X Koordinate
 * @param y[out] Pointer auf Y Koordinate
 */
static void pseudoRandomXY(uint8_t *x, uint8_t *y);

/**
 * @brief Erhalte eine zufällige Richtung welche noch nicht versucht wurde
 * 
 * @param x // aktuelle Koordinate X
 * @param y // aktuelle Koordinate Y
 * @param firstGuess muss true sein wenn die Ausrichtung von einem neuen Schiff gesucht wird
 * @return direction_t eine mögliche Ausrichtung
 */
static direction_t guessDir(uint8_t x, uint8_t y, bool firstGuess);


/**
 * @brief Implementation Öffentlicher Funktionen
 * 
 */

void computer(uint8_t *x, uint8_t *y, bool lastWasHit, bool lastHitWasLast) {
    // Zustandsvariablen des Suchalgorythmus
    static step_t step = SEARCH_FOR_SHIP;
    static uint8_t startX = SIZE_X, startY = SIZE_Y;
    static direction_t dir = DIRECTION_MAX;
    static bool foundOnce = false;
    if (lastWasHit) ++step; // Schrittkette bei Treffer vorwärtsschieben
    if (lastHitWasLast) step = SEARCH_FOR_SHIP; // Schrittkette bei komplett-Abschuss neustarten
    switch (step) {
        case (SEARCH_FOR_SHIP): // zufällige Koordinate auswählen
            pseudoRandomXY(x, y);
            break;
        case (FIRST_HIT): // erster Treffer
            startX = *x; // Koordinaten des ersten Treffers speichern
            startY = *y;
            foundOnce = false;
            dir = guessDir(*x, *y, true); // schätzen und anhand Koordinatenlage gewisse Richtungen ausschliessen
            playgroundTranslateCoordinate(x, y, dir);
            step = SEARCH_DIRECTION;
            break;
        case (SEARCH_DIRECTION): // Richtung noch nicht gefunden
            *x = startX;
            *y = startY;
            if (!foundOnce) { // wenn zuvor nicht im Schritt "DIRECTION_FOUND" gewesen
                dir = guessDir(*x, *y, false); // weitere Richtung schätzen
            } else {
                // zuvor wurde bereits im Schritt "DIRECTION_FOUND" ein Schiffsteil abgeschossen wenn wir anschliessend
                // wieder hier landen bedeutet dies, dass das Schiff in der gegenüberliegenden Richtung weitergeht
                dir += 2;
                dir %= DIRECTION_MAX;
            }
            playgroundTranslateCoordinate(x, y, dir);
            break;
        case (DIRECTION_FOUND): // Richtung gefunden, weitermachen
            foundOnce = true;
            playgroundTranslateCoordinate(x, y, dir); // Koordinate in funktionierende Richtung schieben
            // wenn nächste Koordinate bereits aufgedeckt, dann stelle Startkoordinate her
            // und suche in die andere Richtung
            if (playgroundGetState(PLAYER, *x, *y) == FOUND) {
                *x = startX;
                *y = startY;
                dir += 2;
                dir %= DIRECTION_MAX;
                playgroundTranslateCoordinate(x, y, dir);
            }
            step = SEARCH_DIRECTION; // Setze Schritt um eins zurück. Wenn das neue Ziel ein Treffer war,
            // dann wird mit ++step zum Beginn der Funktion wieder in diesen Schritt gesprungen. Ansonsten
            // wird in Schritt "SEARCH_DIRECTION" gesprungen.
            break;
    }
}


/**
 * @brief Implementation Privater Funktionen
 * 
 */

static void pseudoRandomXY(uint8_t *x, uint8_t *y) {
    static uint8_t parity = 3; // pro Spiel einmal eine Parität / Farbe bestimmen
    if (parity == 3) parity = rand() % 2; // 0 - jedes gerade Feld, 1 - jedes ungerade Feld
    bool skip = false;
    do {
        playgroundGetRandomCoordinate(x, y);
        skip = playgroundGetState(PLAYER, *x, *y) == FOUND; // überspringe bereits aufgedeckte
        skip |= *y % 2 == (*x + parity) % 2; // nur jedes zweite Feld relevant (Schachbrettmuster)
        // wird ermittelt über den Rest der Division der Koordinaten / 2, das ergibt das "Muster": 0 1 0 1 ...
        // nur wenn der Rest gleich ist (oder bei parity = 1, ungleich) ist die Koordinate "Schachbrettmusterartig".
    } while (skip);
}

static direction_t guessDir(uint8_t x, uint8_t y, bool firstGuess) {
    static uint8_t triedDirections = 0; // speichere bereits versuchte Richtungen
    if (firstGuess) { // bei Neuanfang
        triedDirections = 0; // Versuchte Richtungen zurücksetzen
        // Wenn Koordinate am Rand des Feldes ist, dann kann in dieser Richtung kein Schiff sein
        if (x == 0) {
            triedDirections |= (0x1 << LEFT);
        } else if (x == (SIZE_X - 1)) {
            triedDirections |= (0x1 << RIGHT);
        }
        if (y == 0) {
            triedDirections |= (0x1 << UP);
        } else if (y == (SIZE_Y - 1)) {
            triedDirections |= (0x1 << DOWN);
        }
    }
    direction_t dir;
    bool skip = false;
    do {
        uint8_t copyX = x; // arbeite nur mit einer Kopie
        uint8_t copyY = y;
        dir = playgroundGetRandomDirection();
        // übergehe Richtung wenn:
        if ((triedDirections >> dir) & 0x1                      // bereits probiert
        || playgroundTranslateCoordinate(&copyX, &copyY, dir)   // ungültig
        || playgroundGetState(PLAYER, copyX, copyY) == FOUND) { // zuvor bereits besucht
            skip = true;
        } else {
            skip = false;
        }
    } while (skip);
    triedDirections |= (0x1 << dir);
    return dir;
}
