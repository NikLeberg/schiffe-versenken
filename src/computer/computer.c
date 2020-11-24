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

// Zustandsspeicher für Algorythmus
typedef struct {
    struct { // Zustandsvariablen des Suchalgorythmus
        step_t step;
        uint8_t startX, startY;
        direction_t dir;
        bool foundOnce;
    } computer;
    struct {
        uint8_t triedDirections; // speichere bereits versuchte Richtungen
    } guessDir;
    struct {
        uint8_t parity; // speichere eine zufällige Parität per Spiel
    } pseudoRandomXY;
} algorythmState_t;


/**
 * @brief Variablendeklarationen
 * 
 */

static algorythmState_t state = { // Startzustand
    .computer = {
        .step = SEARCH_FOR_SHIP,
        .startX = SIZE_X,
        .startY = SIZE_Y,
        .dir = DIRECTION_MAX,
        .foundOnce = false
    },
    .guessDir = {
        .triedDirections = 0
    },
    .pseudoRandomXY = {
        .parity = 3
    }
};


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
    if (lastWasHit) ++(state.computer.step); // Schrittkette bei Treffer vorwärtsschieben
    if (lastHitWasLast) state.computer.step = SEARCH_FOR_SHIP; // Schrittkette bei komplett-Abschuss neustarten
    switch (state.computer.step) {
        case (SEARCH_FOR_SHIP): // zufällige Koordinate auswählen
            pseudoRandomXY(x, y);
            break;
        case (FIRST_HIT): // erster Treffer
            state.computer.startX = *x; // Koordinaten des ersten Treffers speichern
            state.computer.startY = *y;
            state.computer.foundOnce = false;
            state.computer.dir = guessDir(*x, *y, true); // schätzen und anhand Koordinatenlage gewisse Richtungen ausschliessen
            playgroundTranslateCoordinate(x, y, state.computer.dir);
            state.computer.step = SEARCH_DIRECTION;
            break;
        case (SEARCH_DIRECTION): // Richtung noch nicht gefunden
            *x = state.computer.startX;
            *y = state.computer.startY;
            if (!state.computer.foundOnce) { // wenn zuvor nicht im Schritt "DIRECTION_FOUND" gewesen
                state.computer.dir = guessDir(*x, *y, false); // weitere Richtung schätzen
            } else {
                // zuvor wurde bereits im Schritt "DIRECTION_FOUND" ein Schiffsteil abgeschossen wenn wir anschliessend
                // wieder hier landen bedeutet dies, dass das Schiff in der gegenüberliegenden Richtung weitergeht
                state.computer.dir += 2;
                state.computer.dir %= DIRECTION_MAX;
            }
            playgroundTranslateCoordinate(x, y, state.computer.dir);
            break;
        case (DIRECTION_FOUND): // Richtung gefunden, weitermachen
            state.computer.foundOnce = true;
            playgroundTranslateCoordinate(x, y, state.computer.dir); // Koordinate in funktionierende Richtung schieben
            // wenn nächste Koordinate bereits aufgedeckt, dann stelle Startkoordinate her
            // und suche in die andere Richtung
            if (playgroundGetState(PLAYER, *x, *y) == FOUND) {
                *x = state.computer.startX;
                *y = state.computer.startY;
                state.computer.dir += 2;
                state.computer.dir %= DIRECTION_MAX;
                playgroundTranslateCoordinate(x, y, state.computer.dir);
            }
            state.computer.step = SEARCH_DIRECTION; // Setze Schritt um eins zurück. Wenn das neue Ziel ein Treffer war,
            // dann wird mit ++step zum Beginn der Funktion wieder in diesen Schritt gesprungen. Ansonsten
            // wird in Schritt "SEARCH_DIRECTION" gesprungen.
            break;
    }
}

void computerReset() {
    // Startzustand wiederherstellen
    state.computer.step = SEARCH_FOR_SHIP;
    state.computer.startX = SIZE_X;
    state.computer.startY = SIZE_Y;
    state.computer.dir = DIRECTION_MAX;
    state.computer.foundOnce = false;
    state.guessDir.triedDirections = 0;
    state.pseudoRandomXY.parity = 3;
}


/**
 * @brief Implementation Privater Funktionen
 * 
 */

static void pseudoRandomXY(uint8_t *x, uint8_t *y) {
    // pro Spiel einmal eine Parität / Farbe bestimmen
    if (state.pseudoRandomXY.parity == 3) {
        state.pseudoRandomXY.parity = rand() % 2; // 0 - jedes gerade Feld, 1 - jedes ungerade Feld
    }
    bool skip = false;
    do {
        playgroundGetRandomCoordinate(x, y);
        skip = playgroundGetState(PLAYER, *x, *y) == FOUND; // überspringe bereits aufgedeckte
        skip |= *y % 2 == (*x + state.pseudoRandomXY.parity) % 2; // nur jedes zweite Feld relevant (Schachbrettmuster)
        // wird ermittelt über den Rest der Division der Koordinaten / 2, das ergibt das "Muster": 0 1 0 1 ...
        // nur wenn der Rest gleich ist (oder bei parity = 1, ungleich) ist die Koordinate "Schachbrettmusterartig".
    } while (skip);
}

static direction_t guessDir(uint8_t x, uint8_t y, bool firstGuess) {
    if (firstGuess) { // bei Neuanfang
        state.guessDir.triedDirections = 0; // Versuchte Richtungen zurücksetzen
        // Wenn Koordinate am Rand des Feldes ist, dann kann in dieser Richtung kein Schiff sein
        if (x == 0) {
            state.guessDir.triedDirections |= (0x1 << LEFT);
        } else if (x == (SIZE_X - 1)) {
            state.guessDir.triedDirections |= (0x1 << RIGHT);
        }
        if (y == 0) {
            state.guessDir.triedDirections |= (0x1 << UP);
        } else if (y == (SIZE_Y - 1)) {
            state.guessDir.triedDirections |= (0x1 << DOWN);
        }
    }
    direction_t dir;
    bool skip = false;
    do {
        uint8_t copyX = x; // arbeite nur mit einer Kopie
        uint8_t copyY = y;
        dir = playgroundGetRandomDirection();
        // übergehe Richtung wenn:
        if ((state.guessDir.triedDirections >> dir) & 0x1       // bereits probiert
        || playgroundTranslateCoordinate(&copyX, &copyY, dir)   // ungültig
        || playgroundGetState(PLAYER, copyX, copyY) == FOUND) { // zuvor bereits besucht
            skip = true;
        } else {
            skip = false;
        }
    } while (skip);
    state.guessDir.triedDirections |= (0x1 << dir);
    return dir;
}
