input = main.cpp
output = a

gcc:
	g++ $(input) -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -O3 -o $(output)

emcc:
	g++ $(input) -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -O3 -o $(output).html

run:
	@echo executando $(output)
	./$(output)
