/**
 * @file statistics.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Statistik Modul, Zähle mit wer wie gut trifft.
 * @version 0.1
 * @date 2020-10-04
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
 * @brief Aktualisiere die Statistik
 * 
 * @param player welcher Spieler gerade am Zug war
 * @param wasHit wie der Zug ausgegangen ist
 */
void statisticsUpdate(ground_t player, bool wasHit);

/**
 * @brief Gebe die Statistik aus
 * 
 */
void statisticsPrint();
