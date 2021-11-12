input = main
output = a

libs =

path_emcc = /home/bruno/Documentos/emscripten

SHELL := /bin/bash

gcc: $(input).o $(libs)
	g++ $(input).o -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -O3 -o $(output)

emcc:
	# source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input) -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -O3 -o $(output).html
	source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input) -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s WASM=1 -lGL -lGLU -O3 -o $(output).js

run: $(output) $(libs)
	@echo executando $(output)
	./$(output)

server:
	@echo Abrindo servido http
	python3 -m http.server
