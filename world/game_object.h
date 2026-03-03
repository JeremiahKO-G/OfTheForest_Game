// game_object.h
#pragma once
#include <utility>
#include "vec.h"
#include "graphics.h"
#include "physics.h"

class World;
class FSM;
class Input;

class GameObject {
public:
    GameObject(const Vec<float>& position, const Vec<float>& size, World& world, FSM* fsm, Input* input, Color color);
    ~GameObject();

    void update(World& world, float dt);

    std::pair<Vec<float>, Color> get_sprite() const;

    // Player data
    Vec<float> size;
    Physics physics;
    FSM* fsm;
    Input* input;
    Color color;
};