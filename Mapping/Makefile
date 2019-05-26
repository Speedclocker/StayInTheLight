CC=g++
CFLAGS=-W -Wall -Wextra -g 
EXEC= MapCreator
SFMLDIR=-I/usr/lib/x86_64-linux-gnu/ 
IDIR=-I../includes
LDIR= -lsfml-graphics -lsfml-window -lsfml-system -lm
SDIR= ../src/
ODIR= ../obj/

SOURCES:=$(filter-out $(SDIR)loadMap.cpp, $(wildcard $(SDIR)*.cpp))
OBJECTS:=$(SOURCES:$(SDIR)%.cpp=$(ODIR)%.o)


all : $(EXEC)

$(EXEC): $(OBJECTS) saveMap.o MapCreator.cpp
	g++ -o $@ $^ $(IDIR) $(LFLAGS) $(LDIR) $(CFLAGS)

MapCreator.o: MapCreator.cpp
	g++ -c $< -o $@ -I$(SFMLDIR) $(IDIR) $(CFLAGS)

$(OBJECTS): $(ODIR)%.o : $(SDIR)%.cpp
	@if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi
	$(CC) -c $< -o $@ -I$(SFMLDIR) $(IDIR) $(CFLAGS)

saveMap.o: saveMap.cpp
	g++ -c $< -o $@ -I$(SFMLDIR) $(IDIR) $(CFLAGS)



clean:
	rm -f *.o MapCreator



#all : MapCreator
#	rm -f *.o

#MapCreator: MapCreator.cpp $(ODIR)Map.o saveMap.o
#	g++ -o $@ $^ $(SFMLDIR) $(IDIR) $(LDIR) $(CFLAGS)

#$(ODIR)Map.o: $(CDIR)Map.cpp
#	g++ -c $< -o $@ -I$(SFMLDIR) $(IDIR) $(CFLAGS)

#saveMap.o: saveMap.cpp
#	g++ -c $< -o $@ -I$(SFMLDIR) $(IDIR) $(CFLAGS)




