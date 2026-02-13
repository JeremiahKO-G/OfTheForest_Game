// game_object.h
#pragma once
#include <utility>
#include "vec.h"
#include "graphics.h"
#include "physics.h"

class World;

class GameObject {
public:
    GameObject(const Vec<float>& position, const Vec<float>& size, World& world);
    ~GameObject();

    void input(World& world);
    void update(World& world, float dt);

    std::pair<Vec<float>, Color> get_sprite() const;

    // Player data
    Vec<float> position;
    Vec<float> size;
    Vec<float> velocity, acceleration;
    Physics physics;
};