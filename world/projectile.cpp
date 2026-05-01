#include "projectile.h"

Projectile::Projectile(std::string name, FSM* fsm, Input* input, double lifetime)
    : GameObject{name, fsm, input, Color{200, 100, 100, 180}}, lifetime{lifetime} {}

void Projectile::update(World& world, float dt) {
    GameObject::update(world, dt);
    elapsed += dt;
}

