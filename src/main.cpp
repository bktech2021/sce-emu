#ifdef __MINGW32__

// Windows mingw compiler
#define SDL_MAIN_HANDLED

#endif

/*
    TODO:
    Sound timer, decrementing but not playing
    Code, needs comments and improvement
    Write speed using SDL_ttf
*/

/*

    v0 0.0.1

*/

#include <SDL2/SDL.h>
#include <chrono>
#include <thread>
#include <string.h>
#include "chip8/chip8.h"
SDL_Window *window = NULL;
SDL_Renderer *renderer;

bool pause = false;
unsigned int delay = 1600;
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

SDL_Texture *loadTexture(std::string path)
{
    std::ifstream input(std::string("./textures/").append(path).append("Texture.bin"), std::ios::binary);
    if (!input)
    {
        std::cout << "Texture " << path << " not found.\n";
        exit(1);
    }
    SDL_Texture *fin = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    uint8_t *rawscreen = new unsigned char[2048];
    uint32_t *screen = new unsigned int[2048];
    for (int i = 0; i < 2048; i++)
    {
        rawscreen[i] = 0;
        screen[i] = 0;
    }
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
    for (int i = 0; i < 256; i++)
    {
        rawscreen[(8 * i)] = (buffer[i] >> 7);
        rawscreen[(8 * i) + 1] = (buffer[i] >> 6) & 0b1;
        rawscreen[(8 * i) + 2] = (buffer[i] >> 5) & 0b1;
        rawscreen[(8 * i) + 3] = (buffer[i] >> 4) & 0b1;
        rawscreen[(8 * i) + 4] = (buffer[i] >> 3) & 0b1;
        rawscreen[(8 * i) + 5] = (buffer[i] >> 2) & 0b1;
        rawscreen[(8 * i) + 6] = (buffer[i] >> 1) & 0b1;
        rawscreen[(8 * i) + 7] = (buffer[i]) & 0b1;
    }
    for (int i = 0; i < 2048; i++)
    {
        if (rawscreen[i])
            screen[i] = 0xFFFFFFFF;
    }
    delete[] screen;
    delete[] rawscreen;
    SDL_UpdateTexture(fin, 0, screen, 64 * sizeof(Uint32));
    return fin;
}

int main(int argc, char **argv)
{
    std::cout << "SCE Emulator version v0.0.1\n";
    int w = 1024;
    int h = 512;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
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
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    SDL_Texture *pauseTexture = loadTexture("pause");
    uint32_t pixels[2048];

    SDL_RenderSetLogicalSize(renderer, w, h);
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

    // Main loop
    SDL_Rect rect;
    rect.h = 16;
    rect.w = 16;

    while (true)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                exit(0);

            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);
                else if (e.key.keysym.sym == SDLK_F1)
                    goto load;
                else if (e.key.keysym.sym == SDLK_F2)
                {
                    if (delay > 1)
                    {
                        delay -= 1000;
                    }
                }
                else if (e.key.keysym.sym == SDLK_F3)
                    delay += 1000;
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
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, pauseTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
            continue;
        }

        emu.emulateCycle();

        if (emu.draw)
        {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            for (int i = 0; i < 2048; i++)
                pixels[i] = 0;
            for (int x = 0; x < 64; x++)
            {
                for (int y = 0; y < 32; y++)
                {
                    if (emu.gfx[x + (y * 64)] == 1)
                    {
                        rect.x = x * 16;
                        rect.y = y * 16;
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }
            }
            SDL_RenderPresent(renderer);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }

    return 0;
}