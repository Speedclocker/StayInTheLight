CC=g++
CFLAGS=-W -Wall -Wextra -g 
EXECGAME= Rogue
EXECMAP= MapCreator
SFMLDIR=-I/usr/lib/x86_64-linux-gnu/ 
IGAMEDIR=-Iincludes/game/
IMAPDIR=-Iincludes/mapcreator/
LFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lm
SGAMEDIR= src/game/
OGAMEDIR= obj/game/
SMAPDIR= src/mapcreator/
OMAPDIR= obj/mapcreator/

SOURCES_GAME:=$(wildcard $(SGAMEDIR)*.cpp)
OBJECTS_GAME:=$(SOURCES_GAME:$(SGAMEDIR)%.cpp=$(OGAMEDIR)%.o)

SOURCES_MAP:=$(wildcard $(SMAPDIR)*.cpp)
OBJECTS_MAP:=$(SOURCES_MAP:$(SMAPDIR)%.cpp=$(OMAPDIR)%.o)



all : $(EXECGAME)



$(EXECGAME): $(OBJECTS_GAME)	
	$(CC) -o $@ $^ $(IGAMEDIR) $(LFLAGS) $(CFLAGS)

$(EXECMAP): $(filter-out $(OGAMEDIR)loadContent.o $(OGAMEDIR)main_Rogue.o, $(OBJECTS_GAME)) $(OBJECTS_MAP)
	g++ -o $@ $^ $(IGAMEDIR) $(IMAPDIR) $(LFLAGS) $(LDIR) $(CFLAGS)




$(OBJECTS_GAME): $(OGAMEDIR)%.o : $(SGAMEDIR)%.cpp
	@if [ ! -d $(OGAMEDIR) ]; then mkdir $(OGAMEDIR); fi
	$(CC) -c $< -o $@ $(SFMLDIR) $(IGAMEDIR) $(CFLAGS)


$(OBJECTS_MAP): $(OMAPDIR)%.o : $(SMAPDIR)%.cpp
	@if [ ! -d $(OMAPDIR) ]; then mkdir $(OMAPDIR); fi
	$(CC) -c $< -o $@ $(SFMLDIR) $(IGAMEDIR) $(IMAPDIR) $(CFLAGS)



.PHONY: clean
clean:
	rm -f $(OGAMEDIR)*.o Rogue *.o
	rm -f $(OMAPDIR)*.o MapCreator *.o