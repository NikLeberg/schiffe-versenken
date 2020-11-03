/**
 * @file logic.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Logik Modul, Funktionen der Spiellogik
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

#include "../playground/playground.h"
#include "../game.h"


/**
 * @brief Typdeklarationen
 * 
 */


/**
 * @brief Variablendeklarationen
 * 
 */


/**
 * @brief Öffentliche Funktionen
 * 
 */

/**
 * @brief Prüft ob das Spiel auf dem angegebenen Spielfeld zuende ist. Der Spieler der
 * dieses Feld beschiesst hat gewonnen wenn Rückgabe = true.
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @return true - Spiel ist beendet, false - noch nicht fertig
 */
bool logicFinished(ground_t ground);

/**
 * @brief Verarbeite die Spiellogik bei Treffer und gebe Rückmeldung an Spieler.
 * 
 * @param ground ID des Spielfeldes (0 = Computer, 1 = Spieler)
 * @param x Koordinate X
 * @param y Koordinate Y
 * @return uint8_t 0 - nichts getroffen, 1 - getroffen aber nicht versenkt, 2 - getroffen und versenkt
 */
uint8_t logicComputeHit(ground_t ground, uint8_t x, uint8_t y);
