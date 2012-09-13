
/**

@file State.hpp
@author Matheus Pimenta

*/

#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <map>

#include "mod/observer.hpp"

/// Declares the game state ID.
#define GAMESTATE			\
private:					\
	static id_buf id_;		\
public:						\
	static id_type getid();	\
	id_type id() const;

/// Defines the game state ID and the constructor builder.
#define GAMESTATE_DEF(GameState)												\
State::id_buf GameState::id_ = true;											\
State::id_type GameState::getid() { return &id_; }								\
State::id_type GameState::id() const { return &id_; }							\
State* build(State::ArgsBase* args) { return new GameState(args); }				\
class Assigner { public: Assigner() { State::builders[#GameState] = build; } };	\
class Builder { static Assigner assigner; };									\
Assigner Builder::assigner;

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
	
	/// Throw this class to change the current game state.
	class Change {
	private:
		std::string state_;
		ArgsBase* args_;
	public:
		/// Assignmest constructor.
		Change(const std::string& state, ArgsBase* args = 0);
		
		/// Access method to the name of the new game state.
		const std::string& state() const;
		
		/// Access method to the arguments.
		ArgsBase* args() const;
	};
	
	/// Throw this class to stack up the current game state and load a new one.
	class StackUp : public Change {
	public:
		/// Assignment constructor.
		StackUp(const std::string& state, ArgsBase* args = 0);
	};
	
	/// Throw this class if you want to unstack a game state.
	class Unstack {};
	
	static std::map<std::string, State* (*)(ArgsBase*)> builders;
	
	/// List with all loaded game states.
	static std::list<State*> states;
protected:
	bool frozen_;
public:
	/// Empty constructor.
	State();
	
	/// Empty destructor.
	virtual ~State();
	
	static State* build(const std::string& name, ArgsBase* args = 0);
protected:
	/// Access method to the game state ID.
	virtual id_type id() const = 0;
public:
	/// The state tells if it's frozen, or not.
	bool frozen() const;
	
	virtual void input();
	virtual void update();
	virtual void render();
protected:
	virtual void handleQuit(const observer::Event& event, bool& stop);
};

#endif
