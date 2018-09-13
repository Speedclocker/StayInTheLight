all : main

main: main.o
	g++ -g -o main main.o -lsfml-graphics -lsfml-window -lsfml-system -lm
	
main.o: main.cpp
	g++ -c main.cpp -I /usr/lib/x86_64-linux-gnu/
	
clean:
	rm -f *.o
