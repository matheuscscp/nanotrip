
OBJDIR = obj
SRCDIR = src
DOCDIR = doc

ERRLOG = ErrorLog.txt

STATE = StateMainMenu
RELEASE = -D'RELEASE="$(STATE)"'
CXXFLAGS = -ansi -pedantic -Wall -Wextra -Wunused-parameter -g -std=c++0x
INCLUDE = -I /usr/include/SDL -I /usr/local/include/SDL

EXE = game

FPS = 
RUNFLAGS = $(FPS) -p $(CURDIR)/ -s $(STATE)

LIB = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer

MOD = $(OBJDIR)/configfile.o $(OBJDIR)/linearalgebra.o $(OBJDIR)/common.o $(OBJDIR)/observer.o

OBJ0 = $(MOD) $(OBJDIR)/main.o $(OBJDIR)/SDLBase.o $(OBJDIR)/Sprite.o $(OBJDIR)/Animation.o
OBJ1 = $(OBJ0) $(OBJDIR)/TileSet.o $(OBJDIR)/TileMap.o $(OBJDIR)/GameObject.o $(OBJDIR)/Camera.o
OBJ2 = $(OBJ1) $(OBJDIR)/InputManager.o $(OBJDIR)/Particle.o $(OBJDIR)/Circle.o
OBJ3 = $(OBJ2) $(OBJDIR)/StateManager.o $(OBJDIR)/Text.o $(OBJDIR)/Audio.o $(OBJDIR)/Timer.o
OBJ4 = $(OBJ3) $(OBJDIR)/State.o $(OBJDIR)/Ranking.o $(OBJDIR)/ClearSurface.o $(OBJDIR)/SurfaceManager.o
OBJ5 = $(OBJ4) $(OBJDIR)/InputString.o $(OBJDIR)/StateTest.o $(OBJDIR)/Interaction.o
OBJ6 = $(OBJ5) $(OBJDIR)/Shape.o $(OBJDIR)/StateLevelMaker.o $(OBJDIR)/Rectangle.o
OBJ7 = $(OBJ6) $(OBJDIR)/Button.o $(OBJDIR)/StateNewGame.o $(OBJDIR)/StateLoadLevel.o
OBJ8 = $(OBJ7) $(OBJDIR)/StateMainMenu.o $(OBJDIR)/StateLevel.o $(OBJDIR)/StatePlay.o
OBJ9 = $(OBJ8) $(OBJDIR)/StateMakeLevel.o $(OBJDIR)/StateMovie.o $(OBJDIR)/StateRanking.o
OBJ10 = $(OBJ9) $(OBJDIR)/StateInstructions.o $(OBJDIR)/StateCredits.o $(OBJDIR)/StatePause.o
OBJ11 = $(OBJ10) $(OBJDIR)/Stopwatch.o $(OBJDIR)/StateYouLose.o $(OBJDIR)/Avatar.o
OBJ12 = $(OBJ11) $(OBJDIR)/StateYouWin.o $(OBJDIR)/Item.o $(OBJDIR)/LevelMakerPanel.o
OBJ13 = $(OBJ12) $(OBJDIR)/LevelMakerObject.o $(OBJDIR)/GameBGM.o $(OBJDIR)/PanelGeneral.o
OBJ14 = $(OBJ13) $(OBJDIR)/StateLevelMakerQuit.o $(OBJDIR)/LevelMakerData.o $(OBJDIR)/PanelBlackHole.o
OBJ15 = $(OBJ14) $(OBJDIR)/PanelKey.o

OBJ  = $(OBJ15)

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
#r -f -p /home/matheus/Documents/C_C++/C++/IDJ/nanotrip/ -s StateMakeLevel

release: $(OBJ) smclean
	g++ $(RELEASE) $(CXXFLAGS) $(INCLUDE) -c src/StateManager.cpp -o obj/StateManager.o
	g++ $(CXXFLAGS) $(OBJ) -o $(EXE) $(LIB)

smclean:
	rm obj/StateManager.o

clean:
	rm -rf $(EXE) $(OBJDIR)/* $(ERRLOG)

dox:
	doxygen

doxclean:
	rm -rf $(DOCDIR)/html $(DOCDIR)/latex

.PHONY: all build run test gdb release smclean clean dox doxclean
