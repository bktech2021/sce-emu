#ifdef __MINGW32__

// Windows mingw compiler
#define SDL_MAIN_HANDLED
#include <string.h>

#endif

/*
    Sound timer, decrementing but not playing
    Screen, using SDL_Rect, needs to be texture
    Code, needs comments and improvement
    Write speed using SDL_ttf

*/

/*

    wip0 .0.1

*/

#include <SDL2/SDL.h>
#include "chip8.h"
bool pause = false;
unsigned int delay = 5;
SDL_Event e;
uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v};

int main(int argc, char **argv)
{
    int w = 1024;
    int h = 512;
    SDL_Window *window = NULL;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(2);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, w, h);
    SDL_Rect pixel;
    pixel.h = 16;
    pixel.w = 16;
    Chip8 emu;

load:
    if (strcmp(argv[1], "-h") == 0)
    {
        std::cout << "Usage: " << argv[0] << " <rom name>\n";
        exit(0);
    }
    else
    {
        emu.load(argv[1]);
    }
    while (true)
    {
        SDL_Delay(delay);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                exit(0);

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);
                else if (e.key.keysym.sym == SDLK_F1)
                    goto load;
                else if (e.key.keysym.sym == SDLK_F2)
                {
                    if (delay > 1)
                    {
                        delay--;
                    }
                }
                else if (e.key.keysym.sym == SDLK_F3)
                    delay++;
                else if (e.key.keysym.sym == SDLK_F5)
                {
                    if (pause)
                    {
                        pause = false;
                    }
                    else
                    {
                        pause = true;
                    }
                }

                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        emu.key[i] = 1;
                    }
                }
            }
            else if (e.type == SDL_KEYUP)
            {
                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        emu.key[i] = 0;
                    }
                }
            }
        }
        if (pause)
        {
            continue;
        }
        emu.emulateCycle();
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                if (emu.gfx[i + (j * 64)] == 1)
                {
                    pixel.x = i * 16;
                    pixel.y = j * 16;
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }
        SDL_RenderPresent(renderer);
    }

    return 0;
}