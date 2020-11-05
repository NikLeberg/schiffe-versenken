/**
 * @file test_logic.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Teste öffentliche Funktionen von logic Modul
 * @version 0.1
 * @date 2020-10-30
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */


#include "test.h"
#include "../src/logic/logic.h"


/**
 * @brief Tests
 * 
 */

void logic_finishedOnlyWhenAllFound() {
    assert(!logicFinished(PLAYER));
    for (uint8_t x = 0; x < 3; ++x) {
        for (uint8_t y = 0; y < 3; ++y) {
            playgroundSetState(0, x, y, FOUND);
        }
    }
    assert(logicFinished(PLAYER));
    for (uint8_t x = 0; x < 3; ++x) {
        for (uint8_t y = 0; y < 3; ++y) {
            playgroundSetState(0, x, y, HIDDEN);
        }
    }
}

void logic_hitsAreCorrectlyReported() {
    // Fehlschuss
    assert(logicComputeHit(PLAYER, 0, 0) == 0);
    for (uint8_t x = 0; x < 3; ++x) { // nur das relevante Feld aufgedeckt
        for (uint8_t y = 0; y < 3; ++y) {
            if (x == 0 && y == 0) {
                assert(playgroundGetState(0, x, y) == FOUND);
            } else {
                assert(playgroundGetState(0, x, y) == HIDDEN);
            }
        }
    }
    // Treffer
    assert(logicComputeHit(PLAYER, 0, 1) == 1);
    for (uint8_t x = 0; x < 3; ++x) { // nur das relevante Feld aufgedeckt
        for (uint8_t y = 0; y < 3; ++y) {
            if (x == 0 && (y == 0 || y == 1)) {
                assert(playgroundGetState(0, x, y) == FOUND);
            } else {
                assert(playgroundGetState(0, x, y) == HIDDEN);
            }
        }
    }
    // Versenkt
    assert(logicComputeHit(PLAYER, 1, 1) == 2);
    for (uint8_t x = 0; x < 3; ++x) { // alle Felder rund um das Schiff aufgedeckt
        for (uint8_t y = 0; y < 3; ++y) {
            assert(playgroundGetState(0, x, y) == FOUND);
        }
    }
}

/**
 * @brief Etwas wird über Stdout ausgegeben
 * 
 */
void logic_printGeneratesOutput() {
    ASSERT_OUTPUT(logicComputeHit(PLAYER, 0, 0));
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
    TEST_RUN(logic_finishedOnlyWhenAllFound);
    TEST_RUN(logic_hitsAreCorrectlyReported);
    TEST_RUN(logic_printGeneratesOutput);
    return 0;
}


/**
 * @brief Mock-Up von externen Abhängigkeiten
 * 
 */

/**
 * @brief playground Modul
 * 
 * Alles spielt sich auf einem 3x3 Feld ab. Zugriffe ausserhalb dieses Ranges werden zugelassen aber ignoriert.
 * 
 */
#include "../src/playground/playground.h"
uint8_t entities[3][3] = {{0, 1, 0}, {0, 1, 0}, {0, 0, 0}};
state_t states[3][3] = {0};
state_t playgroundGetState(ground_t ground, uint8_t x, uint8_t y) {
    (void) ground;
    return (x < 3 && y < 3) ? states[x][y] : HIDDEN;
}
void playgroundSetState(ground_t ground, uint8_t x, uint8_t y, state_t state) {
    (void) ground;
    if (x < 3 && y < 3) states[x][y] = state;
}
uint8_t playgroundGetEntity(ground_t ground, uint8_t x, uint8_t y) {
    (void) ground;
    return (x < 3 && y < 3) ? entities[x][y] : 0;
}
void playgroundSetEntity(ground_t ground, uint8_t x, uint8_t y, uint8_t entity) {
    (void) ground;
    if (x < 3 && y < 3) entities[x][y] = entity;
}
