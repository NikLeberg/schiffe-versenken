/**
 * @file test_game.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Teste Spielkonfiguration von game.h
 * @version 0.1
 * @date 2020-10-28
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */


#include "test.h"
#include "../src/game.h"


/**
 * @brief Tests
 * 
 */

/**
 * @brief Die Spielfeldgrösse darf maximal 99 * 26 Felder sein
 * 
 */
void game_groundSizeIsInBounds() {
    ASSERT_BETWEEN(SIZE_X, 2, 99);
    ASSERT_BETWEEN(SIZE_Y, 2, 26);
}

/**
 * @brief Schiffe sind minimal 2 und maximal SIZE - 1 Felder gross.
 * 
 */
void game_shipSizesAreInBounds() {
    int16_t shipConfig[] = {SHIP_CONFIG};
    for (uint8_t ship = 0; ship < (sizeof(shipConfig) / sizeof(int16_t)); ++ship) {
        assert(shipConfig[ship] > 1); // alle Schiffe sind mindestens zwei Felder gross
        assert(shipConfig[ship] < SIZE_X || shipConfig[ship] < SIZE_Y); // Schiff passt mindestens in eine Spielfeldimension
    }
}

/**
 * @brief Das Spielfeld muss Platz für alle Schiffsteile haben
 * 
 */
void game_groundHasSpaceForAllShips() {
    int16_t shipConfig[] = {SHIP_CONFIG};
    uint32_t shipParts = 0;
    for (uint8_t ship = 0; ship < (sizeof(shipConfig) / sizeof(int16_t)); ++ship) {
        shipParts += 3 * shipConfig[ship]; // (3 *) zähle Wasser rundherum dazu
    }
    assert(SIZE_X * SIZE_Y >= shipParts);
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

    TEST_RUN(game_groundSizeIsInBounds);
    TEST_RUN(game_shipSizesAreInBounds);
    TEST_RUN(game_groundHasSpaceForAllShips);
    return 0;
}
