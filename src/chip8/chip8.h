#include <random>
#include <time.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>

class Chip8
{
private:
    uint16_t stack[16];
    uint16_t sp;

    uint8_t memory[4096];
    uint8_t V[16];

    uint16_t pc;
    uint16_t opcode;
    uint16_t I;

    uint8_t delay;

    void init();

public:
    uint8_t gfx[2048];
    uint8_t key[16];
    uint8_t sound;
    bool draw;
    void load(char *file);
    void emulateCycle();
};