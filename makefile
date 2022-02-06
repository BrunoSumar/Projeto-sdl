output = a

libs = figura.h scene.h mapa.h unidade.h
src = main.o GLutils.o 
imgui_src = ./imgui/backends/imgui_impl_sdl.cpp ./imgui/backends/imgui_impl_opengl3.cpp  ./imgui/imgui*.cpp
link_flags = -ldl -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW -lassimp
path_emcc = /home/bruno/Documentos/emscripten

SHELL := /bin/bash
CXXFLAGS += -I./include/ -I./imgui/ -I./imgui/backends/

$(output): $(src) $(libs)
	@echo Compilando projeto
	@c++ `sdl2-config --cflags` $(CXXFLAGS) $(src) $(imgui_src) -o a `sdl2-config --libs` $(link_flags)

main.o: $(libs)

run: $(output)
	@echo executando: $(output)
	@./$(output)

clean:
	@echo Removendo arquivos .o
	@rm *.o

server:
	@echo Iniciando servidor http
	@python3 -m http.server

emcc: $(input).cpp $(libs)
	@echo Compilando projeto via emcc
	# @source $(path_emcc)/emsdk/emsdk_env.sh &&	em++ $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lGLU -O3 -o $(output).js -Xclang -isystem/usr/include
	# @source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lGLU -O3 -o $(output).js -Xclang -isystem/usr/include
	source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lassimp -O3 -o $(output).js -I/home/bruno/Documentos/emscripten
