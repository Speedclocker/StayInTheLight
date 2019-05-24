CC=g++
CFLAGS=-W -Wall -Wextra -g 
EXEC= Rogue
SFMLDIR=/usr/lib/x86_64-linux-gnu/ 
IDIR=-Iincludes/
LDIR= -lsfml-graphics -lsfml-window -lsfml-system -lm


all : $(EXEC)
	#rm -f *.o

$(EXEC): main.o Animation.o Character.o Interactions.o collision.o Map.o Object.o
	g++ -o $@ $^ $(IDIR) $(LDIR) $(CFLAGS)

main.o: main.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)

collision.o: collision.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)

Interactions.o: Interactions.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS) 

Character.o: Character.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)

Object.o: Object.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)

Animation.o: Animation.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)

Map.o: Map.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)


clean:
	rm -f *.o Rogue
