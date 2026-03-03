#include "fsm.h"

FSM::FSM(Transitions transitions, States states, StateType start)
    : transitions{transitions}, states{states}, current_state_type{start}, current_state{states[current_state_type]}{}
// what are we doing with current_states{states[current_state_type]} what's the [] doing?
FSM::~FSM() {
    for (auto [_, state] : states) {
        delete state;
    }
}

void FSM::transition(Transition t, World& world, GameObject& obj) {
    auto itr = transitions.find({current_state_type, t});
    if (itr != transitions.end()) {
        current_state->on_exit(world, obj); // why on_exit first?

        current_state_type = itr->second; // what is second? Is it returning the second value of the pair in map?
        current_state = states[current_state_type];

        current_state->on_enter(world, obj);
    }
}



