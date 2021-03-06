CXX := clang++
LIBS := -lGL -lGLU -lglut -lglfw -lGLEW -lassimp
FLAGS := -std=c++17 -g

main: *.cxx *.hpp
	${CXX} ${FLAGS} ${LIBS} *.cxx -o main

format: main.cxx
	clang-format -i main.cxx

run: main
	./main

mem: main
	valgrind --tool=memcheck ./main