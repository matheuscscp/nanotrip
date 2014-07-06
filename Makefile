all:
	g++ src/*.cpp -o nanotrip -I3rd/include/SDL -L3rd/bin -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer -mwindows
