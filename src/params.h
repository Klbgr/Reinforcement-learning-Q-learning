/**
 * @file params.h
 * @author Antoine Qiu
 * @brief Definition of the parameters of the program
 * @date 2023-12-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PARAMS_H
#define PARAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Default vaue for alpha
 *
 */
#define ALPHA 0.1
/**
 * @brief Default value for gamma
 *
 */
#define GAMMA 0.9
/**
 * @brief Default value for epsilon
 *
 */
#define EPSILON 0.01

/**
 * @brief Structure containing the parameters of the program
 *
 */
typedef struct
{
    /**
     * @brief Number of epochs to run
     *
     */
    int epochs;
    /**
     * @brief Epsilon value for the epsilon-greedy policy
     *
     */
    float epsilon;
    /**
     * @brief Alpha value for the Q-learning algorithm
     *
     */
    float alpha;
    /**
     * @brief Gamma value for the Q-learning algorithm
     *
     */
    float gamma;
    /**
     * @brief Use euclidean distance instead of default reinforcement system
     *
     */
    int euclidean;
    /**
     * @brief Enable the teleporter on the map
     *
     */
    int teleporter;
    /**
     * @brief Make the agent go through goal 1, goal 2 and starting point
     * 
     */
    int loop;
    /**
     * @brief Enable the testing mode instead of the training mode
     *
     */
    int test;
    /**
     * @brief Path to the file to load the Q-table from
     *
     */
    char *load;
    /**
     * @brief Path to the file to save the Q-table to
     *
     */
    char *save;
    /**
     * @brief Enable the GUI
     *
     */
    int gui;
    /**
     * @brief Enable debug mode with the Q-table shown on the screen
     *
     */
    int debug;
    /**
     * @brief Enable printing information on the console
     *
     */
    int print;
} Params;

/**
 * @brief Parse the parameters of the program from the command line
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Params Parameters of the program
 */
Params parse_params(int argc, char **argv);
/**
 * @brief Print the help of the program
 *
 */
void print_help();
/**
 * @brief Print a summary of the parameters of the program
 *
 * @param params Parameters of the program
 */
void print_params(Params params);

#endif