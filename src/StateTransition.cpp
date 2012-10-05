#include "StateTransition.hpp"

#include "StateLevel.hpp"
#include "Ranking.hpp"

#define SCREEN_DELAY	2500

using std::string;

GAMESTATE_DEF(StateTransition)

StateTransition::Args::Args(const string& player_name, char level) :
player_name(player_name),
level(level),
points(0)
{}

StateTransition::StateTransition(ArgsBase* args) : bg1(0), bg2(0) {
	StateLevel::FinalArgs* casted_args = dynamic_cast<StateLevel::FinalArgs*>(args);
	bool try_again = false;
	
	if (!casted_args) {
		this->args = (Args*)args;
		
		// thrown by state level (try again)
		if (this->args->level != Args::VERY_FIRST)
			try_again = true;
		// just clicked play story
		else
			++this->args->level;
	}
	else {
		this->args = (Args*)casted_args->nextargs;
		++this->args->level;
		this->args->points += casted_args->points;
	}
	
	switch (this->args->level) {
	case Args::FIRST:
		bg1 = new Sprite("img/transition/open_level1.png");
		break;
		
	case Args::SECOND:
		if (try_again)
			bg1 = new Sprite("img/transition/open_level2.png");
		else {
			saveRanking();
			bg1 = new Sprite("img/transition/close_level1.png");
			bg2 = new Sprite("img/transition/open_level2.png");
		}
		break;
		
	case Args::THIRD:
		if (try_again)
			bg1 = new Sprite("img/transition/open_level3.png");
		else {
			saveRanking();
			bg1 = new Sprite("img/transition/close_level2.png");
			bg2 = new Sprite("img/transition/open_level3.png");
		}
		break;
		
	case Args::LAST:
		saveRanking();
		bg1 = new Sprite("img/transition/close_level3.png");
		break;
		
	default:
		break;
	}
	
	stopwatch.start();
}

StateTransition::~StateTransition() {
	if (bg1)
		delete bg1;
	if (bg2)
		delete bg2;
	if (args->level == Args::LAST)
		delete args;
}

void StateTransition::update() {
	// throw credits screen: the game was beaten
	if ((args->level == Args::LAST) && (stopwatch.time() > SCREEN_DELAY))
		throw new Change("StateCredits", new ArgsBase());
	
	// don't throw state level yet...
	if (((stopwatch.time() <= SCREEN_DELAY) || (bg2)) &&
		((stopwatch.time() <= 2*SCREEN_DELAY) || (!bg2))) {
		return;
	}
	
	string level;
	
	// checking the level name string to load
	switch (args->level) {
	case Args::FIRST:
		level = "1";
		break;
		
	case Args::SECOND:
		level = "2";
		break;
		
	case Args::THIRD:
		level = "3";
		break;
		
	default:
		break;
	}
	
	throw new Change("StateLevel", new StateLevel::Args(level, "StateTransition", args));
}

void StateTransition::render() {
	if (stopwatch.time() <= SCREEN_DELAY)
		bg1->render();
	else if (bg2)
		bg2->render();
}

void StateTransition::saveRanking() {
	RankingData rnk_data(this->args->player_name, this->args->points);
	rnk_data.save("bin/nanotrip.rnk", 5);
}
