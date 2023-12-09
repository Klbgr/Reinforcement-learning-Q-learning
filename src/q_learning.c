/**
 * @file q_learning.c
 * @author Antoine Qiu
 * @brief Implementation of the Q-learning algorithms
 * @date 2023-12-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "q_learning.h"

Map build_map(int teleporter)
{
    // init random seed
    srand(time(NULL));

    // map initialization and memory allocation
    int teleporter_1 = 0;
    int teleporter_2 = 0;
    if (teleporter)
    {
        teleporter_1 = TELEPORTER_1;
        teleporter_2 = TELEPORTER_2;
    }
    Map map;
    map.width = 6;
    map.height = 6;
    map.map = malloc(map.height * sizeof(int *));
    for (int i = 0; i < 6; i++)
    {
        map.map[i] = malloc(map.width * sizeof(int));
    }
    int matrix[6][6] = {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GOAL_1},
                        {GOAL_2, EMPTY, EMPTY, EMPTY, teleporter_2, EMPTY},
                        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                        {EMPTY, EMPTY, WALL, WALL, WALL, WALL},
                        {EMPTY, EMPTY, WALL, teleporter_1, EMPTY, EMPTY},
                        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}};
    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            map.map[i][j] = matrix[i][j];
        }
    }
    map.q = malloc(map.height * sizeof(double **));
    for (int i = 0; i < map.height; i++)
    {
        map.q[i] = malloc(map.width * sizeof(double *));
        for (int j = 0; j < map.width; j++)
        {
            map.q[i][j] = malloc(4 * sizeof(double));
            for (int k = 0; k < 4; k++)
            {
                map.q[i][j][k] = 0;
            }
        }
    }
    map.start.x = 5;
    map.start.y = 5;
    map.agent = map.start;
    map.steps = 0;
    map.epoch = 0;
    return map;
}

void free_map(Map map)
{
    for (int i = 0; i < map.height; i++)
    {
        free(map.map[i]);
        for (int j = 0; j < map.width; j++)
        {
            free(map.q[i][j]);
        }
        free(map.q[i]);
    }
    free(map.map);
    free(map.q);
}

int check_state(Map map, State state)
{
    if (state.x < 0 || state.x >= map.width || state.y < 0 || state.y >= map.height)
    {
        return 0;
    }
    return 1;
}

int are_states_equal(State state1, State state2)
{
    if (state1.x == state2.x && state1.y == state2.y)
    {
        return 1;
    }
    return 0;
}

State find_state(Map map, enum Type type)
{
    State state = {-1, -1};
    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            if (map.map[i][j] == type)
            {
                state.x = j;
                state.y = i;
                return state;
            }
        }
    }
    return state;
}

enum Type get_type(Map map, State state)
{
    // if the state does not belong to the map, we return VOID
    if (!check_state(map, state))
    {
        return VOID;
    }
    return map.map[state.y][state.x];
}

int max_q(Map map, State state)
{
    int max_q = -10;
    for (int i = 0; i < 4; i++)
    {
        if (map.q[state.y][state.x][i] > max_q)
        {
            max_q = map.q[state.y][state.x][i];
        }
    }
    return max_q;
}

double euclidean_distance(State state1, State state2)
{
    return sqrt(pow(state1.x - state2.x, 2) + pow(state1.y - state2.y, 2));
}

State q(Map *map, State state, Params params)
{
    // choose next action
    enum Action next_action;
    double max = -10;
    for (int i = 0; i < 4; i++)
    {
        double value = map->q[state.y][state.x][i];
        if (value > max)
        {
            max = value;
            next_action = i;
        }
    }

    // if there are several max values, we choose one randomly
    int vote[4] = {0};
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        if ((*map).q[state.y][state.x][i] == max && get_type(*map, move_state(state, i)) != VOID)
        {
            vote[i] = 1;
            count++;
        }
    }

    if (count > 0)
    {
        int random_count = rand() % count;
        count = 0;
        for (int i = 0; i < 4; i++)
        {
            if (vote[i] == 1)
            {
                if (count == random_count)
                {
                    next_action = i;
                    break;
                }
                count++;
            }
        }
    }

    // only in training mode
    if (!params.test)
    {
        next_action = epsilon_greedy(*map, next_action, params.epsilon);
    }

    State next_state = move_state(state, next_action);
    enum Type type = get_type(*map, next_state);

    // if the agent is on a teleporter, we move it to the other teleporter
    if (params.teleporter && type == TELEPORTER_1)
    {
        next_state = find_state(*map, TELEPORTER_2);
    }

    // no need to update the Q-table if we are in test mode
    if (params.test)
    {
        return next_state;
    }

    // reward calculation
    int reward = 0;
    switch (type)
    {
    case EMPTY:
        // if we are in euclidean mode, we add a reward based on the distance to the goals
        if (params.euclidean)
        {
            // max distance possible on the map for normalization
            double max_distance = euclidean_distance((State){0, 0}, (State){map->width - 1, map->height - 1});
            State goal_1 = find_state(*map, GOAL_1);
            State goal_2 = find_state(*map, GOAL_2);
            // reward for the first goal is higher than the second one to encourage the agent to go to the first one
            reward = ((euclidean_distance(state, goal_1) - euclidean_distance(next_state, goal_1)) / max_distance * 1000) +
                     ((euclidean_distance(state, goal_2) - euclidean_distance(next_state, goal_2)) / max_distance * 50);
        }
        else
        {
            reward = EMPTY;
        }
        break;
    case WALL:
        reward = WALL;
        break;
    case GOAL_1:
        reward = GOAL_1;
        break;
    case GOAL_2:
        reward = GOAL_2;
        break;
    default:
        break;
    }

    // Q-table update
    (*map).q[state.y][state.x][next_action] = (1 - params.alpha) * map->q[state.y][state.x][next_action] + params.alpha * (reward + params.gamma * max_q(*map, next_state));
    return next_state;
}

State move_state(State state, enum Action action)
{
    State new_state = state;
    switch (action)
    {
    case UP:
        new_state.y--;
        break;
    case DOWN:
        new_state.y++;
        break;
    case LEFT:
        new_state.x--;
        break;
    case RIGHT:
        new_state.x++;
        break;
    default:
        break;
    }
    return new_state;
}

int check_action(Map map, enum Action action)
{
    State new_state = move_state(map.agent, action);
    if (get_type(map, new_state) != VOID)
    {
        return 1;
    }
    return 0;
}

enum Action epsilon_greedy(Map map, enum Action action, float epsilon)
{
    enum Action next_action = action;
    float random = (float)rand() / RAND_MAX;
    if (random <= epsilon)
    {
        next_action = rand() % 4;
    }

    // if the action is not possible, we choose another one
    if (!check_action(map, next_action))
    {
        next_action = epsilon_greedy(map, action, epsilon);
    }
    return next_action;
}

int load_q(Map *map, char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }

    fscanf(file, "%d\n", &(*map).epoch);

    for (int i = 0; i < (*map).height; i++)
    {
        for (int j = 0; j < (*map).width; j++)
        {
            fscanf(file, "%lf %lf %lf %lf\n", &(*map).q[i][j][0], &(*map).q[i][j][1], &(*map).q[i][j][2], &(*map).q[i][j][3]);
        }
    }

    fclose(file);
    return 1;
}

int save_q(Map map, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "%d\n", map.epoch);

    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            fprintf(file, "%lf %lf %lf %lf\n", map.q[i][j][0], map.q[i][j][1], map.q[i][j][2], map.q[i][j][3]);
        }
    }

    fclose(file);
    return 1;
}