// game_object.h
#pragma once
#include <utility>
#include <map>
#include "vec.h"
#include "graphics.h"
#include "physics.h"
#include "animated_sprite.h"

class World;
class FSM;
class Input;
class AABB;

using Sprites = std::map<std::string, AnimatedSprite>;

class GameObject {
public:
    GameObject(std::string name, FSM* fsm, Input* input, Color color);
    ~GameObject();

    virtual void update(World& world, float dt);

    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);

    AABB get_bounding_box();

    void take_damage(int attack_dmg);
    bool flash_sprite() const;

    // Player data
    std::string obj_name;
    Vec<float> size;
    Physics physics;
    FSM* fsm;
    Input* input;
    Color color;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;

    // combat stuff
    int health;
    int max_health;
    int damage;
    bool is_alive = true;

    double i_frame_time{0.0};
};