#include "StateTransition.hpp"

#include "StateLevel.hpp"
#include "Ranking.hpp"
#include "GameBGM.hpp"
#include "InputManager.hpp"

using namespace common;

using std::string;

GAMESTATE_DEF(StateTransition)

const int open_delays[StateTransition::Args::LAST] = {
	0,		// VERY FIRST (no screen to show)
	6500,	// FIRST level
	6500,	// SECOND level
	6500	// THIRD level
};

#define CLOSE_DELAY	11500

StateTransition::Args::Args(const string& player_name, int level) :
player_name(player_name),
level(level),
points(0)
{}

StateTransition::StateTransition(ArgsBase* args) :
bg1(0),
bg2(0),
sound_open(0),
sound_close(0)
{
	GameBGM::stop();

	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateTransition::handleKeyDown);
	
	StateLevel::FinalArgs* casted_args = dynamic_cast<StateLevel::FinalArgs*>(args);
	
	if (!casted_args) {
		this->args = (Args*)args;
		
		// just clicked play story
		if (this->args->level == Args::VERY_FIRST)
			++this->args->level;
		// thrown by state level (try again when lose)
		else {
			this->args->level = Args::FIRST;
			saveRanking();
			this->args->points = 0;
		}
	}
	else {
		this->args = (Args*)casted_args->nextargs;
		++this->args->level;
		this->args->points += casted_args->points;
	}
	
	switch (this->args->level) {
	case Args::FIRST:
		bg1 = new Sprite("img/transition/open_level1.png");
		sound_open = new Audio("sfx/transition/open_level1.wav");
		sound_open->play(1);
		bg = bg1;
		break;
		
	case Args::SECOND:
		saveRanking();
		bg1 = new Sprite("img/transition/close_level1.png");
		bg2 = new Sprite("img/transition/open_level2.png");
		sound_open = new Audio("sfx/transition/open_level2.wav");
		sound_close = new Audio("sfx/transition/close.wav");
		sound_close->play(1);
		bg = bg1;
		break;
		
	case Args::THIRD:
		saveRanking();
		bg1 = new Sprite("img/transition/close_level2.png");
		bg2 = new Sprite("img/transition/open_level3.png");
		sound_open = new Audio("sfx/transition/open_level3.wav");
		sound_close = new Audio("sfx/transition/close.wav");
		sound_close->play(1);
		bg = bg1;
		break;
		
	case Args::LAST:
		saveRanking();
		bg1 = new Sprite("img/transition/close_level3.png");
		sound_close = new Audio("sfx/transition/close.wav");
		sound_close->play(1);
		bg = bg1;
		break;
		
	default:
		break;
	}
	
	stopwatch.start();
}

StateTransition::~StateTransition() {
	if (args->level == Args::LAST) {
		delete args;
		GameBGM::play();
	}
	
	if (bg1)
		delete bg1;
	if (bg2)
		delete bg2;
	
	if (sound_open)
		delete sound_open;
	if (sound_close)
		delete sound_close;
}

void StateTransition::update() {
	// throw credits screen: the game was beaten
	if (args->level == Args::LAST) {
		if (stopwatch.time() > CLOSE_DELAY)
			throw new Change("StateCredits", new ArgsBase());
	}
	// load level
	else if (stopwatch.time() > closeDelay())
		throw new Change("StateLevel", new StateLevel::Args(eval(args->level), "StateTransition", args));
	// change bg if needed
	else if (stopwatch.time() > CLOSE_DELAY) {
		if (bg != bg2) {
			bg = bg2;
			sound_open->play(1);
		}
	}
}

void StateTransition::render() {
	bg->render();
}

void StateTransition::saveRanking() {
	Ranking::save(this->args->player_name, this->args->points);
}

int StateTransition::closeDelay() const {
	int delay = 0;
	
	if (args->level != Args::FIRST)
		delay += CLOSE_DELAY;
	if (args->level != Args::LAST)
		delay += open_delays[args->level];
	
	return delay;
}

void StateTransition::handleKeyDown(const observer::Event& event, bool &stop){
	if (args->level == Args::LAST)
		throw new Change("StateCredits", new ArgsBase());
	
	throw new Change("StateLevel", new StateLevel::Args(eval(args->level), "StateTransition", args));
}
