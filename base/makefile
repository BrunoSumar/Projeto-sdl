input = main
output = a

libs = shader.h mesh.h scene.h

path_emcc = /home/bruno/Documentos/emscripten

SHELL := /bin/bash

gcc: $(input).cpp $(libs)
	g++ $(input).cpp -g -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW -lassimp -O3 -o $(output)

emcc: $(input).cpp $(libs)
	# source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input) -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -O3 -o $(output).html
	source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input) -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s WASM=1 -lGL -lGLU -O3 -o $(output).js

run: $(input).cpp $(libs) gcc
	@echo executando: $(output)
	@./$(output)

server:
	@echo Abrindo servido http
	python3 -m http.server
