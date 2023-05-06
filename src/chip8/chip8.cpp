#include "chip8.h"

void Chip8::init()
{
    for (int i = 0; i < 16; i++)
    {
        key[i] = 0;
        stack[i] = 0;
        V[i] = 0;
    }
    for (int i = 0; i < 2048; i++)
    {
        gfx[i] = 0;
    }
    for (int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }
    sp = 0;
    I = 0;
    pc = 0x200;
    opcode = 0;
    delay = 0;
    sound = 0;
    srand(time(NULL));
}

void Chip8::load(char *file)
{
    init();
    std::ifstream input(file, std::ios::binary);
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
    if (buffer.size() == 0)
    {
        std::cout << "Unable to open file " << file << "\n";
        exit(3);
    }
    for (int i = 0; i < buffer.size(); i++)
    {
        memory[i + 0x200] = buffer[i];
    }
}
