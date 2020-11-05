/**
 * @file test_playground.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Teste öffentliche Funktionen von playground Modul
 * @version 0.1
 * @date 2020-10-28
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */


#include "test.h"
#include "../src/playground/playground.h"


/**
 * @brief Tests
 * 
 */

/**
 * @brief Get Set Entity soll das tun was es sollte
 * 
 */
void playground_entityGetSetIsfunctional() {
    struct { // XY-Kombinationen für Tests
        uint8_t x, y;
    } xy[] = {{0, 0}, {SIZE_X - 1, 0}, {0, SIZE_Y - 1}, {SIZE_X - 1, SIZE_Y - 1}};
    for (ground_t ground = 0; ground < GROUND_MAX; ++ground) {
        for (uint8_t i = 0; i < sizeof(xy) / (2 * sizeof(uint8_t)); ++i) {
            assert(playgroundGetEntity(ground, xy[i].x, xy[i].y) == playgroundGetEntity(ground, xy[i].x, xy[i].y));
            playgroundSetEntity(ground, xy[i].x, xy[i].y, 127);
            assert(playgroundGetEntity(ground, xy[i].x, xy[i].y) == 127);
            playgroundSetEntity(ground, xy[i].x, xy[i].y, 0);
        }
    }
}

/**
 * @brief Get Set State soll das tun was es sollte
 * 
 */
void playground_stateGetSetIsFunctional() {
    struct { // XY-Kombinationen für Tests
        uint8_t x, y;
    } xy[] = {{0, 0}, {SIZE_X - 1, 0}, {0, SIZE_Y - 1}, {SIZE_X - 1, SIZE_Y - 1}};
    for (ground_t ground = 0; ground < GROUND_MAX; ++ground) {
        for (uint8_t i = 0; i < sizeof(xy) / (2 * sizeof(uint8_t)); ++i) {
            assert(playgroundGetState(ground, xy[i].x, xy[i].y) == playgroundGetState(ground, xy[i].x, xy[i].y));
            playgroundSetState(ground, xy[i].x, xy[i].y, FOUND);
            assert(playgroundGetState(ground, xy[i].x, xy[i].y) == FOUND);
            playgroundSetState(ground, xy[i].x, xy[i].y, HIDDEN);
        }
    }
}

/**
 * @brief Verschiebung von Koordinaten funktioniert wie es soll
 * 
 */
void playground_translateRespectsBounds() {
    // obere linke Ecke
    uint8_t x = 0, y = 0;
    assert(playgroundTranslateCoordinate(&x, &y, UP));
    assert(playgroundTranslateCoordinate(&x, &y, LEFT));
    assert(!playgroundTranslateCoordinate(&x, &y, RIGHT) && x == 1 && y == 0);
    assert(!playgroundTranslateCoordinate(&x, &y, DOWN) && x == 1 && y == 1);
    // obere rechte Ecke
    x = SIZE_X - 1;
    y = 0;
    assert(playgroundTranslateCoordinate(&x, &y, UP));
    assert(playgroundTranslateCoordinate(&x, &y, RIGHT));
    assert(!playgroundTranslateCoordinate(&x, &y, LEFT) && x == (SIZE_X - 2) && y == 0);
    assert(!playgroundTranslateCoordinate(&x, &y, DOWN) && x == (SIZE_X - 2) && y == 1);
    // untere linke Ecke
    x = 0;
    y = SIZE_Y - 1;
    assert(playgroundTranslateCoordinate(&x, &y, DOWN));
    assert(playgroundTranslateCoordinate(&x, &y, LEFT));
    assert(!playgroundTranslateCoordinate(&x, &y, UP) && x == 0 && y == (SIZE_Y - 2));
    assert(!playgroundTranslateCoordinate(&x, &y, RIGHT) && x == 1 && y == (SIZE_Y - 2));
    // untere rechte Ecke
    x = SIZE_X - 1;
    y = SIZE_Y - 1;
    assert(playgroundTranslateCoordinate(&x, &y, DOWN));
    assert(playgroundTranslateCoordinate(&x, &y, RIGHT));
    assert(!playgroundTranslateCoordinate(&x, &y, UP) && x == (SIZE_X - 1) && y == (SIZE_Y - 2));
    assert(!playgroundTranslateCoordinate(&x, &y, LEFT) && x == (SIZE_X - 2) && y == (SIZE_Y - 2));
}

/**
 * @brief Beim Start müssen alle Felder leer sein
 * 
 */
void playground_groundIsClearOnStart() {
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        for (uint8_t x = 0; x < SIZE_X; x++) {
            assert(playgroundGetEntity(PLAYER, x, y) == 0);
            assert(playgroundGetState(PLAYER, x, y) == HIDDEN);
            assert(playgroundGetEntity(COMPUTER, x, y) == 0);
            assert(playgroundGetState(COMPUTER, x, y) == HIDDEN);
        }
    }
}

