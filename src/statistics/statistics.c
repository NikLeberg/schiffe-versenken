/**
 * @file statistics.c
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Statistik Modul, Zähle mit wer wie gut trifft.
 * @version 0.1
 * @date 2020-10-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */


/**
 * @brief Includes
 * 
 */

#include "statistics.h"


/**
 * @brief Typdeklarationen
 * 
 */


/**
 * @brief Variablendeklarationen
 * 
 */

static struct {
    uint16_t hits;      // Anzahl Treffer
    uint16_t misses;    // Anzahl Fehlschüsse
    uint16_t strike;    // Längster Strike
} stats[GROUND_MAX];    // für jeden Spieler eine Statistik


/**
 * @brief Private Funktionsprototypen
 * 
 */


/**
 * @brief Implementation Öffentlicher Funktionen
 * 
 */

void statisticsUpdate(ground_t player, bool wasHit) {
    static ground_t lastPlayer = PLAYER;
    static uint16_t strike = 0;
    stats[player].hits += wasHit;
    stats[player].misses += !wasHit;
    if (lastPlayer != player) { // erkenne einen Wechsel des Spielers
        lastPlayer = player;
        if (strike > stats[lastPlayer].strike) {
            stats[lastPlayer].strike = strike; // speichere den längsten Strike
        }
        strike = 0;
    } else {
        strike++; // zähle Strike hoch
    }
}

void statisticsPrint() {
    for (ground_t i = COMPUTER; i < GROUND_MAX; i++) {
        printf("\nStatistik für %s\n", i ? "Spieler" : "Computer");
        printf("Treffer:\t\t%u\n", stats[i].hits);
        printf("Fehlschüsse:\t\t%u\n", stats[i].misses);
        printf("Treffrate:\t\t%.1f%%\n", 100.0f / (float)(stats[i].hits + stats[i].misses) * (float)stats[i].hits);
        printf("Strike:\t\t\t%u\n", stats[i].strike);
    }
}


/**
 * @brief Implementation Privater Funktionen
 * 
 */
