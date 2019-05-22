all: map

map: map.o
	g++ -g -o map map.o -lsfml-graphics -lsfml-window -lsfml-system -lm
map.o: map.cpp
	g++ -c map.cpp -I /usr/lib/x86_64-linux-gnu/
clean:
	rm -f *.o
