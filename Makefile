CC=g++
CFLAGS=-W -Wall -Wextra -g 
EXEC= Rogue
SFMLDIR=/usr/lib/x86_64-linux-gnu/ 
IDIR=-Iincludes/
LFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lm
SDIR= src/
ODIR= obj/

SOURCES:=$(wildcard $(SDIR)*.cpp)
OBJECTS:=$(SOURCES:$(SDIR)%.cpp=$(ODIR)%.o)


all : $(EXEC)

$(EXEC): $(OBJECTS)	
	g++ -o $@ $^ $(IDIR) $(LFLAGS) $(CFLAGS)

$(OBJECTS): $(ODIR)%.o : $(SDIR)%.cpp
	@if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi
	$(CC) -c $< -o $@ -I$(SFMLDIR) $(IDIR) $(CFLAGS)




.PHONY: clean
clean:
	rm -f $(ODIR)*.o Rogue
