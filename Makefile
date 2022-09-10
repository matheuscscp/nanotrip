OBJ = Animation.o Audio.o Avatar.o Border.o Button.o Camera.o Circle.o ClearSurface.o GameBGM.o \
	GameObject.o InputManager.o InputString.o Interaction.o Item.o LevelMakerData.o LevelMakerObject.o \
	LevelMakerPanel.o Line.o PanelAvatar.o PanelBlackHole.o PanelGeneral.o PanelItem.o PanelKey.o \
	PanelParticle.o Particle.o Ranking.o Rectangle.o SDLBase.o Shape.o Sprite.o State.o StateCredits.o \
	StateInstructions.o StateIntroduction.o StateLevel.o StateLevelMaker.o StateLevelMakerQuit.o \
	StateLoadLevel.o StateMainMenu.o StateMakeLevel.o StateManager.o StateMovie.o StateNewGame.o \
	StatePause.o StatePlay.o StateRanking.o StateTest.o StateTransition.o StateYouLose.o StateYouWin.o \
	Stopwatch.o SurfaceManager.o Text.o TileMap.o TileSet.o Timer.o common.o configfile.o linearalgebra.o \
	main.o observer.o

WIN_LIBS = -L3rd/bin -lmingw32
WIN_FLAGS = -mwindows
WIN_INCLUDES = -I3rd/include/SDL

OS_LIBS =
OS_FLAGS =
OS_INCLUDES = -I/usr/include/SDL

ifneq ($(shell uname -s),Linux)
	OS_LIBS = $(WIN_LIBS)
	OS_FLAGS = $(WIN_FLAGS)
	OS_INCLUDES = $(WIN_INCLUDES)
endif

all: $(OBJ)
	g++ Debug/src/*.o -o nanotrip $(OS_LIBS) -lSDLmain -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer $(OS_FLAGS)

%.o: src/%.cpp
	g++ $(OS_INCLUDES) -c $< -o Debug/src/$@

.PHONY: install-linux-deps
install-linux-deps:
	sudo apt install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev
