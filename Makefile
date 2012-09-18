
OBJDIR = obj
SRCDIR = src
DOCDIR = doc

ERRLOG = ErrorLog.txt

CXXFLAGS = -ansi -pedantic -Wall -Wextra -Wunused-parameter -g
INCLUDE = -I /usr/include/SDL -I /usr/local/include/SDL

EXE = game

RUNFPS = 
FIRSTATE = StateMainMenu
RUNFLAGS = $(RUNFPS) -p $(CURDIR)/ -s $(FIRSTATE)

LIB = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer

MOD = $(OBJDIR)/configfile.o $(OBJDIR)/linearalgebra.o $(OBJDIR)/common.o $(OBJDIR)/observer.o

OBJ0 = $(MOD) $(OBJDIR)/main.o $(OBJDIR)/SDLBase.o $(OBJDIR)/Sprite.o $(OBJDIR)/Animation.o
OBJ1 = $(OBJ0) $(OBJDIR)/TileSet.o $(OBJDIR)/TileMap.o $(OBJDIR)/GameObject.o $(OBJDIR)/Camera.o
OBJ2 = $(OBJ1) $(OBJDIR)/InputManager.o $(OBJDIR)/Particle.o $(OBJDIR)/Circle.o
OBJ3 = $(OBJ2) $(OBJDIR)/StateManager.o $(OBJDIR)/Text.o $(OBJDIR)/Audio.o $(OBJDIR)/Timer.o
OBJ4 = $(OBJ3) $(OBJDIR)/State.o $(OBJDIR)/Ranking.o $(OBJDIR)/ClearSurface.o $(OBJDIR)/SurfaceManager.o
OBJ5 = $(OBJ4) $(OBJDIR)/InputString.o $(OBJDIR)/StateTest.o $(OBJDIR)/Interaction.o
OBJ6 = $(OBJ5) $(OBJDIR)/Shape.o $(OBJDIR)/Level.o $(OBJDIR)/StateLevelMaker.o $(OBJDIR)/Rectangle.o
OBJ7 = $(OBJ6) $(OBJDIR)/Button.o $(OBJDIR)/StateNewGame.o $(OBJDIR)/StateLoadLevel.o
OBJ8 = $(OBJ7) $(OBJDIR)/StateMainMenu.o $(OBJDIR)/StateLevel.o $(OBJDIR)/StatePlay.o
OBJ9 = $(OBJ8) $(OBJDIR)/StateMakeLevel.o $(OBJDIR)/StateMovie.o $(OBJDIR)/StateRanking.o
OBJ10 = $(OBJ9) $(OBJDIR)/StateInstructions.o $(OBJDIR)/StateCredits.o
OBJ11 = $(OBJ10) 

OBJ  = $(OBJ11)

all: build

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

build: $(OBJ)
	g++ $(CXXFLAGS) $(OBJ) -o $(EXE) $(LIB)

run: build
	./$(EXE) $(RUNFLAGS)

test: build
	./$(EXE) -f -p $(CURDIR)/ -s StateTest

gdb: build
	gdb $(EXE)
#r -f -p /home/matheus/Documents/C_C++/C++/IDJ/nanotrip/ -s StateMainMenu

clean:
	rm -rf $(EXE) $(OBJDIR)/* $(ERRLOG)

dox:
	doxygen

doxclean:
	rm -rf $(DOCDIR)/html $(DOCDIR)/latex

.PHONY: all build run test gdb clean dox doxclean
