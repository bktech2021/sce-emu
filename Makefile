all:
	g++ -c ./src/chip8.cpp
	g++ -c ./src/main.cpp
	g++ chip8.o main.o -o sce -lSDL2
	rm chip8.o main.o
	mkdir bin
	mv ./sce ./bin/sce

clean:
	rm -rf ./bin