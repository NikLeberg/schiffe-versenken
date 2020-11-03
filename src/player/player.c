/**
 * @file player.c
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Player Modul, erlaubt es dem Spieler seine Koordinateneingaben zu tätigen
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
#include "player.h"


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
 * @brief Lese eine Koordinate von der Standarteingabe ein
 * 
 * @param x[out] resultierende Koordinate x, bei Fehler = 0
 * @param y[out] resultierende Koordinate y, bei Fehler = 0
 * @return true - Eingabe fehlerhaft / ungültig, false - Eingabe i.O.
 */
static bool getCoordinate(uint8_t *x, uint8_t *y);


/**
 * @brief Implementation Öffentlicher Funktionen
 * 
 */

void player(uint8_t *x, uint8_t *y, bool lastWasHit, bool lastHitWasLast) {
    (void) lastWasHit;
    (void) lastHitWasLast;
    // lese Ziel von Standarteingabe ein
    bool inputValid;
    do {
        // Zielkoordinate von Spieler erhalten
        printf("Ziel eingeben [A-%c1-%d]: ", (SIZE_Y - 1) + 'A', SIZE_X);
        inputValid = !getCoordinate(x, y);
        inputValid &= playgroundGetState(COMPUTER, *x, *y) == HIDDEN; // nur noch nicht aufgedeckte erlauben
        if (!inputValid) { // Zeile hoch: \033[A
            printf("\033[A\rFehlerhafte Eingabe.   \n"); // [A - auf obere Zeile springen, \r - Cursor an Anfang setzen
        }
    } while (!inputValid);
}


/**
 * @brief Implementation Privater Funktionen
 * 
 */

static bool getCoordinate(uint8_t *x, uint8_t *y) {
    bool inputValid = true;
    unsigned int scannedX; // scanf kann nur in normales int "sicher" einscannen
    if (scanf("%c%u", y, &scannedX) != 2) {
        inputValid = false;
    }
    while (getchar() != '\n') {}; // Eingabepuffer leeren
    *x = scannedX;
    // y umrechnen in interne Koordinaten
    if (*y >= 'a' && *y <= 'z') { // Kleinbuchstaben
        *y = *y - 'a';
    } else if (*y >= 'A' && *y <= 'Z') { // Grossbuchstaben
        *y = *y - 'A';
    } else {
        inputValid = false;
    }
    // x umrechnen in interne Koordinaten
    --(*x);
    // erhaltene Koordinaten prüfen
    if (*x >= SIZE_X || *y >= SIZE_Y) {
        inputValid = false;
    }
    // zurückmelden
    if (inputValid) {
        return false;
    } else {
        *x = 0;
        *y = 0;
        return true;
    }
}
