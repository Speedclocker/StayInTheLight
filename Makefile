CC=g++
CFLAGS=-W -Wall -Wextra -g 
EXEC= Rogue
SFMLDIR=/usr/lib/x86_64-linux-gnu/ 
LDIR= -lsfml-graphics -lsfml-window -lsfml-system -lm


all : $(EXEC) MapCreator
	#rm -f *.o

MapCreator: MapCreator.cpp Map.o
	g++ -o $@ $^ -I$(SFMLDIR) $(LDIR) $(CFLAGS)

$(EXEC): main.o Animation.o Character.o Interactions.o collision.o Map.o Object.o
	g++ -o $@ $^ $(LDIR) $(CFLAGS)

main.o: main.cpp Character.h Interactions.h
	g++ -c $< -I$(SFMLDIR) $(CFLAGS)

collision.o: collision.cpp collision.h Character.h
	g++ -c $< -I$(SFMLDIR) $(CFLAGS)

Interactions.o: Interactions.cpp Interactions.h Character.h
	g++ -c $< -I$(SFMLDIR) $(CFLAGS) 

Character.o: Character.cpp Character.h Object.h
	g++ -c $< -I$(SFMLDIR) $(CFLAGS)

Object.o: Object.cpp Object.h
	g++ -c $< -I$(SFMLDIR) $(CFLAGS)

Animation.o: Animation.cpp Animation.h
	g++ -c $< -I$(SFMLDIR) $(CFLAGS)

Map.o: Map.cpp Map.h 
	g++ -c $< -I$(SFMLDIR) $(CFLAGS)


clean:
	rm -f *.o Rogue
