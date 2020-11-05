/**
 * @file test_statistics.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Teste funktionen von statistik Modul
 * @version 0.1
 * @date 2020-10-30
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */


#include "test.h"
// statistics Modul enthält keine öffentliche Funktionen dessen Effekte man über die öffentliche
// Schnittstelle prüfen könnte. Desshalb muss c-File eingebunden werden um auf private Funktionen
// und Variablen zugreifen zu können.
#include "../src/statistics/statistics.h"
#include "../src/statistics/statistics.c"


/**
 * @brief Tests
 * 
 */

/**
 * @brief Zu beginn sollte die Statistik leer sein
 * 
 */
void statistics_statsClearOnStart() {
    for (ground_t ground = 0; ground > GROUND_MAX; ++ground) {
        assert(!stats[ground].hits);
        assert(!stats[ground].misses);
        assert(!stats[ground].strike);
    }
}

/**
 * @brief Ein Hit soll die Werte korrekt aktualisieren
 * 
 */
void statistics_updateCalculationIsCorrect() {
    for (ground_t ground = 0; ground > GROUND_MAX; ++ground) {
        // ein Miss ändert nur Anzahl der Misses
        statisticsUpdate(ground, false);
        assert(!stats[ground].hits);
        assert(stats[ground].misses == 1);
        assert(!stats[ground].strike);
        // ein Hit ändert Anzahl der Hits und verlängert den Strike
        statisticsUpdate(ground, true);
        assert(stats[ground].hits == 1);
        assert(stats[ground].misses == 1);
        assert(stats[ground].strike == 1);
        // zweiter Hit erhöht den Strike
        statisticsUpdate(ground, true);
        assert(stats[ground].hits == 2);
        assert(stats[ground].misses == 1);
        assert(stats[ground].strike == 2);
        // folgender Miss erhöht Misses aber lässt Strike unverändert
        statisticsUpdate(ground, false);
        assert(stats[ground].hits == 2);
        assert(stats[ground].misses == 2);
        assert(stats[ground].strike == 2);
        // drei Hits erhöhen den Strike richtig
        statisticsUpdate(ground, true);
        assert(stats[ground].hits == 3);
        assert(stats[ground].misses == 2);
        assert(stats[ground].strike == 2);
        statisticsUpdate(ground, true);
        assert(stats[ground].hits == 4);
        assert(stats[ground].misses == 2);
        assert(stats[ground].strike == 2);
        statisticsUpdate(ground, true);
        assert(stats[ground].hits == 5);
        assert(stats[ground].misses == 2);
        assert(stats[ground].strike == 3);
    }
}

/**
 * @brief Etwas wird ausgegeben
 * 
 */
void statistics_printGeneratesOutput() {
    ASSERT_OUTPUT(statisticsPrint());
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

    TEST_RUN(statistics_statsClearOnStart);
    TEST_RUN(statistics_updateCalculationIsCorrect);
    TEST_RUN(statistics_printGeneratesOutput);
    return 0;
}
