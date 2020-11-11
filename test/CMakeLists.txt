if (CMAKE_BUILD_TYPE MATCHES Debug)

add_executable(test_game test_game.c)
add_test(game test_game)

add_executable(test_playground
               test_playground.c
               ../src/playground/playground.c)
add_test(playground test_playground)

add_executable(test_statistics
               test_statistics.c)
add_test(statistics test_statistics)

add_executable(test_player
               test_player.c
               ../src/player/player.c)
add_test(player test_player)

add_executable(test_computer
               test_computer.c
               ../src/computer/computer.c)
add_test(computer test_computer)

add_executable(test_logic
               test_logic.c
               ../src/logic/logic.c)
add_test(logic test_logic)

endif()