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

nanotrip: bin $(shell ls src/*.cpp | sed 's/.cpp/.o/g' | sed 's/src/bin/g')
	g++ bin/*.o -o $@ $(OS_LIBS) -lSDLmain -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer $(OS_FLAGS)

bin:
	mkdir -p $@

bin/%.o: src/%.cpp
	g++ $(OS_INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -rf bin/ nanotrip*

.PHONY: install-linux-deps
install-linux-deps:
	sudo apt install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev
