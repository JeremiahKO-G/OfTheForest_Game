#include "world.h"
#include <algorithm>
#include "game_object.h"
#include "fsm.h"
#include "states.h"
#include "keyboard_input.h"

World::World(int width, int height)
    : tilemap{width, height} {}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile::Platform;
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x, y) == Tile::Platform;
}

GameObject* World::create_player() {
    // Create FSM
    Transitions transitions ={
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::Dodge}, StateType::Dodging},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::InAir, Transition::Move}, StateType::Running},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running , Transition::Jump}, StateType::InAir},
        {{StateType::Running, Transition::Sprint}, StateType::Sprinting},
        {{StateType::Running, Transition::Dodge}, StateType::Dodging},
        {{StateType::Sprinting, Transition::Stop}, StateType::Standing},
        {{StateType::Sprinting, Transition::Move}, StateType::Running},
        {{StateType::Sprinting, Transition::Jump}, StateType::InAir},
        {{StateType::Sprinting, Transition::Dodge}, StateType::Dodging},
        {{StateType::Dodging, Transition::Stop}, StateType::Standing},
        {{StateType::Dodging, Transition::Move}, StateType::Running},
        {{StateType::Dodging, Transition::Sprint}, StateType::Sprinting}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprinting, new Sprinting()},
        {StateType::Dodging, new Dodging}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    KeyboardInput* input = new KeyboardInput();

    player = std::make_unique<GameObject>(Vec<float>{10, 5}, Vec<float>{1, 1}, *this, fsm, input, Color{255, 0, 0, 255});
    return player.get();
}

void World::move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity) {
    // test for collisions on the bottom or top first
    if (collides(position) && collides({position.x+size.x, position.y})) {
        position.y = ceil(position.y);
        velocity.y = 0;
    }
    else if (collides({position.x, position.y + size.y}) && collides({position.x + size.x, position.y + size.y})) {
        position.y = floor(position.y);
        velocity.y = 0;
    }

    // then test for collisions on the left and right sides
    if (collides(position) && collides({position.x, position.y + size.y})) {
        position.x = ceil(position.x);
        velocity.x = 0;
    }
    else if (collides({position.x + size.x, position.y}) && collides({position.x + size.x, position.y+size.y})) {
        position.x = floor(position.x);
        velocity.x = 0;
    }

    // now test each corner
    if (collides(position)) {
        float dx = ceil(position.x) - position.x;
        float dy = ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x+size.x, position.y})) {
        float dx = position.x - floor(position.x);
        float dy = ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = floor(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x, position.y+size.y})) {
        float dx = ceil(position.x) - position.x;
        float dy = position.y - floor(position.y);
        if (dx > dy) {
            position.y = floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x+size.x, position.y+size.y})) {
        float dx = position.x - floor(position.x);
        float dy = position.y - floor(position.y);
        if (dx > dy) {
            position.y = floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = floor(position.x);
            velocity.x = 0;
        }
    }
}

void World::update(float dt) {
    float terminal_velocity = player->physics.terminal_velocity;
    // Vec<float> velocity = player->physics.velocity;
    // Vec<float> acceleration = player->physics.acceleration;
    auto velocity = player->physics.velocity;
    auto position = player->physics.position;

    velocity += 0.5f * player->physics.acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * player->physics.acceleration * dt;
    velocity.x *= player->physics.damping;
    // player->acceleration.y = player->physics.gravity;

    velocity.x = std::clamp(velocity.x, -terminal_velocity, terminal_velocity);
    velocity.y = std::clamp(velocity.y, -terminal_velocity, terminal_velocity);

    // check for x collisions
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);

    // now y direction after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);

    // update the player position and velocity
    player->physics.position = future_position;
    player->physics.velocity = future_velocity;
}


