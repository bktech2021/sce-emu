all:
	g++ -c chip8.cpp
	g++ -c main.cpp
	g++ chip8.o main.o -o chemu -lSDL2
	rm chip8.o main.o