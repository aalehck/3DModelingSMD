
all: lighting

lighting: mesh-parser.o main.o smd_mesh_parser.o
	g++ -Wall main.o mesh-parser.o smd_mesh_parser.o -o lighting -lGL -lGLU -lglut

main.o: main.cpp mesh-parser.h
	g++ -Wall -c main.cpp

mesh-parser.o: mesh-parser.c mesh-parser.h
	g++ -Wall -c mesh-parser.c

smd_mesh_parser.o: smd_mesh_parser.cpp mesh-parser.h
	g++ -Wall -c smd_mesh_parser.cpp
