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
#include <setjmp.h>
#include <signal.h>


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

#define TEST_STDOUT_MUTE()  assert(freopen("NUL:", "w", stdout)) // Standartoutput ins Nivana umlenken
#define TEST_STDIN_PREPARE(format, ...) do { \
    FILE *input = fopen("input.txt", "w+"); \
    fprintf(input, format, __VA_ARGS__); \
    fclose(input); \
    assert(freopen("input.txt", "r", stdin)); \
} while (0) // Standartinput vorbereiten der von zukünftigen Lesenzugriffe von stdin genutzt wird

#define ASSERT_OUTPUT(func) do { \
    assert(freopen("output.txt", "w+", stdout)); \
    func; \
    TEST_STDOUT_MUTE(); \
    FILE *output = fopen("output.txt", "r"); \
    fseek(output, 0, SEEK_END); \
    assert(ftell(output) > 0); \
    fclose(output); \
} while (0) // Leite Standartausgabe in Datei um und prüfe ob resultierende Datei grösser als 0 ist

#define ASSERT_BETWEEN(a, min, max)     assert(a >= min && a <= max)
#define ASSERT_IN_DELTA(a, b, delta)    ASSERT_BETWEEN(a, b - delta, b + delta)


static jmp_buf jumpBuffer;

void test_catchSigabort(int sig) {
    (void) sig;
    longjmp(jumpBuffer, 0);
}

#define EXPECT_ASSERT(func) do { \
    if (setjmp(jumpBuffer) == 0) { \
        __p_sig_fn_t handler = signal(SIGABRT, test_catchSigabort); \
        func; \
        signal(SIGABRT, handler); \
        assert(0 && "was expected to assert:" && #func); \
    } \
} while (0) // Erwarte ein assert(), setze ein Longjump und fange SIGABRT ab


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
