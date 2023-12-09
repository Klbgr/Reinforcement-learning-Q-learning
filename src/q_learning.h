/**
 * @file q_learning.h
 * @author Antoine Qiu
 * @brief Definition of the Q-learning algorithms
 * @date 2023-12-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "params.h"

/**
 * @brief Cell types of the map
 *
 */
enum Type
{
    /**
     * @brief Empty cell
     *
     */
    EMPTY = 0,
    /**
     * @brief Wall cell
     *
     */
    WALL = -10,
    /**
     * @brief Goal 1 cell
     *
     */
    GOAL_1 = 1000,
    /**
     * @brief Goal 2 cell
     *
     */
    GOAL_2 = 50,
    /**
     * @brief Teleporter 1 cell
     *
     */
    TELEPORTER_1 = -1,
    /**
     * @brief Teleporter 2 cell
     *
     */
    TELEPORTER_2 = -2,
    /**
     * @brief Void cell, outsite of the map
     *
     */
    VOID = -3
};

/**
 * @brief Actions that the agent can take
 *
 */
enum Action
{
    /**
     * @brief Go up
     *
     */
    UP = 0,
    /**
     * @brief Go down
     *
     */
    DOWN = 1,
    /**
     * @brief Go left
     *
     */
    LEFT = 2,
    /**
     * @brief Go right
     *
     */
    RIGHT = 3
};

/**
 * @brief Structure representing a state or a position in the map
 *
 */
typedef struct
{
    /**
     * @brief X coordinate
     *
     */
    int x;
    /**
     * @brief Y coordinate
     *
     */
    int y;
} State;

/**
 * @brief Structure representing the map
 *
 */
typedef struct
{
    /**
     * @brief The actual map of shape (height, width)
     *
     */
    int **map;
    /**
     * @brief Q-table of shape (height, width, action)
     *
     */
    double ***q;
    /**
     * @brief Width of the map
     *
     */
    int width;
    /**
     * @brief Height of the map
     *
     */
    int height;
    /**
     * @brief Position of the agent
     *
     */
    State agent;
    /**
     * @brief Starting position of the agent
     *
     */
    State start;
    /**
     * @brief Step counter
     *
     */
    int steps;
    /**
     * @brief Epoch counter
     *
     */
    int epoch;
} Map;

/**
 * @brief Initialize the map structure and build the map
 *
 * @param teleporter Enable teleporter
 * @return Map Initialized map
 */
Map build_map(int teleporter);
/**
 * @brief Free the map structure
 *
 * @param map Map to free
 */
void free_map(Map map);
/**
 * @brief Check if the state belongs to the map
 *
 * @param map Map to look in
 * @param state State to check
 * @return int Status
 */
int check_state(Map map, State state);
/**
 * @brief Check if two states are equal
 *
 * @param state1 State 1
 * @param state2 State 2
 * @return int Status
 */
int are_states_equal(State state1, State state2);
/**
 * @brief Find the position of a cell type in the map
 *
 * @param map Map to search in
 * @param type Type to search
 * @return State Position of the cell
 */
State find_state(Map map, enum Type type);
/**
 * @brief Get the type of a cell
 *
 * @param map Map to search in
 * @param state Position of the cell
 * @return enum Type Type of the cell
 */
enum Type get_type(Map map, State state);
/**
 * @brief Find the maximum Q-value of a state
 *
 * @param map Map to look in
 * @param state State to find the maximum Q-value
 * @return int Maximum Q-value
 */
int max_q(Map map, State state);
/**
 * @brief Calculate the euclidean distance between two states
 *
 * @param state1 State 1
 * @param state2 State 2
 * @return double Euclidean distance
 */
double euclidean_distance(State state1, State state2);
/**
 * @brief Q-learning algorithm
 *
 * @param map Map to use
 * @param state Current state
 * @param params Parameters
 * @return State Next state
 */
State q(Map *map, State state, Params params);
/**
 * @brief Move a state in a direction
 *
 * @param state State to move
 * @param action Action to take
 * @return State State after the action
 */
State move_state(State state, enum Action action);
/**
 * @brief Check if an action is possible by the agent
 *
 * @param map Map containing the agent
 * @param action Action to check
 * @return int Status
 */
int check_action(Map map, enum Action action);
/**
 * @brief Epislon-greedy policy
 *
 * @param map Map containing the agent
 * @param action Intended action
 * @param epsilon Epsilon value
 * @return enum Action Final action
 */
enum Action epsilon_greedy(Map map, enum Action action, float epsilon);
/**
 * @brief Load a Q-table from a file
 *
 * @param map Map to store the loaded Q-table
 * @param filename Name of the file containing the Q-table
 * @return int Status
 */
int load_q(Map *map, char *filename);
/**
 * @brief Save the Q-table to a file
 *
 * @param map Map containing the Q-table to save
 * @param filename Name of the file to save the Q-table
 * @return int Status
 */
int save_q(Map map, char *filename);

#endif