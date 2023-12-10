/**
 * @file main.c
 * @author Antoine Qiu
 * @brief Main file of the project
 * @date 2023-12-10
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
 * @brief Free memory and destroy GUI
 *
 * @param map Map to free
 * @param params Params
 */
void quit(Map map, Params params)
{
    if (params.gui)
    {
        destroy_gui();
    }
    free_map(map);
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
            printf("Failed to load Q-table from %s\n", params.load);
            quit(map, params);
            return 1;
        }
    }

    // find goals
    State goal_1 = find_state(map, GOAL_1);
    if (!check_state(map, goal_1))
    {
        printf("Goal %d does not exist\n", GOAL_1);
        quit(map, params);
        return 1;
    }
    State goal_2 = find_state(map, GOAL_2);
    if (!check_state(map, goal_2))
    {
        printf("Goal %d does not exist\n", GOAL_2);
        quit(map, params);
        return 1;
    }

    // init GUI
    if (params.gui)
    {
        if (!init_gui_map(map))
        {
            printf("Failed to initialize map window\n");
            quit(map, params);
            return 1;
        }
        if (params.debug && !init_gui_q(map))
        {
            printf("Failed to initialize q window\n");
            quit(map, params);
            return 1;
        }
    }

    enum Action action;
    SDL_Event event;
    int slow = 1;
    int pause = 0;
    int checkpoint_count = -1;
    int time = 0;
    struct timeval start, end;
    if (!params.test)
    {
        gettimeofday(&start, NULL);
    }

    // main loop
    while (running && ((params.epochs >= 0 && map.epoch < params.epochs) || params.epochs < 0))
    {
        if (!pause)
        {
            // check if checkpoint reached
            if (params.loop && (checkpoint_count == -1 || (checkpoint_count == 0 && are_states_equal(map.agent, goal_1)) || (checkpoint_count == 1 && are_states_equal(map.agent, goal_2)) || (checkpoint_count == 2 && are_states_equal(map.agent, map.start))))
            {
                checkpoint_count++;
                char *checkpoint_path = NULL;
                switch (checkpoint_count)
                {
                case 0:
                    checkpoint_path = strdup("./loop/goal_1.txt");
                    break;
                case 1:
                    checkpoint_path = strdup("./loop/goal_2.txt");
                    break;
                case 2:
                    checkpoint_path = strdup("./loop/start.txt");
                    break;
                default:
                    break;
                }
                if (checkpoint_path != NULL)
                {
                    if (!load_q(&map, checkpoint_path))
                    {
                        printf("Failed to load Q-table from %s\n", checkpoint_path);
                        free(checkpoint_path);
                        quit(map, params);
                        return 1;
                    }
                    free(checkpoint_path);
                }
            }

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
            if ((!params.loop && (are_states_equal(map.agent, goal_1) || are_states_equal(map.agent, goal_2))) || (params.loop && checkpoint_count == 3))
            {
                // epoch duration
                if (!params.test)
                {
                    gettimeofday(&end, NULL);
                    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                }

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
                if (params.loop)
                {
                    checkpoint_count = -1;
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

                if (!params.test)
                {
                    gettimeofday(&start, NULL);
                }
            }
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
                case SDLK_p:
                    pause = !pause;
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
    quit(map, params);
    return 0;
}