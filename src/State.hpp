
/**

@file State.hpp
@author Matheus Pimenta

*/

#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <map>

#include "common.hpp"
#include "observer.hpp"

#include "Text.hpp"
#include "Interaction.hpp"

/// Declares the game state ID.
#define GAMESTATE				\
public:							\
	class Assigner {			\
	public:						\
		Assigner();				\
	};							\
private:						\
	static Assigner assigner;	\
								\
	static id_buf id_;			\
public:							\
	static id_type getid();		\
	id_type id() const;

/// Defines the game state ID and the constructor builder.
#define GAMESTATE_DEF(GameState)												\
GameState::Assigner GameState::assigner;										\
State::id_buf GameState::id_ = true;											\
State* build##GameState(State::ArgsBase* args) { return new GameState(args); }	\
GameState::Assigner::Assigner() {												\
	State::builders[#GameState] = &build##GameState;							\
}																				\
State::id_type GameState::getid() { return &id_; }								\
State::id_type GameState::id() const { return &id_; }

/// Base class for a game state.
class State {
public:
	typedef const bool id_buf;
	typedef id_buf* id_type;
	
	/// Class for passing arguments between game states.
	class ArgsBase {
	public:
		/// Empty constructor.
		ArgsBase();
		
		/// Empty destructor.
		virtual ~ArgsBase();
	};
	
	/// Throw (POINTER!) this class to change the current game state.
	class Change : public common::mexception {
	private:
		std::string state_;
		ArgsBase* args_;
	public:
		/// Assignmest constructor.
		Change(const std::string& state, ArgsBase* args = 0);
		
		/// Empty destructor.
		~Change() throw();
		
		/// Access method to the name of the new game state.
		const std::string& state() const;
		
		/// Access method to the arguments.
		ArgsBase* args() const;
	};
	
	/// Throw (POINTER!) this class to stack up the current game state and load a new one.
	class StackUp : public common::mexception {
	private:
		std::string state_;
		ArgsBase* args_;
	public:
		/// Assignmest constructor.
		StackUp(const std::string& state, ArgsBase* args = 0);
		
		/// Empty destructor.
		~StackUp() throw();
		
		/// Access method to the name of the new game state.
		const std::string& state() const;
		
		/// Access method to the arguments.
		ArgsBase* args() const;
	};
	
	/// Throw (POINTER!) this class if you want to unstack a game state.
	class Unstack : public common::mexception {
	private:
		ArgsBase* args_;
	public:
		/// Assignmest constructor.
		Unstack(ArgsBase* args = 0);
		
		/// Empty destructor.
		~Unstack() throw();
		
		/// Access method to the arguments.
		ArgsBase* args() const;
	};
	
	static std::map<std::string, State* (*)(ArgsBase*)> builders;
	
	/// List with all loaded game states.
	static std::list<State*> states;
protected:
	bool frozen_;
	Sprite* bg;
	std::list<Sprite*> sprites;
	std::list<Text*> texts;
	std::list<GameObject*> game_objects;
	std::list<Interaction*> interactions;
public:
	/// Empty constructor.
	State();
	
	/// Empty destructor.
	virtual ~State();
	
	static State* build(const std::string& name, ArgsBase* args = 0);
	
	virtual void handleUnstack(ArgsBase* args);
protected:
	/// Access method to the game state ID.
	virtual id_type id() const = 0;
public:
	/// The state tells if it's frozen, or not.
	bool frozen() const;
	
	void externUpdate();
	void externRender();
protected:
	virtual void update();
	virtual void render();
	
	virtual void handleQuit(const observer::Event& event, bool& stop);
};

#endif
