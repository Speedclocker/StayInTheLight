CC=g++
CFLAGS=-W -Wall -Wextra -g 
EXEC= MapCreator
SFMLDIR=-I/usr/lib/x86_64-linux-gnu/ 
LDIR= -lsfml-graphics -lsfml-window -lsfml-system -lm
IGAMEDIR=-I../includes
IMAPDIR=-Iincludes
SGAMEDIR= ../src/
OGAMEDIR= ../obj/
SMAPDIR= src/
OMAPDIR= obj/

SOURCES_GAME:=$(filter-out $(SGAMEDIR)loadMap.cpp, $(wildcard $(SGAMEDIR)*.cpp))
OBJECTS_GAME:=$(SOURCES_GAME:$(SGAMEDIR)%.cpp=$(OGAMEDIR)%.o)

SOURCES_MAP:=$(wildcard $(SMAPDIR)*.cpp)
OBJECTS_MAP:=$(SOURCES_MAP:$(SMAPDIR)%.cpp=$(OMAPDIR)%.o)


all : $(EXEC)

$(EXEC): $(OBJECTS_GAME) $(OBJECTS_MAP) MapCreator.cpp
	g++ -o $@ $^ $(IGAMEDIR) $(IMAPDIR) $(LFLAGS) $(LDIR) $(CFLAGS)

MapCreator.o: MapCreator.cpp
	g++ -c $< -o $@ $(SFMLDIR) $(IGAMEDIR) $(IMAPDIR)  $(CFLAGS)

$(OBJECTS_GAME): $(OGAMEDIR)%.o : $(SGAMEDIR)%.cpp
	@if [ ! -d $(OGAMEDIR) ]; then mkdir $(OGAMEDIR); fi
	$(CC) -c $< -o $@ -I$(SFMLDIR) $(IGAMEDIR) $(CFLAGS)

$(OBJECTS_MAP): $(OMAPDIR)%.o : $(SMAPDIR)%.cpp
	@if [ ! -d $(OMAPDIR) ]; then mkdir $(OMAPDIR); fi
	$(CC) -c $< -o $@ -I$(SFMLDIR) $(IGAMEDIR) $(IMAPDIR) $(CFLAGS)


.PHONY: clean
clean:
	rm -f $(OBJECTS_MAP)*.o MapCreator






