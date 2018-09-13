all : main

main: main.o Character.o
	g++ -g -o main main.o Character.o -lsfml-graphics -lsfml-window -lsfml-system -lm
	
	
main.o: main.cpp Character.h
	g++ -c main.cpp -I /usr/lib/x86_64-linux-gnu/ 
	
Character.o: Character.cpp Character.h
	g++ -c Character.cpp -I /usr/lib/x86_64-linux-gnu/ 
	
clean:
	rm -f *.o
