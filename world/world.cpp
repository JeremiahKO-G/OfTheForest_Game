#include "world.h"
#include <algorithm>
#include "game_object.h"
// #include "vec.h"


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
    player = std::make_unique<GameObject>(Vec<float>{10, 5}, Vec<float>{1, 1}, *this);
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
        float dx = floor(position.x) - position.x;
        float dy = ceil(position.y) - position.y;
        if (dx > dy) {
            position.x = floor(position.x);
            velocity.x = 0;
        }
        else {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
    }
    else if (collides({position.x, position.y+size.y})) {
        float dx = ceil(position.x) - position.x;
        float dy = ceil(position.y) - position.y;
        if (dx < dy) {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
        else {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
    }
    else if (collides({position.x+size.x, position.y+size.y})) {
        float dx = floor(position.x) - position.x;
        float dy = floor(position.y) - position.y;
        if (dx > dy) {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
        else {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
    }
}

void World::update(float dt) {
    float terminal_velocity = player->physics.terminal_velocity;
    // Vec<float> velocity = player->physics.velocity;
    // Vec<float> acceleration = player->physics.acceleration;
    auto velocity = player->velocity;
    auto position = player->position;

    velocity += 0.5f * player->acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * player->acceleration * dt;
    velocity.x *= player->physics.damping;
    // player->acceleration.y = player->physics.gravity;

    velocity.x = std::clamp(velocity.x, -terminal_velocity, terminal_velocity);
    velocity.y = std::clamp(velocity.y, -terminal_velocity, terminal_velocity);

    // check for x collisions
    Vec<float> future_position{position.x, player->position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);

    player->position.x = future_position.x;
    player->velocity.x = future_velocity.x;

    // now y direction after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);

    // update the player position and velocity
    player->position.y = future_position.y;
    player->velocity.y = future_velocity.y;

    // check for x collisions
    // Vec<float> future{player->position.x, player->position.y};
    // if (collides(future)) {
    //     velocity.x = 0;
    //     acceleration.x = 0;
    // }
    // else {
    //     velocity = velocity.x;
    //     player->physics.position = player->physics.position.x;
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


