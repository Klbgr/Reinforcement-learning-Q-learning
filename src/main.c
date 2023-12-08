/**
 * @file main.c
 * @author Antoine Qiu
 * @brief Main file of the project
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include "q_learning.h"
#include "gui.h"
#include "params.h"

/**
 * @brief Running state of the program
 *
 */
int running = 1;

/**
 * @brief Ctrl+C handler
 *
 * @param signum Signal number
 */
void ctrl_c_handler(int signum)
{
    running = 0;
}

/**
 * @brief Main function
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return int Status
 */
int main(int argc, char **argv)
{
    signal(SIGINT, ctrl_c_handler);

    // init params
    Params params = parse_params(argc, argv);

    print_params(params);

    // init and build map
    Map map = build_map(params.teleporter);

    // load Q-table
    if (params.load != NULL)
    {
        if (load_q(&map, params.load))
        {
            printf("Loaded Q-table from %s\n\n", params.load);
        }
        else
        {
            printf("Failed to load Q-table from %s\n\n", params.load);
        }
    }

    // find goals
    State goal_1 = find_type(map, GOAL_1);
    if (!check_state(map, goal_1))
    {
        printf("Goal %d does not exist\n", GOAL_1);
        return 1;
    }
    State goal_2 = find_type(map, GOAL_2);
    if (!check_state(map, goal_2))
    {
        printf("Goal %d does not exist\n", GOAL_2);
        return 1;
    }

    // init GUI
    if (params.gui)
    {
        if (!init_gui_map(map))
        {
            printf("Failed to initialize map window\n");
            return 1;
        }
        if (params.debug && !init_gui_q(map))
        {
            printf("Failed to initialize q window\n");
            return 1;
        }
    }

    enum Action action;
    SDL_Event event;
    int slow = 1;
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // main loop
    while (running && ((params.epochs >= 0 && map.epoch < params.epochs) || params.epochs < 0))
    {
        // show GUI
        if (params.gui)
        {
            show_map(map);
            if (params.debug)
            {
                show_q(map);
            }
            // slow mode
            if (slow)
            {
                SDL_Delay(100);
            }
        }

        // get next state
        State next_state = q(&map, map.agent, params);
        if (get_type(map, next_state) != WALL)
        {
            map.agent = next_state;
        }
        map.steps++;

        // check if goal reached
        if (are_states_equal(map.agent, goal_1) || are_states_equal(map.agent, goal_2))
        {
            // epoch duration
            gettimeofday(&end, NULL);
            int time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

            // show GUI
            if (params.gui)
            {
                show_map(map);
                if (params.debug)
                {
                    show_q(map);
                }
                if (slow)
                {
                    SDL_Delay(100);
                }
            }

            // reset agent
            map.agent = map.start;
            if (!params.test)
            {
                map.epoch++;
            }

            // print epoch info
            if (params.print)
            {
                if (params.test)
                {
                    printf("%d steps\n", map.steps);
                }
                else
                {
                    printf("Epoch %d/%d\t%d steps\t%d ms\n", map.epoch, params.epochs, map.steps, time);
                }
            }
            map.steps = 0;

            gettimeofday(&start, NULL);
        }

        // handle SDL events
        while (SDL_PollEvent(&event))
        {
            // window close button
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            // key pressed
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                // q or escape to quit
                case SDLK_q:
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                // space to toggle slow mode
                case SDLK_SPACE:
                    slow = !slow;
                    break;
                default:
                    break;
                }
            }
        }
    }

    // save Q-table
    if (params.save != NULL)
    {
        if (save_q(map, params.save))
        {
            printf("\nSaved Q-table to %s\n", params.save);
        }
        else
        {
            printf("\nFailed to save Q-table to %s\n", params.save);
        }
    }

    // free memory and destroy GUI
    if (params.gui)
    {
        destroy_gui();
    }
    free_map(map);
    return 0;
}