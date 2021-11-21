input = main
output = a

libs = shader.h mesh.h scene.h

path_emcc = /home/bruno/Documentos/emscripten

SHELL := /bin/bash

$(output): $(input).cpp $(libs)
	@echo Compilando projeto via gcc
	@g++ $(input).cpp -g -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW -lassimp -O3 -o $(output)

emcc: $(input).cpp $(libs)
	@echo Compilando projeto via emcc
	@source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input) -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s WASM=1 -lGL -lGLU -O3 -o $(output).js

run: $(output)
	@echo executando: $(output)
	@./$(output)

server:
	@echo Iniciando servidor http
	@python3 -m http.server