/**
 * @brief Anzahl der platzierten Schiffsteile == verlangte Anzahl gemäss Konfiguration
 * 
 */
void playground_countOfPlacedShipsEqualToWantedShips() {
    uint8_t shipConfig[] = {SHIP_CONFIG};
    uint32_t shipParts = 0;
    for (uint8_t ship = 0; ship < sizeof(shipConfig); ++ship) {
        shipParts += shipConfig[ship];
    }
    playgroundInit(PLAYER);
    playgroundInit(COMPUTER);
    uint32_t countedParts[GROUND_MAX] = {0, 0};
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        for (uint8_t x = 0; x < SIZE_X; x++) {
            if (playgroundGetEntity(PLAYER, x, y)) {
                ++countedParts[PLAYER];
            }
            if (playgroundGetEntity(COMPUTER, x, y)) {
                ++countedParts[COMPUTER];
            }
        }
    }
    assert(countedParts[PLAYER] == shipParts);
    assert(countedParts[COMPUTER] == shipParts);
}

/**
 * @brief Schiffe berühren keine andere Schiffe
 * 
 */
void playground_shipsDontTouch() {
    uint8_t lastEntity;
    // Entitäten gleicher Art dürfen aneinander sein
    // unterschiedliche müssen von Wasser getrennt sein
    for (ground_t ground = 0; ground < GROUND_MAX; ++ground) {
        // zeilenweise
        for (uint8_t y = 0; y < SIZE_Y; ++y) {
            lastEntity = 0;
            for (uint8_t x = 0; x < SIZE_X; ++x) {
                uint8_t entity = playgroundGetEntity(ground, x, y);
                assert(((lastEntity == 0) && entity) || (lastEntity == entity) || (lastEntity && (entity == 0)));
                lastEntity = entity;
            }
        }
        // spaltenweise
        for (uint8_t x = 0; x < SIZE_X; ++x) {
            lastEntity = 0;
            for (uint8_t y = 0; y < SIZE_Y; ++y) {
                uint8_t entity = playgroundGetEntity(ground, x, y);
                assert(((lastEntity == 0) && entity) || (lastEntity == entity) || (lastEntity && (entity == 0)));
                lastEntity = entity;
            }
        }
    }
}

/**
 * @brief Zufällige Koordinaten prüfen
 * 
 */
void playground_randomCoordinatesInBounds() {
    uint8_t x, y;
    uint32_t tryNum = SIZE_X * SIZE_Y;
    tryNum *= tryNum;
    bool got[SIZE_X][SIZE_Y] = {0};
    // immer im Bereich 0 - (SIZE - 1)
    for (uint32_t i = 0; i < tryNum; ++i) {
        playgroundGetRandomCoordinate(&x, &y);
        assert(x < SIZE_X);
        assert(y < SIZE_Y);
        got[x][y] = true;
    }
    // jede Koordinate wurde mindestens einmal erwischt
    for (uint8_t x = 0; x < SIZE_X; ++x) {
        for (uint8_t y = 0; y < SIZE_Y; ++y) {
            assert(got[x][y]);
        }
    }
}

/**
 * @brief Zufällige Ausrichtung muss immer im Bereich 0 bis 3 liegen.
 * 
 */
void playground_randomDirectionInBounds() {
    direction_t dir;
    uint32_t tryNum = DIRECTION_MAX * DIRECTION_MAX * 2;
    bool got[DIRECTION_MAX] = {0};
    for (uint8_t i = 0; i < tryNum; ++i) {
        dir = playgroundGetRandomDirection();
        assert(dir < DIRECTION_MAX);
        got[dir] = true;
    }
    // jede Ausrichtung wurde mindestens einmal erhalten
    for (direction_t dir = 0; dir < DIRECTION_MAX; ++dir) {
        assert(got[dir]);
    }
}

/**
 * @brief Etwas wird ausgegeben
 * 
 */
void playground_printGeneratesOutput() {
    ASSERT_OUTPUT(playgroundPrint(PLAYER, 0, 0));
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

    TEST_RUN(playground_entityGetSetIsfunctional);
    TEST_RUN(playground_stateGetSetIsFunctional);
    TEST_RUN(playground_translateRespectsBounds);
    TEST_RUN(playground_groundIsClearOnStart);
    TEST_RUN(playground_countOfPlacedShipsEqualToWantedShips);
    TEST_RUN(playground_shipsDontTouch);
    TEST_RUN(playground_randomCoordinatesInBounds); // random erst nach init testen!
    TEST_RUN(playground_randomDirectionInBounds);
    TEST_RUN(playground_printGeneratesOutput);
    return 0;
}
