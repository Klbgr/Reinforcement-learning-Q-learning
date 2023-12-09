/**
 * @file params.c
 * @author Antoine Qiu
 * @brief Implementation of the parameters of the program
 * @date 2023-12-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "params.h"

Params parse_params(int argc, char **argv)
{
    Params params;
    params.epochs = -1;
    params.epsilon = EPSILON;
    params.alpha = ALPHA;
    params.gamma = GAMMA;
    params.euclidean = 0;
    params.teleporter = 0;
    params.loop = 0;
    params.test = 0;
    params.load = NULL;
    params.save = NULL;
    params.gui = 1;
    params.debug = 0;
    params.print = 1;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-epochs") == 0)
        {
            params.epochs = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-epsilon") == 0)
        {
            params.epsilon = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-alpha") == 0)
        {
            params.alpha = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-gamma") == 0)
        {
            params.gamma = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-euclidean") == 0)
        {
            params.euclidean = 1;
        }
        else if (strcmp(argv[i], "-teleporter") == 0)
        {
            params.teleporter = 1;
        }
        else if (strcmp(argv[i], "-loop") == 0)
        {
            params.loop = 1;
        }
        else if (strcmp(argv[i], "-test") == 0)
        {
            params.test = 1;
        }
        else if (strcmp(argv[i], "-load") == 0)
        {
            params.load = argv[++i];
        }
        else if (strcmp(argv[i], "-save") == 0)
        {
            params.save = argv[++i];
        }
        else if (strcmp(argv[i], "-nogui") == 0)
        {
            params.gui = 0;
        }
        else if (strcmp(argv[i], "-debug") == 0)
        {
            params.debug = 1;
        }
        else if (strcmp(argv[i], "-noprint") == 0)
        {
            params.print = 0;
        }
        else if (strcmp(argv[i], "-help") == 0)
        {
            print_help();
            exit(0);
        }
        else
        {
            printf("Unknown parameter: %s\n", argv[i]);
            print_help();
            exit(0);
        }
    }

    // special case for loop
    if (params.loop) {
        params.epochs = -1;
        params.euclidean = 0;
        params.teleporter = 0;
        params.test = 1;
        params.load = NULL;
        params.save = NULL;
    }

    return params;
}

void print_help()
{
    printf("Usage: ./main [options]\n");
    printf("Options:\n");
    printf("-epochs <int> (default: inf) : Number of training/testing epochs\n");
    printf("-epsilon <float> (default: %f) : Exploration rate\n", EPSILON);
    printf("-alpha <float> (default: %f) : Learning rate\n", ALPHA);
    printf("-gamma <float> (default: %f) : Discount factor\n", GAMMA);
    printf("-euclidean : Use euclidean distance instead of default reinforcement system\n");
    printf("-teleporter : Enable teleporter in the environment\n");
    printf("-loop : Make the agent go through goal 1, goal 2 and starting point\n");
    printf("-test : Enable test mode instead of train mode\n");
    printf("-load <filename> (default: NULL) : Load a saved Q-table from file\n");
    printf("-save <filename> (default: NULL) : Save the Q-table to a file\n");
    printf("-nogui : Disable the graphical user interface\n");
    printf("-debug : Enable debug mode with the Q-table shown on the screen\n");
    printf("-noprint : Disable printing information on the console\n");
    printf("-help : Print the help message\n");
}

void print_params(Params params)
{
    printf("Parameters:\n");
    printf("epochs: %d\n", params.epochs);
    printf("epsilon: %f\n", params.epsilon);
    printf("alpha: %f\n", params.alpha);
    printf("gamma: %f\n", params.gamma);
    printf("euclidean: %d\n", params.euclidean);
    printf("teleporter: %d\n", params.teleporter);
    printf("loop: %d\n", params.loop);
    printf("test: %d\n", params.test);
    printf("load: %s\n", params.load);
    printf("save: %s\n", params.save);
    printf("gui: %d\n", params.gui);
    printf("debug: %d\n", params.debug);
    printf("print: %d\n\n", params.print);
}