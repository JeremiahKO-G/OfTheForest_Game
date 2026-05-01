#include "action.h"

#include "audio.h"
#include "game_object.h"
#include "world.h"

void Jump::perform(World& world, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
    world.audio->play_sounds("jump");
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
    obj.physics.velocity.x = -obj.physics.dodge_velocity;
}

void DodgeRight::perform(World&, GameObject& obj) {
    obj.physics.velocity.x = obj.physics.dodge_velocity;
}
