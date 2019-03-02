CC=g++
CFLAGS=-W -Wall -Wextra
EXEC= Rogue



all : $(EXEC)

Rogue: main.o Animation.o Character.o Interactions.o collision.o  Map.o
	g++ -g -o Rogue.elf main.o Animation.o Character.o Interactions.o collision.o Map.o -lsfml-graphics -lsfml-window -lsfml-system 
	
main.o: main.cpp Character.h Interactions.h
	g++ -c main.cpp -I /usr/lib/x86_64-linux-gnu/ 


collision.o: collision.cpp collision.h Character.h
	g++ -c collision.cpp -I /usr/lib/x86_64-linux-gnu/ 

Interactions.o: Interactions.cpp Interactions.h Character.h
	g++ -c Interactions.cpp -I /usr/lib/x86_64-linux-gnu/  -lm

Character.o: Character.cpp Character.h
	g++ -c Character.cpp -I /usr/lib/x86_64-linux-gnu/ 

Animation.o: Animation.cpp Animation.h
	g++ -c Animation.cpp -I /usr/lib/x86_64-linux-gnu/ 


Map.o: Map.cpp Map.h
	g++ -c Map.cpp -I /usr/lib/x86_64-linux-gnu/
	

clean:
	rm -f *.o
