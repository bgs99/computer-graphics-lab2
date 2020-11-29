CXX := clang++
LIBS := -lGL -lGLU -lglut -lglfw -lGLEW
FLAGS := -std=c++17 -g

main: main.cxx
	${CXX} ${FLAGS} ${LIBS}  main.cxx -o main

format: main.cxx
	clang-format -i main.cxx

run: main
	./main
