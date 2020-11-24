/**
 * @file test_computer.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Teste öffentliche Funktionen von computer Modul
 * @version 0.1
 * @date 2020-11-10
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 * @note das playground-Modul wird benötig
 * 
 */


#include "test.h"
#include "../src/computer/computer.h"
#include "../src/playground/playground.h"


/**
 * @brief Tests
 * 
 */

/**
 * @brief Algorythmus findet das kleiste Schiff (Felderlänge 2)
 * 
 * @note Schiff muss mindestens innerhalb (SIZE_Y * SIZE_X) / 2 Versuche gefunden werden (halbes Feld)
 * @note Schiff muss dannach innerhalb 4 Versuche versenkt werden (alle Richtungen)
 * 
 */
void computer_findShipOfLengthTwo() {
    const uint32_t maxTriesUntillFound = (SIZE_Y * SIZE_X) / 2;
    const uint32_t maxTriesUntillSunk = 4;
    struct { // XY-Kombinationen für Tests in allen Ecken + Mittig
        uint8_t x1, y1;
        uint8_t x2, y2;
    } xy[] = {
        {0, 0, 0, 1}, // links Oben
        {SIZE_X - 1, 0, SIZE_X - 1, 1}, // rechts Oben
        {0, SIZE_Y - 1, 1, SIZE_Y - 1}, // links Unten
        {SIZE_X - 1, SIZE_Y - 1, SIZE_X - 2, SIZE_Y - 1}, // rechts Unten
        {SIZE_X / 2, SIZE_Y / 2, (SIZE_X / 2) + 1, SIZE_Y / 2} // mittig
    };
    // Teste an allen fünf Positionen
    for (uint8_t i = 0; i < 5; ++i) {
        // Spielfeld zurücksetzen
        for (uint8_t y = 0; y < SIZE_Y; ++y) {
            for (uint8_t x = 0; x < SIZE_X; ++x) {
                playgroundSetEntity(PLAYER, x, y, 0);
                playgroundSetState(PLAYER, x, y, HIDDEN);
            }
        }
        // Schiff platzieren
        playgroundSetEntity(PLAYER, xy[i].x1, xy[i].y1, 1);
        playgroundSetEntity(PLAYER, xy[i].x2, xy[i].y2, 1);
        // computer-Modul zurücksetzen
        computerReset();
        // Variablen
        uint8_t x = 0, y = 0;
        bool lastTargetWasHit = false;
        bool lastTargetWasLast = false;
        uint32_t tries = 0;
        uint32_t foundAtTry = 0;
        uint32_t sunkAtTry = 0;
        // Algorythmus rechnen lassen
        while (!lastTargetWasLast && tries < (maxTriesUntillFound + maxTriesUntillSunk)) {
            computer(&x, &y, lastTargetWasHit, lastTargetWasLast);
            playgroundSetState(PLAYER, x, y, FOUND);
            lastTargetWasHit = playgroundGetEntity(PLAYER, x, y);
            lastTargetWasLast = (foundAtTry && lastTargetWasHit); // zweiter Treffer ist letzter Treffer, da Schiff nur 2 Felder lang
            if (lastTargetWasHit && !foundAtTry) foundAtTry = tries;
            if (lastTargetWasHit && !sunkAtTry) sunkAtTry = tries;
            ++tries;
        }
        // Prüfen
        assert(foundAtTry < maxTriesUntillFound);
        assert((sunkAtTry - foundAtTry) < maxTriesUntillSunk);
        assert(tries < (maxTriesUntillFound + maxTriesUntillSunk));
    }
}


/**
 * @brief Testprogramm
 * 
 * @param argc Anzahl der übergebenen Befehlszeilenargumente
 * @param argv Array mit Befehlszeilenargumenten
 * @return 0
 */
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    TEST_STDOUT_MUTE();
    TEST_RUN(computer_findShipOfLengthTwo);
    return 0;
}
