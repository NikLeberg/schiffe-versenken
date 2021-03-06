/**
 * @file main.c
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Das bekannte Spiel "Schiffe versenken"
 * @version 0.1
 * @date 2020-09-17
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */

#include "game.h"
#include "playground/playground.h"
#include "logic/logic.h"
#include "player/player.h"
#include "computer/computer.h"
#include "statistics/statistics.h"

/**
 * @brief Globale Definitionen
 * 
 */


/**
 * @brief Globale Typdeklarationen
 * 
 */



/**
 * @brief Funktionsprototypen
 * 
 */


/**
 * @brief Hauptprogramm
 * 
 * @param argc Anzahl der übergebenen Befehlszeilenargumente
 * @param argv Array mit Befehlszeilenargumenten
 * @return 0
 */
int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

#if defined(_WIN32) || defined(_WIN64)
    // Windows versteht standartmässig keine Umlaute im Terminal, ändere dies
    system("chcp 65001 && cls"); // Codepage auf UTF-8 setzen, erlaubt ä ö und ü
#endif

    // Initialisiere das Spielfeld
    printf("Generiere Spielfeld des Computers, bitte warten...\n");
    playgroundInit(COMPUTER);
    printf("Generiere Spielfeld des Spielers, bitte warten...\n");
    playgroundInit(PLAYER);
    // Bildschrm leeren und zum Home (0,0) springen per ANSI-Escapes
    printf("\033[2J\033[H");
    printf("Ich bin bereit. Sind sie es? [Enter]\n");
    (void) getchar(); // auf Enter warten
    // Spielfeld ausgeben
    playgroundPrint(GROUND_MAX, SIZE_X, SIZE_Y); // da noch kein aktueller Schuss übergebe Maximalwerte

    uint8_t x = 0, y = 0;
    bool lastTargetWasHit = false;
    bool lastTargetWasLast = false;
    ground_t lastPlayer = COMPUTER; // Spieler beginnt, also war der "letzte" Spieler der Computer

    // solange keiner gewonnen hat
    while (!logicFinished(COMPUTER) && !logicFinished(PLAYER)) {
        // eruiere wer am Zug ist
        // Spieler ist dran wenn zuletzt der Computer am zug war und nicht getroffen hatte
        // oder wenn der Spieler im vorherigen Zug etwas getroffen hatte
        if (((lastPlayer == COMPUTER) && !lastTargetWasHit) || ((lastPlayer == PLAYER) && lastTargetWasHit)) { // Spieler am zug
            player(&x, &y, lastTargetWasHit, lastTargetWasLast);
            lastPlayer = PLAYER;
        } else { // ansonsten ist der Computer am zug
            computer(&x, &y, lastTargetWasHit, lastTargetWasLast);
            lastPlayer = COMPUTER;
        }
        // Bildschirm leeren und zum Home (0,0) springen per ANSI-Escapes
        printf("\033[2J\033[H");
        // Ausgeben was von wem getroffen wurde (geschossen wurde auf den anderen Spieler, desshalb Negation)
        uint8_t hit = logicComputeHit(!lastPlayer, x, y);
        lastTargetWasHit = hit;
        lastTargetWasLast = (hit == 2);
        statisticsUpdate(lastPlayer, lastTargetWasHit);
        // Spielfeld ausgeben
        playgroundPrint(!lastPlayer, x, y);
        printf("Weiter? [Enter]");
        int c;
        while((c = getchar()) != '\n' && c != EOF); // auf Enter warten und Input verwerfen
    }
    // Ende
    if (logicFinished(COMPUTER)) {
        printf("\nSie haben gewonnen!\n");
    } else {
        printf("\nComputer hat gewonnen.\n");
    }
    statisticsPrint();
    int c;
    printf("\nSpiel zuende. Programm beenden mit [Enter].");
    while((c = getchar()) != '\n' && c != EOF); // auf Enter warten und Input verwerfen
    return 0;
}
