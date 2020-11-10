/**
 * @file test.h
 * @author NikLeberg (niklaus.leuenb@gmail.com)
 * @brief Allgemeine Funktionsmakros für Testprogramme
 * @version 0.1
 * @date 2020-10-28
 * 
 * @note https://bastian.rieck.me/blog/posts/2017/simple_unit_tests/
 * 
 * @copyright Copyright (c) 2020 Niklaus Leuenberger
 * 
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>


/**
 * @brief Globale Definitionen
 * 
 */

#define TEST_BEGIN(name)    fprintf(stderr, "-- Running test \"%s\"...", name)
#define TEST_END()          fprintf(stderr, "finished\n")

#define TEST_RUN(func) do { \
    TEST_BEGIN(#func); \
    func(); \
    TEST_END(); \
} while (0)

#define TEST_STDOUT_MUTE()  (void) freopen("NUL:", "w", stdout) // Standartoutput ins Nivana umlenken
#define TEST_STDIN_PREPARE(format, ...) do { \
    FILE *input = fopen("input.txt", "w+"); \
    fprintf(input, format, __VA_ARGS__); \
    fclose(input); \
    (void) freopen("input.txt", "r", stdin); \
} while (0) // Standartinput vorbereiten der von zukünftigen Lesenzugriffe von stdin genutzt wird

#define ASSERT_OUTPUT(func) do { \
    (void) freopen("output.txt", "w+", stdout); \
    func; \
    TEST_STDOUT_MUTE(); \
    FILE *output = fopen("output.txt", "r"); \
    fseek(output, 0, SEEK_END); \
    assert(ftell(output) > 0); \
    fclose(output); \
} while (0) // Leite Standartausgabe in Datei um und prüfe ob resultierende Datei grösser als 0 ist

#define ASSERT_BETWEEN(a, min, max)     assert(a >= min && a <= max)
#define ASSERT_IN_DELTA(a, b, delta)    ASSERT_BETWEEN(a, b - delta, b + delta)

/**
 * @brief Globale Typdeklarationen
 * 
 */

/* ... */


/**
 * @brief Funktionsprototypen
 * 
 */

/* ... */




/**
 * @brief Lokale Variablen
 * 
 */

/* ... */
