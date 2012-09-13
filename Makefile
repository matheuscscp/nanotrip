
OBJDIR = obj
SRCDIR = src
DOCDIR = doc

SDLDIR = /usr/include/SDL

ERRLOG = ErrorLog.txt

CXXFLAGS = -ansi -pedantic -Wall -Wextra -Wunused-parameter -g -I $(SDLDIR)/

EXE = game

RUNFPS = 
FIRSTATE = StateTest
RUNFLAGS = $(RUNFPS) -p $(CURDIR)/ -s $(FIRSTATE)

LIB = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer

MOD = $(OBJDIR)/configfile.o $(OBJDIR)/linearalgebra.o $(OBJDIR)/common.o $(OBJDIR)/observer.o

OBJ0 = $(MOD) $(OBJDIR)/main.o $(OBJDIR)/SDLBase.o $(OBJDIR)/Sprite.o $(OBJDIR)/Animation.o
OBJ1 = $(OBJ0) $(OBJDIR)/TileSet.o $(OBJDIR)/TileMap.o $(OBJDIR)/GameObject.o $(OBJDIR)/Camera.o
OBJ2 = $(OBJ1) $(OBJDIR)/Geometry.o $(OBJDIR)/InputManager.o $(OBJDIR)/Button.o
OBJ3 = $(OBJ2) $(OBJDIR)/StateManager.o $(OBJDIR)/Text.o $(OBJDIR)/Audio.o $(OBJDIR)/Timer.o
OBJ4 = $(OBJ3) $(OBJDIR)/State.o $(OBJDIR)/Ranking.o $(OBJDIR)/ClearSurface.o $(OBJDIR)/SurfaceManager.o
#OBJ5 = $(OBJ4) $(OBJDIR)/InputString.o $(OBJDIR)/StateTest.o $(OBJ6) $(OBJDIR)/StateMovie.o
OBJ5 = $(OBJ4) $(OBJDIR)/InputString.o $(OBJDIR)/StateTest.o
#OBJ6 = $(OBJ5) $(OBJDIR)/StatePauseManager.o 
#OBJ  = $(OBJ6)
OBJ  = $(OBJ5)

all: build

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

build: $(OBJ)
	g++ $(CXXFLAGS) $(OBJ) -o $(EXE) $(LIB)

run: build
	$(EXE) $(RUNFLAGS)

test: build
	$(EXE) -f -p $(CURDIR)/ -s StateTest

gdb: build
	gdb $(EXE)
#r -f -p /home/matheus/Documents/C_C++/C++/IDJ/game_engine/ -s StateTest

clean:
	rm -rf $(EXE) $(OBJDIR)/* $(ERRLOG)

dox:
	doxygen

doxclean:
	rm -rf $(DOCDIR)/html $(DOCDIR)/latex

.PHONY: all build run test gdb clean dox doxclean
