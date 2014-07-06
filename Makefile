all:
	g++ src/*.cpp -o nanotrip -I3rd/include/SDL -L3rd/bin -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -mwindows
