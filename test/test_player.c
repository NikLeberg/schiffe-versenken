/**
 * @file test_player.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Teste öffentliche Funktionen von player Modul
 * @version 0.1
 * @date 2020-10-30
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */


#include "test.h"
#include "../src/player/player.h"


/**
 * @brief Tests
 * 
 */

/**
 * @brief Bei der Koordinatenabfrage soll ein Hinweistext ausgegeben werden
 * 
 */
void test_generatesOutput() {
    TEST_STDIN_PREPARE("%s\n", "a1");
    uint8_t x, y;
    ASSERT_OUTPUT(player(&x, &y, false, false));
}

/**
 * @brief Korrekte Umwandlung von korrekt einggegebenen Koordinaten
 * 
 */
void test_acceptGoodEnteredCoordinates() {
    // Input für scanf vorbereiten
    char maxYUpper = 'A' + (SIZE_Y - 1);
    char maxYLower = 'a' + (SIZE_Y - 1);
    char maxX[3] = "00";
    maxX[0] = '0' + SIZE_X / 10;
    maxX[1] = '0' + SIZE_X % 10;
    TEST_STDIN_PREPARE("A1\na1\n%c%s\n%c%s\nA%s\n%c1\n", maxYUpper, maxX, maxYLower, maxX, maxX, maxYLower);
    // Koordinaten einlesen
    uint8_t x = 0, y = 0;
    player(&x, &y, false, false); // liest A1
    assert(x == 0 && y == 0);
    player(&x, &y, false, false); // liest a1
    assert(x == 0 && y == 0);
    player(&x, &y, false, false); // liest SIZE_Y (Gross) und SIZE_X
    assert(x == (SIZE_X - 1) && y == (SIZE_Y - 1));
    player(&x, &y, false, false); // liest SIZE_Y (Klein) und SIZE_X
    assert(x == (SIZE_X - 1) && y == (SIZE_Y - 1));
    player(&x, &y, false, false); // liest A und SIZE_X
    assert(x == (SIZE_X - 1) && y == 0);
    player(&x, &y, false, false); // liest SIZE_Y und 1
    assert(x == 0 && y == (SIZE_Y - 1));
}

/**
 * @brief Fehleingaben sollten nicht akzeptiert werden
 * 
 */
void test_discardWronglyEnteredCoordinates() {
    // Input für scanf vorbereiten
    char firstInvalidY = 'a' + SIZE_Y;
    char firstInvalidX[3] = "00";
    firstInvalidX[0] = '0' + (SIZE_X + 1) / 10;
    firstInvalidX[1] = '0' + (SIZE_X + 1) % 10;
    TEST_STDIN_PREPARE("--\na1\n%c1\na1\na%s\na1\n", firstInvalidY, firstInvalidX);
    // Koordinaten einlesen
    uint8_t x = SIZE_X, y = SIZE_Y;
    player(&x, &y, false, false); // liest --
    assert(x == 0 && y == 0);
    x = SIZE_X, y = SIZE_Y;
    player(&x, &y, false, false); // liest SIZE_Y + 1 und 1
    assert(x == 0 && y == 0);
    x = SIZE_X, y = SIZE_Y;
    player(&x, &y, false, false); // liest a und SIZE_X + 1
    assert(x == 0 && y == 0);
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
    TEST_RUN(test_generatesOutput);
    TEST_RUN(test_acceptGoodEnteredCoordinates);
    TEST_RUN(test_discardWronglyEnteredCoordinates);
    return 0;
}

/**
 * @brief Mock-Up von externen Abhängigkeiten
 * 
 */

/**
 * @brief playground Modul
 */
#include "../src/playground/playground.h"
state_t playgroundGetState(ground_t ground, uint8_t x, uint8_t y) {
    (void) ground;
    (void) x;
    (void) y;
    return HIDDEN;
}
