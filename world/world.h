#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include <vector>

#include "events.h"
#include "tilemap.h"
#include "vec.h"

class Event;
class Level;
class GameObject;
class Audio;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, Events events);

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    void update(float dt);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);
    void load_level(const Level& level);

    Tilemap tilemap;
    bool end_level{false};

private:
    GameObject* player;
    Audio* audio;
    std::map<std::string, Event*> events;

    void touch_tiles(GameObject& obj);
};