/**
 * @file gui.c
 * @author Antoine Qiu
 * @brief Implementation of the Graphical User Interface
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "gui.h"

/**
 * @brief Window of the map
 *
 */
SDL_Window *map_window = NULL;
/**
 * @brief Renderer of the map
 *
 */
SDL_Renderer *map_renderer = NULL;
/**
 * @brief Window of the Q-table
 *
 */
SDL_Window *q_window = NULL;
/**
 * @brief Renderer of the Q-table
 *
 */
SDL_Renderer *q_renderer = NULL;
/**
 * @brief Font of the GUI
 *
 */
TTF_Font *font = NULL;

int init_gui_map(Map map)
{
    if (!SDL_WasInit(SDL_INIT_EVERYTHING))
    {
        SDL_Init(SDL_INIT_EVERYTHING);
    }
    if (map_window == NULL)
    {
        map_window = SDL_CreateWindow("Q-learning",
                                      SDL_WINDOWPOS_CENTERED - map.width * (CELL_SIZE / 2),
                                      SDL_WINDOWPOS_CENTERED,
                                      map.width * CELL_SIZE, map.height * CELL_SIZE,
                                      SDL_WINDOW_SHOWN);
        SDL_SetWindowResizable(map_window, SDL_FALSE);
    }
    if (map_renderer == NULL)
    {
        map_renderer = SDL_CreateRenderer(map_window, -1, SDL_RENDERER_ACCELERATED);
    }

    if (!SDL_WasInit(SDL_INIT_EVERYTHING) || map_window == NULL || map_renderer == NULL)
    {
        return 0;
    }
    return 1;
}

int init_gui_q(Map map)
{
    if (!SDL_WasInit(SDL_INIT_EVERYTHING))
    {
        SDL_Init(SDL_INIT_EVERYTHING);
    }

    if (q_window == NULL)
    {
        q_window = SDL_CreateWindow("Q-table",
                                    SDL_WINDOWPOS_CENTERED + map.width * (CELL_SIZE / 2),
                                    SDL_WINDOWPOS_CENTERED,
                                    map.width * CELL_SIZE, map.height * CELL_SIZE,
                                    SDL_WINDOW_SHOWN);
        SDL_SetWindowResizable(q_window, SDL_FALSE);
    }
    if (q_renderer == NULL)
    {
        q_renderer = SDL_CreateRenderer(q_window, -1, SDL_RENDERER_ACCELERATED);
    }

    if (!TTF_WasInit())
    {
        TTF_Init();
    }
    if (font == NULL)
    {
        font = TTF_OpenFont("./font/COMIC.TTF", 24);
    }

    if (!SDL_WasInit(SDL_INIT_EVERYTHING) || q_window == NULL || q_renderer == NULL || !TTF_WasInit() || font == NULL)
    {
        return 0;
    }
    return 1;
}

void show_map(Map map)
{
    SDL_SetRenderDrawColor(map_renderer, 0, 0, 0, 255);
    SDL_RenderClear(map_renderer);
    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            SDL_Rect rect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_Rect small_rect = {j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE + CELL_SIZE / 4, CELL_SIZE / 2, CELL_SIZE / 2};

            switch (map.map[i][j])
            {
            case EMPTY:
                SDL_SetRenderDrawColor(map_renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(map_renderer, &rect);
                break;
            case WALL:
                break;
            case GOAL_1:
            case GOAL_2:
                SDL_SetRenderDrawColor(map_renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(map_renderer, &rect);
                SDL_SetRenderDrawColor(map_renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(map_renderer, &small_rect);
                break;
            case TELEPORTER_1:
            case TELEPORTER_2:
                SDL_SetRenderDrawColor(map_renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(map_renderer, &rect);
                SDL_SetRenderDrawColor(map_renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(map_renderer, &small_rect);
                break;
            }
        }
    }
    SDL_SetRenderDrawColor(map_renderer, 0, 0, 255, 255);
    SDL_Rect rect = {map.agent.x * CELL_SIZE + CELL_SIZE / 4, map.agent.y * CELL_SIZE + CELL_SIZE / 4, CELL_SIZE / 2, CELL_SIZE / 2};
    SDL_RenderFillRect(map_renderer, &rect);

    SDL_RenderPresent(map_renderer);
}

void show_q(Map map)
{
    SDL_SetRenderDrawColor(q_renderer, 0, 0, 0, 255);
    SDL_RenderClear(q_renderer);
    SDL_Surface *surface;
    SDL_Texture *texture;
    char text[10];
    int size = CELL_SIZE / 3;
    SDL_Rect rect, action;

    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            rect = (SDL_Rect){j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_SetRenderDrawColor(q_renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(q_renderer, &rect);

            for (int k = 0; k < 4; k++)
            {
                switch (k)
                {
                case UP:
                    action = (SDL_Rect){j * CELL_SIZE + size, i * CELL_SIZE, size, size};
                    break;
                case DOWN:
                    action = (SDL_Rect){j * CELL_SIZE + size, i * CELL_SIZE + size * 2, size, size};
                    break;
                case LEFT:
                    action = (SDL_Rect){j * CELL_SIZE, i * CELL_SIZE + size, size, size};
                    break;
                case RIGHT:
                    action = (SDL_Rect){j * CELL_SIZE + size * 2, i * CELL_SIZE + size, size, size};
                    break;
                }
                SDL_Color color;
                double value = map.q[i][j][k];
                if (value <= -10)
                {
                    color = (SDL_Color){255, 0, 0, 255};
                }
                else if (value < 0)
                {
                    color = (SDL_Color){255 * 0.25 + (255 * 0.75) * (-value / 10), 0, 0, 255};
                }
                else if (value == 0)
                {
                    color = (SDL_Color){255, 255, 255, 255};
                }
                else if (value >= 1000)
                {
                    color = (SDL_Color){0, 255, 0, 255};
                }
                else
                {
                    color = (SDL_Color){0, 255 * 0.25 + (255 * 0.75) * (value / 1000), 0, 255};
                }
                sprintf(text, "%.1f", value);
                surface = TTF_RenderText_Solid(font, text, color);
                texture = SDL_CreateTextureFromSurface(q_renderer, surface);
                SDL_RenderCopy(q_renderer, texture, NULL, &action);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
        }
    }

    SDL_RenderPresent(q_renderer);
}

void destroy_gui()
{
    if (map_renderer != NULL)
    {
        SDL_DestroyRenderer(map_renderer);
        map_renderer = NULL;
    }
    if (map_window != NULL)
    {
        SDL_DestroyWindow(map_window);
        map_window = NULL;
    }
    if (q_renderer != NULL)
    {
        SDL_DestroyRenderer(q_renderer);
        q_renderer = NULL;
    }
    if (q_window != NULL)
    {
        SDL_DestroyWindow(q_window);
        q_window = NULL;
    }
    if (font != NULL)
    {
        TTF_CloseFont(font);
    }
    SDL_Quit();
    TTF_Quit();
}