CC=g++
CFLAGS=-W -Wall -Wextra -g 
EXEC= Rogue
SFMLDIR=-I/usr/lib/x86_64-linux-gnu/ 
IDIR=-I../includes
LDIR= -lsfml-graphics -lsfml-window -lsfml-system -lm
CDIR= ../src/


all : MapCreator
	#rm -f *.o

MapCreator: MapCreator.cpp Map.o saveMap.o
	g++ -o $@ $^ $(SFMLDIR) $(IDIR) $(LDIR) $(CFLAGS)

Map.o: $(CDIR)Map.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)

saveMap.o: saveMap.cpp
	g++ -c $< -I$(SFMLDIR) $(IDIR) $(CFLAGS)

clean:
	rm -f *.o MapCreator
