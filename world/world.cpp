#include "world.h"
#include <algorithm>
#include "game_object.h"
#include "fsm.h"
#include "states.h"
#include "keyboard_input.h"
#include "level.h"
#include "audio.h"
#include "events.h"

World::World(const Level& level, Audio& audio, GameObject* player, Events events)
    : tilemap{level.width, level.height}, audio{&audio}, player{player}, events{events} {
    load_level(level);
}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile{};
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x, y).blocking;
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

    touch_tiles(*player);
}

void World::load_level(const Level& level) {
    for (const auto& [pos, tile_id] : level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
    audio->load_sounds({});
}

void World::touch_tiles(GameObject& obj) {
    int x = std::floor(obj.physics.position.x);
    int y = std::floor(obj.physics.position.y);
    const std::vector<Vec<int>> displacements{{0,0}, {static_cast<int>(obj.size.x), 0}, {0, static_cast<int>(obj.size.y)}, {static_cast<int>(obj.size.x), static_cast<int>(obj.size.y)}};
    for (const auto& displacement : displacements) {
        Tile& tile = tilemap(x+displacement.x, y+displacement.y);
        if (!tile.event_name.empty()) {
            auto itr = events.find(tile.event_name);
            if (itr == events.end()) {
                throw std::runtime_error("Cannot find event" + tile.event_name);
            }
            itr->second->perform(*this, obj);
        }
    }
}