all: clean
	mkdir bin
	g++ -c ./src/chip8/chip8.cpp
	g++ -c ./src/chip8/cpu.cpp
	g++ -c ./src/main.cpp
	g++ chip8.o main.o cpu.o -o sce -lSDL2
	mv ./sce ./bin/sce
	rm chip8.o cpu.o main.o

clean:
	rm -rf ./bin

uninstall:
	rm /usr/local/bin/sce

install: uninstall
	cp ./bin/sce /usr/local/bin