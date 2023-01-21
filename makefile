output = a

libs = figura.h scene.h mapa.h unidade.h
src = main.o GLutils.o 
imgui_src = ./imgui/backends/imgui_impl_sdl.cpp ./imgui/backends/imgui_impl_opengl3.cpp  ./imgui/imgui*.cpp
link_flags = -ldl -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW -lassimp

SHELL := /bin/bash
CXXFLAGS += -I./include/ -I./imgui/ -I./imgui/backends/

emcc_src = main.cpp GLutils.cpp
# path_emcc = /home/bruno/Documentos/emscripten/
path_emcc = ./
emcc_flags = $(CXXFLAGS)

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
	@python3 -m http.server 8000 --directory ./docs

emcc: $(src) $(libs)
	@echo Compilando projeto via emcc
	# @source $(path_emcc)/emsdk/emsdk_env.sh &&	em++ $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lGLU -O3 -o $(output).js -Xclang -isystem/usr/include
	# @source $(path_emcc)/emsdk/emsdk_env.sh &&	emcc $(input).cpp -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lGLU -O3 -o $(output).js -Xclang -isystem/usr/include
	# source $(path_emcc)emsdk/emsdk_env.sh &&	emcc $(emcc_src) $(imgui_src)  -s FULL_ES3=1 -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -lGL -lassimp -O3 -o docs/$(output).html -I$(path_emcc) $(emcc_flags)
	@source $(path_emcc)emsdk/emsdk_env.sh &&	emcc $(emcc_src) $(imgui_src) --preload-file resources/ -s FULL_ES3=1 -s USE_SDL=2 --use-preload-plugins -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s WASM=1 -O3 -o docs/$(output).html -I$(path_emcc) $(emcc_flags)

install:
	@echo Emscripten
	@git clone https://github.com/emscripten-core/emsdk.git
	@./emsdk/emsdk install latest
	@./emsdk/emsdk activate latest
	@echo Imgui
	@git clone git@github.com:ocornut/imgui.git
	@echo
	@echo Libs necessárias: sdl2 sdl2-image sdl2-ttf lassimp glew
