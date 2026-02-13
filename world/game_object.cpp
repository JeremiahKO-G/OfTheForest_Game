#include "game_object.h"

#include "physics.h"
#include "SDL3/SDL_scancode.h"
#include "world.h"

GameObject::GameObject(const Vec<float>& position, const Vec<float>& size, World& world)
    : position{position}, size{size} {
    acceleration = physics.gravity;
}

GameObject::~GameObject() {}

void GameObject::input(World& world) {
    const bool *key_states = SDL_GetKeyboardState(NULL);
    acceleration.x = 0;
    acceleration.y = physics.gravity;
    // velocity.x = velocity.y = 0;
    // if (key_states[SDL_SCANCODE_W]) {
    //     velocity.y += -8;
    // }
    // if (key_states[SDL_SCANCODE_S]) {
    //     velocity.y += 8;
    // }
    if (key_states[SDL_SCANCODE_A]) {
        // velocity.x += -8;
        acceleration.x += -physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_D]) {
        // velocity.x += 8;
        acceleration.x += physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_SPACE]) {
        velocity.y = physics.jump_velocity;
    }
}

void GameObject::update(World& world, float dt) {
    // velocity += 0.5f * acceleration * dt;
    // position += velocity * dt;
    // velocity += 0.5f * acceleration * dt;
    // velocity.x *= physics.damping;
    //
    // velocity.x = std::clamp(velocity.x, -physics.terminal_velocity, physics.terminal_velocity);
    // velocity.y = std::clamp(velocity.y, -physics.terminal_velocity, physics.terminal_velocity);
    //
    // // check for x collisions
    // Vec<float> future{position.x, position.y};
    // if (world.collides(future)) {
    //     velocity.x = 0;
    //     acceleration.x = 0;
    // }
    // else {
    //     velocity = velocity.x;
    //     position = position.x;
    //     acceleration = acceleration.x;
    // }
    // // y collisions
    // future.x = position.x;
    // future.y = position.y;
    // if (world.collides(future)) {
    //     velocity.y = 0;
    //     acceleration.y = physics.gravity;
    // }
    //  else {
    //      position = position.y;
    //      velocity = velocity.y;
    //      acceleration = acceleration.y;
    //  }
}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {position, {255, 0, 255, 255}};
}


