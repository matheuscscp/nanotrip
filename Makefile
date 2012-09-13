BINDIR = bin
INCDIR = include
MODDIR = mod
OBJDIR = obj
SRCDIR = src
DOCDIR = doc

SDLDIR = /usr/include/SDL

ERRLOG = ErrorLog.txt

CXXFLAGS = -ansi -pedantic -Wall -Wextra -Wunused-parameter -g -I $(INCDIR)/ -I $(SDLDIR)/

LIB = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer
MOD = $(MODDIR)/configfile.o $(MODDIR)/linearalgebra.o $(MODDIR)/common.o $(MODDIR)/observer.o
EXE = game

RUNFLAGS = -f -p /home/matheus/Documents/C_C++/C++/IDJ/game_engine/

FIRSTATE = StateTest

OBJ0 = $(OBJDIR)/main.o $(OBJDIR)/SDLBase.o $(OBJDIR)/Sprite.o $(OBJDIR)/Animation.o
OBJ1 = $(OBJ0) $(OBJDIR)/TileSet.o $(OBJDIR)/TileMap.o $(OBJDIR)/GameObject.o $(OBJDIR)/Camera.o
OBJ2 = $(OBJ1) $(OBJDIR)/Geometry.o $(OBJDIR)/InputManager.o $(OBJDIR)/Button.o
OBJ3 = $(OBJ2) $(OBJDIR)/StateManager.o $(OBJDIR)/Text.o $(OBJDIR)/Audio.o $(OBJDIR)/Timer.o
OBJ4 = $(OBJ3) $(OBJDIR)/State.o $(OBJDIR)/Ranking.o $(OBJDIR)/ClearSurface.o $(OBJDIR)/SurfaceManager.o
#OBJ5 = $(OBJ4) $(OBJDIR)/InputString.o $(OBJDIR)/StateTest.o $(OBJ6) $(OBJDIR)/StateMovie.o
OBJ5 = $(OBJ4) $(OBJDIR)/InputString.o $(OBJDIR)/StateTest.o
#OBJ6 = $(OBJ5) $(OBJDIR)/StatePauseManager.o 

OBJ  = $(OBJ5)

all: build

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

build: $(OBJ)
	g++ $(CXXFLAGS) $(MOD) $(OBJ) -o $(BINDIR)/$(EXE) $(LIB)

run: build
	$(BINDIR)/$(EXE) $(RUNFLAGS) -s $(FIRSTATE)

test: build
	$(BINDIR)/$(EXE) $(RUNFLAGS) -s StateTest

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/* $(ERRLOG)

dox:
	doxygen

doxclean:
	rm -rf $(DOCDIR)/html $(DOCDIR)/latex

gdb: build
	gdb $(BINDIR)/$(EXE)
#r -f -p /home/matheus/Documents/C_C++/C++/IDJ/game_engine/ -s StateTest