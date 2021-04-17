
gameapp: main.o
	g++ main.o -o gameapp -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp -std=c++11

