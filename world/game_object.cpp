#include "game_object.h"

#include "physics.h"
#include "SDL3/SDL_scancode.h"
#include "world.h"
#include "fsm.h"
#include "action.h"
#include "input.h"

GameObject::GameObject(const Vec<float>& position, const Vec<float>& size, World& world, FSM* fsm, Input* input, Color color)
    : physics{position, {0,0}, {0,0}}, size{size}, input{input}, fsm{fsm}, color{color}{
    physics.acceleration.y = physics.gravity;
    fsm->current_state->on_enter(world, *this);
}

GameObject::~GameObject() {
    delete fsm;
    delete input;
}

void GameObject::update(World& world, float dt) {
    fsm->current_state->update(world, *this, dt);
}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}