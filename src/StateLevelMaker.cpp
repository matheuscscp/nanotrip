#include "StateLevelMaker.hpp"

GAMESTATE_DEF(StateLevelMaker)

StateLevelMaker::Args::Args(const std::string& levelname) : levelname(levelname) {}

StateLevelMaker::StateLevelMaker(ArgsBase* args) : levelname(((Args*)args)->levelname) {
	// only one opaque background renderization
	bg = new Sprite("img/bg_opaco.png");
	bg->render();
	delete bg;
	
	bg = new Sprite("img/bg_transparente.png");
	
	delete args;
}

void StateLevelMaker::update() {
}

void StateLevelMaker::render() {
}
