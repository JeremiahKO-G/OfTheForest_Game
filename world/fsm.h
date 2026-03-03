#pragma once
#include <map>

class World;
class GameObject;
class Action;
enum class ActionType;
class State;

enum class StateType {Standing, InAir, Running, Sprinting, Dodging};
enum class Transition {Jump, Stop, Move, Sprint, Dodge}; // helps us move from one state to the other

using Transitions = std::map<std::pair<StateType, Transition>, StateType>;
using States = std::map<StateType, State*>; // pointer to the class

class FSM {
public:
    FSM(Transitions transitions, States states, StateType start); // you start in a state
    virtual ~FSM();

    void transition(Transition t, World& world, GameObject& obj);

    // FSM data
    Transitions transitions; // transitions map
    States states; // states map
    StateType current_state_type; // the state we're in
    State* current_state; // pointer to the class
};

class State {
public:
    virtual ~State() = default;

    virtual void on_enter(World&, GameObject&) {}
    virtual void on_exit(World&, GameObject&) {}

    virtual Action* input(World&, GameObject&, ActionType) {return nullptr;} // if you get a return, do something with it, but if not don't do anything (nullptr)
    virtual void update(World&, GameObject&, [[maybe_unused]]double dt){}
};