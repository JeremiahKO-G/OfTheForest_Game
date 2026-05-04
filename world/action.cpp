#include "action.h"

#include "audio.h"
#include "game_object.h"
#include "world.h"

void Jump::perform(World& world, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
    world.audio->play_sounds("jump");
}

void MoveRight::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
    // world.audio->play_sounds("walk");
}

void MoveLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
    // world.audio->play_sounds("walk");
}

void SprintRight::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = 5*obj.physics.walk_acceleration;
    // world.audio->play_sounds("walk");
}

void SprintLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = -5*obj.physics.walk_acceleration;
    // world.audio->play_sounds("walk"); // need a function to tell code when to stop playing the sound
}

void DodgeLeft::perform(World& world, GameObject& obj) {
    obj.physics.velocity.x = -obj.physics.dodge_velocity;
    world.audio->play_sounds("dash");
}

void DodgeRight::perform(World& world, GameObject& obj) {
    obj.physics.velocity.x = obj.physics.dodge_velocity;
    world.audio->play_sounds("dash");
}
