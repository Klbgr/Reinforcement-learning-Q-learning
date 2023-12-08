/**
 * @file gui.h
 * @author Antoine Qiu
 * @brief Definition of the Graphical User Interface
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "q_learning.h"

/**
 * @brief Size of a cell in the GUI
 *
 */
#define CELL_SIZE 100

/**
 * @brief Initialize the GUI to show the map
 *
 * @param map Map containing various information for initialization
 * @return int Status
 */
int init_gui_map(Map map);
/**
 * @brief Initialize the GUI to show the Q-table
 *
 * @param map Map containing various information for initialization
 * @return int Status
 */
int init_gui_q(Map map);
/**
 * @brief Show the map on the screen
 *
 * @param map Map to show
 */
void show_map(Map map);
/**
 * @brief Show the Q-table on the screen
 *
 * @param map Map containing the Q-table to show
 */
void show_q(Map map);
/**
 * @brief Destroy the GUI of the map and the Q-table
 *
 */
void destroy_gui();

#endif