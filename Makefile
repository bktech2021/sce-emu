all:
	g++ -c ./src/chip8/chip8.cpp
	g++ -c ./src/main.cpp
	rm -rf ./bin/
	mkdir bin
	mkdir ./bin/textures
	g++ chip8.o main.o -o ./bin/sce -lSDL2
	touch ./bin/textures/pauseTexture.bin
	cp ./src/textures/pauseTexture.bin ./bin/textures/pauseTexture.bin
	rm chip8.o
	rm main.o

clean:
	rm sce

uninstall:
	rm -rf /usr/local/bin/sce

install: uninstall
	cp ./bin/sce /usr/local/bin