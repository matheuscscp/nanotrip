OBJ = Animation.o Audio.o Avatar.o Border.o Button.o Camera.o Circle.o ClearSurface.o GameBGM.o GameObject.o InputManager.o InputString.o Interaction.o Item.o LevelMakerData.o LevelMakerObject.o LevelMakerPanel.o Line.o PanelAvatar.o PanelBlackHole.o PanelGeneral.o PanelItem.o PanelKey.o PanelParticle.o Particle.o Ranking.o Rectangle.o SDLBase.o Shape.o Sprite.o State.o StateCredits.o StateInstructions.o StateIntroduction.o StateLevel.o StateLevelMaker.o StateLevelMakerQuit.o StateLoadLevel.o StateMainMenu.o StateMakeLevel.o StateManager.o StateMovie.o StateNewGame.o StatePause.o StatePlay.o StateRanking.o StateTest.o StateTransition.o StateYouLose.o StateYouWin.o Stopwatch.o SurfaceManager.o Text.o TileMap.o TileSet.o Timer.o common.o configfile.o linearalgebra.o main.o observer.o

all: $(OBJ)
	g++ Debug/src/*.o -o nanotrip -L3rd/bin -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer -mwindows

%.o: src/%.cpp
	g++ -I3rd/include/SDL -c $< -o Debug/src/$@
