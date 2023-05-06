#ifdef __MINGW32__

// Windows mingw compiler
#define SDL_MAIN_HANDLED
#include <string.h>

#endif

/*
    TODO:
    Sound timer, decrementing but not playing
    Code, needs comments and improvement
    Write speed using SDL_ttf
*/

/*

    wip0 0.0.1

*/

#include <SDL2/SDL.h>
#include "chip8/chip8.h"

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
    std::cout << "SCE Emulator version pre0.0.1\n";
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
    if (argc < 2)
    {
        std::cout << "No file specified.\nUsage: " << argv[0] << " <rom name>\n";
        exit(4);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    SDL_Texture *pauseTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    uint32_t pixels[2048];
    for (int i = 0; i < 2048; i++)
        pixels[i] = 0;
    pixels[(13 * 64) + 30] = 0xFFFFFFFF;
    pixels[(13 * 64) + 33] = 0xFFFFFFFF;
    pixels[(14 * 64) + 30] = 0xFFFFFFFF;
    pixels[(14 * 64) + 33] = 0xFFFFFFFF;
    pixels[(15 * 64) + 30] = 0xFFFFFFFF;
    pixels[(15 * 64) + 33] = 0xFFFFFFFF;
    pixels[(16 * 64) + 30] = 0xFFFFFFFF;
    pixels[(16 * 64) + 33] = 0xFFFFFFFF;
    pixels[(17 * 64) + 30] = 0xFFFFFFFF;
    pixels[(17 * 64) + 33] = 0xFFFFFFFF;
    pixels[(18 * 64) + 30] = 0xFFFFFFFF;
    pixels[(18 * 64) + 33] = 0xFFFFFFFF;
    SDL_UpdateTexture(pauseTexture, NULL, pixels, 64 * sizeof(Uint32));
    SDL_RenderSetLogicalSize(renderer, w, h);
    SDL_Rect pixel;
    pixel.h = 16;
    pixel.w = 16;
    Chip8 emu;

load:
    if (strcmp(argv[1], "-h") == 0)
    {
        std::cout << "Usage: " << argv[0] << " <rom name>\n-h   Help (here)";
        exit(0);
    }
    else
    {
        emu.load(argv[1]);
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
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
            // TODO: Make a pause texture
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, pauseTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
            continue;
        }
        emu.emulateCycle();
        for (int i = 0; i < 2048; i++)
            pixels[i] = 0;
        if (emu.draw)
        {
            SDL_RenderClear(renderer);
            for (int x = 0; x < 64; x++)
            {
                for (int y = 0; y < 32; y++)
                {
                    if (emu.gfx[x + (y * 64)] == 1)
                    {
                        pixels[x + (y * 64)] = 0xFFFFFFFF;
                        SDL_UpdateTexture(screenTexture, NULL, pixels, 64 * sizeof(Uint32));
                    }
                }
            }
            SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

    return 0;
}