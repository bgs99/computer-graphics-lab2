main: main.cxx
	clang++ -std=c++17 -g -lGL -lGLU -lglut -lglfw -lGLEW main.cxx -o main

format: main.cxx
	clang-format -i main.cxx

run: main
	./main
