cmake_minimum_required(VERSION 3.22)
project(nhnt LANGUAGES C)

add_executable(${PROJECT_NAME} main.c data.c ui.c)

find_package(Curses REQUIRED)

find_library(PANEL_LIBRARIES panel REQUIRED)
find_path(PANEL_INCLUDE_PATH panel.h REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_PATH}
                                                   ${PANEL_INCLUDE_PATH})

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES}
                                      ${PANEL_LIBRARIES})
