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
 * @param x[in,out] Pointer auf die Koordinate X des vorhergehenden Schusses, wird von Funktion auf neues Ziel gesetzt.
 * @param y[in,out] Pointer auf die Koordinate Y des vorhergehenden Schusses, wird von Funktion auf neues Ziel gesetzt.
 * @param lastWasHit muss true sein wenn das vorhergehende Ziel nicht ein Leerschuss war, der Computer darf nocheinmal
 *                   schiessen. Die Koordinaten X & Y müssen noch dem letzten Treffer entsprechen.
 * @param lastHitWasLast muss true sein wenn der vorhergehende Treffer das Schiff versenkt hat.
 */
void computer(uint8_t *x, uint8_t *y, bool lastWasHit, bool lastHitWasLast);

/**
 * @brief Setzt den internen Status des Algorythmus zurück um nach Spielende ein weiteres Spiel starten zu können.
 * 
 */
void computerReset();
