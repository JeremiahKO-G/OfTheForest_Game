#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include <vector>

#include "events.h"
#include "quadtree.h"
#include "tilemap.h"
#include "vec.h"
#include "projectile.h"

class Event;
class Level;
class GameObject;
class Audio;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, Events events);
    ~World();

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    void update(float dt);
    void update_object(GameObject* obj, double dt);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);
    void load_level(const Level& level);

    Tilemap tilemap;
    bool end_level{false};
    bool end_game{false};

    void build_quadtree();

    GameObject* player;
    Audio* audio;
    std::vector<GameObject*> game_objects;
    std::vector<Projectile*> projectiles;
    std::map<std::string, Event*> events;
    std::vector<Background> backgrounds;

    void touch_tiles(GameObject& obj);

    Quadtree quadtree;

    // items
    std::map<std::string, std::function<GameObject*()>> available_items;
};