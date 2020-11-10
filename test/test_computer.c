/**
 * @file test_computer.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Teste öffentliche Funktionen von computer Modul
 * @version 0.1
 * @date 2020-11-10
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */


#include "test.h"
#include "../src/computer/computer.h"


/**
 * @brief Tests
 * 
 */

/**
 * @brief Dummy Test
 * 
 */
void computer_dummy() {
    uint8_t x = 0, y = 0;
    computer(&x, &y, false, false);
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

    TEST_RUN(computer_dummy);
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
void playgroundGetRandomCoordinate(uint8_t *x, uint8_t *y) {
    *x = 0;
    *y = 0;
}
direction_t playgroundGetRandomDirection() {
    return RIGHT;
}
bool playgroundTranslateCoordinate(uint8_t *x, uint8_t *y, direction_t dir) {
    (void) x;
    (void) y;
    (void) dir;
    return true;
}
