#include "game_object.h"

#include <iostream>

#include "physics.h"
#include "SDL3/SDL_scancode.h"
#include "world.h"
#include "fsm.h"
#include "action.h"
#include "input.h"
#include "quadtree.h"

GameObject::GameObject(std::string name, FSM* fsm, Input* input, Color color)
    : obj_name{name}, fsm{fsm}, input{input}, color{color}{
}

GameObject::~GameObject() {
    delete fsm;
    delete input;
}

void GameObject::update(World& world, float dt) {
    if (fsm != nullptr) {
        fsm->current_state->update(world, *this, dt);
    }
    sprites[sprite_name].update(dt);
    sprites[sprite_name].flip(physics.acceleration.x < 0);
    set_sprite(sprite_name);
    if (i_frame_time > 0.0) i_frame_time = -dt;
}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}

void GameObject::set_sprite(const std::string& next_sprite) {
    // if the next sprite is a new sprite, reset current sprite
    if (next_sprite != sprite_name) {
        sprites[sprite_name].reset();

        auto itr = sprites.find(next_sprite);
        if (itr != sprites.end()) {
            sprite_name = next_sprite;
        }
        else {
            sprite_name = "idle"; // assume every GameObject has an idle sprite
        }
    }

    sprite = sprites[sprite_name].get_sprite();
}

AABB GameObject::get_bounding_box() {
    Vec<float> half_size = {size.x / 2.0f, size.y / 2.0f};
    Vec<float> center = {physics.position.x + half_size.x, physics.position.y + half_size.y};
    AABB bounding_box{center, half_size};
    return bounding_box;
}

void GameObject::take_damage(int attack_dmg) {
    if (i_frame_time > 0.0) return;
    health -= attack_dmg;
    i_frame_time = 3;
    if (health <= 0) {
        is_alive = false;
    }
}

bool GameObject::flash_sprite() const {
    if (i_frame_time <= 0.0) {
        return false;
    }

    // alternate overlay on/off every 80 ms
    return ((SDL_GetTicks() / 80) % 2) == 0;
}
