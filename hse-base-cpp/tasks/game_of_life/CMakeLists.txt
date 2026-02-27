add_catch(test_game_of_life test.cpp game_of_life.cpp)

add_executable(game_of_life_tui tui.cpp game_of_life.cpp)
target_include_directories(game_of_life_tui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
