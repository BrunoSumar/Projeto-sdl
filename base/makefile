input = main.cpp
output = a

path_emcc = /home/bruno/Documentos/emscripten

SHELL := /bin/bash

gcc:
	g++ $(input) -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -O3 -o $(output)

emcc:
	# source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input) -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -O3 -o $(output).html
	source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input) -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s WASM=1 -lGL -lGLU -O3 -o $(output).js

run:
	@echo executando $(output)
	./$(output)
