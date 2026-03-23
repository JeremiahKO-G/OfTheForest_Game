#include "action.h"
#include "game_object.h"
#include "world.h"

void Jump::perform(World&, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
}

void MoveRight::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
}

void MoveLeft::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
}

void SprintRight::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = 5*obj.physics.walk_acceleration;
}

void SprintLeft::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = -5*obj.physics.walk_acceleration;
}

void DodgeLeft::perform(World&, GameObject& obj) {
    obj.physics.velocity.x = -obj.physics.jump_velocity;
}

void DodgeRight::perform(World&, GameObject& obj) {
    obj.physics.velocity.x = obj.physics.jump_velocity;
}
