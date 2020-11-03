/**
 * @file computer.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Computer Modul, realisiert die Logik des Computerspielers
 * @version 0.1
 * @date 2020-09-30
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


/**
 * @brief Variablendeklarationen
 * 
 */


/**
 * @brief Öffentliche Funktionen
 * 
 */

/**
 * @brief Durchläuft den Computeralgorythmus und setzt die Koordinaten auf ein nächstes Ziel
 * 
 * @param x[in,out] Pointer auf die Koordinate X, wird von Fuktion auf neues Ziel gesetzt.
 * @param y[in,out] Pointer auf die Koordinate Y, wird von Fuktion auf neues Ziel gesetzt.
 * @param lastWasHit muss true sein wenn das vorhergehende Ziel nicht ein Leerschuss war, des Computer darf nocheinmal
 *                   schiessen. Die Koordinaten X & Y entsprechen noch dem letzten Treffer
 * @param lastHitWasLast muss true sein wenn der vorhergehende Treffer das Schiff versenkt hat.
 */
void computer(ground_t player, uint8_t *x, uint8_t *y, bool lastWasHit, bool lastHitWasLast);
