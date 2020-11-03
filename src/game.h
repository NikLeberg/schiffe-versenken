/**
 * @file game.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Globale Includes für das Spiel und globale Konfiguration
 * @version 0.1
 * @date 2020-09-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once


/**
 * @brief Includes
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>


/**
 * @brief Globale Definitionen und Spieleinstellungen
 * 
 */

#define SIZE_X 10 // Spielfeldgrösse in X-Richtung / Waagrecht
#define SIZE_Y 10 // Spielfeldgrösse in Y-Richtung / Senkrecht
#define SHIP_CONFIG 5, 4, 4, 3, 3, 3, 2, 2, 2, 2 // Die Länge der zu platzierender Schiffe, Mindestlänge 2

static_assert(SIZE_X > 1 && SIZE_X < 100, "Minimum 2 und wegen Printfunktion nur maximal 99 Spalten.");
static_assert(SIZE_Y > 1 && SIZE_Y <= 26, "Minimum 2 und weil das Alphabet nur A bis Z hat maximal 26 Zeilen.");
