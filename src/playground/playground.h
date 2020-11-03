/**
 * @file playground.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Playground Modul, verwaltet das Spielfeld und initialisiert dieses
 * @version 0.1
 * @date 2020-09-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once


/**
 * @brief Includes
 * 
 */

#include "../game.h"


/**
 * @brief Typdeklarationen
 * 
 */

typedef enum {      // Spielfeld
    COMPUTER = 0,   // Spielfeld des Computers auf dessen der Spieler schiesst
    PLAYER,         // Spielfeld des Spielers auf dessen der Computer schiesst
    GROUND_MAX
} ground_t;

typedef enum {      // Ausrichtung
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
    DIRECTION_MAX
} direction_t;

typedef enum {  // Koordinatenzustand
    HIDDEN,     // noch nicht getroffen
    FOUND,      // getroffen
    STATE_MAX
} state_t;


/**
 * @brief Variablendeklarationen
 * 
 */

/* ... */


/**
 * @brief Öffentliche Funktionen
 * 
 */

/**
 * @brief Erhalte die Entität der Koordinate
 * 
 * @param ground ID des Spielfeldes
 * @param x X Koordinate
 * @param y Y Koordinate
 * @return uint8_t entity Entität (0 = Wasser, >0 = ID des Schiffs)
 */
uint8_t playgroundGetEntity(ground_t ground, uint8_t x, uint8_t y);

/**
 * @brief Setze die Entität der Koordinate
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x X Koordinate
 * @param y Y Koordinate
 * @param entity Entität (0 = Wasser, >0 = ID des Schiffs)
 */
void playgroundSetEntity(ground_t ground, uint8_t x, uint8_t y, uint8_t entity);

/**
 * @brief Erhalte den Status der Koordinate
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x X Koordinate
 * @param y Y Koordinate
 * @return state_t Status (HIDDEN oder FOUND)
 */
state_t playgroundGetState(ground_t ground, uint8_t x, uint8_t y);

/**
 * @brief Setze den Status der Koordinate
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x X Koordinate
 * @param y Y Koordinate
 * @param state Status (HIDDEN oder FOUND)
 */
void playgroundSetState(ground_t ground, uint8_t x, uint8_t y, state_t state);

/**
 * @brief Gebe eine zufällige Koordinate zurück
 * 
 * @param[out] x zufällige Koorinate zwischen 0 bis (SIZE_X - 1)
 * @param[out] y zufällige Koorinate zwischen 0 bis (SIZE_Y - 1)
 */
void playgroundGetRandomCoordinate(uint8_t *x, uint8_t *y);

/**
 * @brief Gebe eine zufällige Orientierung zurück
 * 
 * @return direction_t 0 - Hoch, 1 - Rechts, 2 - Runter, 3 - Links
 */
direction_t playgroundGetRandomDirection();

/**
 * @brief Koordinate entsprechend der Ausrichtung verschieben
 * 
 * @param x[in,out] X Koordinate
 * @param y[in,out] Y Koordinate
 * @param dir Verschieberichtung (0 - hoch, 1 - rechts, 2 - runter, 3 - links)
 * @return true - ungültige Translation, false - gültig
 */
bool playgroundTranslateCoordinate(uint8_t *x, uint8_t *y, direction_t dir);

/**
 * @brief Initialisiert das Spielfeld und setzt die Schiffe
 * 
 * Typischerweise:
 *  1x Schlachtschiff, 5 Felder
 *  2x Kreuzer, 4 Felder
 *  3x Zerstörer, 3 Felder
 *  4x U-Boote, 2 Felder
 * hängt von SHIP_CONFIG ab.
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 */
void playgroundInit(ground_t ground);

/**
 * @brief
 * 
 */

/**
 * @brief Gebe die Spielfelder aus. ('.' - noch nicht aufgedeckt, '~' - Wasser, '#' - Schiff)
 * 
 * @param ground Spielfeld des letzten Schusses, wird eingefärbt
 * @param x X Koordinate des letzten Schusses, wird eingefärbt
 * @param y Y Koordinate des letzten Schusses, wird eingefärbt
 */
void playgroundPrint(ground_t ground, uint8_t x, uint8_t y);
