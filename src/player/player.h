/**
 * @file player.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Player Modul, erlaubt es dem Spieler seine Koordinateneingaben zu tätigen
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
 * @brief Erhalte ein neues Ziel vom Spielerdinaten auf ein nächstes Ziel
 * 
 * @param x[in,out] Pointer auf die Koordinate X, wird von Fuktion auf neues Ziel gesetzt.
 * @param y[in,out] Pointer auf die Koordinate Y, wird von Fuktion auf neues Ziel gesetzt.
 * @param lastWasHit muss true sein wenn das vorhergehende Ziel nicht ein Leerschuss war, der Spieler darf nocheinmal
 *                   schiessen. Die Koordinaten X & Y entsprechen noch dem letzten Treffer
 * @param lastHitWasLast muss true sein wenn der vorhergehende Treffer das Schiff versenkt hat.
 */
void player(uint8_t *x, uint8_t *y, bool lastWasHit, bool lastHitWasLast);
