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

void Chip8::emulateCycle()
{
    opcode = (memory[pc] << 8) | memory[pc + 1];
    switch (opcode & 0xF000)
    {
    case 0x0000:
    {
        switch (opcode & 0x000F)
        {
        case 0x0:
        {
            for (int i = 0; i < 2048; i++)
            {
                gfx[i] = 0;
            }
            pc += 2;
            break;
        }

        case 0xE:
        {
            sp--;
            pc = stack[sp];
            pc += 2;
            break;
        }

        default:
            break;
        }
        break;
    }
    case 0x1000:
    {
        pc = opcode & 0x0FFF;
        break;
    }

    case 0x2000:
    {

        stack[sp] = pc;
        sp++;
        pc = opcode & 0x0FFF;
        break;
    }

    case 0x3000:
    {
        if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        {
            pc += 2;
        }
        pc += 2;
        break;
    }

    case 0x4000:
    {
        if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        {
            pc += 2;
        }
        pc += 2;
        break;
    }

    case 0x5000:
    {
        if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
        {
            pc += 2;
        }
        pc += 2;
        break;
    }

    case 0x6000:
    {
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        pc += 2;
        break;
    }

    case 0x7000:
    {
        V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        pc += 2;
        break;
    }

    case 0x8000:
    {
        switch (opcode & 0x000F)
        {
        case 0x0:
        {
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        }

        case 0x1:
        {
            V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        }

        case 0x2:
        {
            V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        }

        case 0x3:
        {
            V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        }

        case 0x4:
        {
            int temp = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]);
            if (temp > 255)
            {
                V[(opcode & 0x0F00) >> 8] = temp & 0xFF;
                V[0xF] = 1;
            }
            else
            {
                V[(opcode & 0x0F00) >> 8] = temp;
                V[0xF] = 0;
            }
            pc += 2;
            break;
        }

        case 0x5:
        {
            int first = V[(opcode & 0x0F00) >> 8];
            int second = V[(opcode & 0x00F0) >> 4];
            V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
            if (first < second)
            {
                V[0xF] = 0;
            }
            else
            {
                V[0xF] = 1;
            }
            pc += 2;
            break;
        }

        case 0x6:
        {
            int vx = V[(opcode & 0x0F00) >> 8];
            V[(opcode & 0x0F00) >> 8] >>= 1;
            V[0xF] = vx & 0b1;
            pc += 2;
            break;
        }

        case 0x7:
        {
            int x = V[(opcode & 0x0F00) >> 8];
            int y = V[(opcode & 0x00F0) >> 4];
            int sub = y - x;
            V[(opcode & 0x0F00) >> 8] = sub;
            if (y > x)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            pc += 2;
            break;
        }

        case 0xE:
        {
            int vx = V[(opcode & 0x0F00) >> 8];
            V[(opcode & 0x0F00) >> 8] = (vx << 1);
            pc += 2;
            break;
        }

        default:
            break;
        }
        break;
    }

    case 0x9000:
    {
        if (V[((opcode & 0x0F00) >> 8)] != V[((opcode & 0x00F0) >> 4)])
        {
            pc += 2;
        }
        pc += 2;
        break;
    }

    case 0xA000:
    {
        I = opcode & 0x0FFF;
        pc += 2;
        break;
    }

    case 0xB000:
    {
        pc = (opcode & 0x0FFF) + V[0];
        break;
    }

    case 0xC000:
    {
        V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & rand() % 255;
        pc += 2;
        break;
    }

    case 0xD000:
    {
        int x = V[(opcode & 0x0F00) >> 8] % 64;
        int y = V[(opcode & 0x00F0) >> 4] % 32;
        int n = opcode & 0x000F;
        int pixel;
        V[0xF] = 0;
        // printf("draw");
        for (int yline = 0; yline < n; yline++)
        {
            pixel = memory[I + yline];
            for (int xline = 0; xline < 8; xline++)
            {
                if ((pixel & (0x80 >> xline)) != 0)
                {
                    if (gfx[(x + xline) + ((y + yline) * 64)] == 1)
                    {
                        V[0xF] = 1;
                    }
                    gfx[(x + xline) + ((y + yline) * 64)] ^= 1;
                }
            }
        }
        pc += 2;
        break;
    }

    case 0xE000:
    {
        switch (opcode & 0x00FF)
        {
        case 0x9E:
        {
            if (key[V[(opcode & 0x0F00) >> 8]] == 1)
            {
                pc += 2;
            }
            pc += 2;
            break;
        }

        case 0xA1:
        {
            if (key[V[(opcode & 0x0F00) >> 8]] == 0)
            {
                pc += 2;
            }
            pc += 2;
            break;
        }

        default:
            break;
        }
        break;
    }

    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x07:
        {
            V[(opcode & 0x0F00) >> 8] = delay;
            pc += 2;
            break;
        }

        case 0x15:
        {
            delay = V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        }

        case 0x18:
        {
            sound = V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        }

        case 0x29:
        {
            I = V[(opcode & 0x0F00) >> 8] * 0x5;
            pc += 2;
            break;
        }

        case 0x33:
        {
            int number = V[(opcode & 0x0F00) >> 8];
            int first = ((number / 100));
            int second = (((number / 10) % 10));
            int third = ((number % 10));
            memory[I] = first;
            memory[I + 1] = second;
            memory[I + 2] = third;
            pc += 2;
            break;
        }

        case 0x55:
        {
            for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
            {
                memory[I + i] = V[i];
            }
            I += ((opcode & 0x0F00) >> 8) + 1;
            pc += 2;
            break;
        }

        case 0x65:
        {
            int loop = ((opcode & 0x0F00) >> 8);
            for (int i = 0; i <= loop; i++)
            {
                V[i] = memory[I + i];
            }
            I += loop + 1;
            pc += 2;
            break;
        }

        case 0x0A:
        {
            for (int i = 0; i < 16; i++)
            {
                if (key[i] != 1)
                {
                    return;
                }
            }
            pc += 2;
            break;
        }

        case 0x1E:
        {
            I += V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        }

        default:
            break;
        }
        break;

    default:
        break;
    }
    if (delay > 0)
        delay--;
    if (sound > 0)
        sound--;
}