output = a

src =  main.o GLutils.o
libs = element.h scene.h

path_emcc = /home/bruno/Documentos/emscripten

SHELL := /bin/bash
CXXFLAGS += -I./include

$(output): $(src) $(libs)
	@echo Compilando projeto via gcc
	@g++  $(src) -g -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW -lassimp -O3 -o $(output) -I./include

main.o: $(libs)

emcc: $(input).cpp $(libs)
	@echo Compilando projeto via emcc
	# @source $(path_emcc)/emsdk/emsdk_env.sh &&	em++ $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lGLU -O3 -o $(output).js -Xclang -isystem/usr/include
	# @source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lGLU -O3 -o $(output).js -Xclang -isystem/usr/include
	source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lassimp -O3 -o $(output).js -I/home/bruno/Documentos/emscripten

run: $(output)
	@echo executando: $(output)
	@./$(output)

server:
	@echo Iniciando servidor http
	@python3 -m http.server
