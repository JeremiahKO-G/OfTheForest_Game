#include "states.h"
#include "action.h"
#include "game_object.h"

// Helper Function
bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x + epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x - epsilon, obj.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}

// Standing
void Standing::on_enter(World&, GameObject& obj) { // color that shows when the object/player is standing
    obj.color = {255, 0, 0, 255};
    obj.physics.acceleration.x = 0;
}

Action* Standing::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::Jump && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    else if (action_type == ActionType::DodgeRight && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Dodge, world, obj);
        return new DodgeRight();
    }
    else if (action_type == ActionType::DodgeLeft && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Dodge, world, obj);
        return new DodgeLeft();
    }
    return nullptr; // if we had other options like walking, it would go here
}

// InAir
void InAir::on_enter(World& world, GameObject& obj) {
    elapsed = cooldown;
    obj.color = {0, 0, 255, 255};
}

void InAir::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

Action* InAir::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight;
    }
    if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft;
    }
    return nullptr;
}

void Running::on_enter(World&, GameObject& obj) {
    obj.color = {255, 255, 0, 255};
}

Action* Running::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::Jump && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::SprintRight) {
        obj.fsm->transition(Transition::Sprint, world, obj);
        return new SprintRight;
    }
    else if (action_type == ActionType::SprintLeft) {
        obj.fsm->transition(Transition::Sprint, world, obj);
        return new SprintLeft;
    }
    else if (action_type == ActionType::DodgeRight && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Dodge, world, obj);
        return new DodgeRight();
    }
    else if (action_type == ActionType::DodgeLeft && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Dodge, world, obj);
        return new DodgeLeft();
    }
    return nullptr;
}

void Sprinting::on_enter(World&, GameObject& obj) {
    obj.color = {255, 100, 100, 255};
}

Action* Sprinting::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::Jump && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move ,world, obj);
        return new MoveLeft();
    }
    else if (action_type == ActionType::DodgeRight && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Dodge, world, obj);
        return new DodgeRight();
    }
    else if (action_type == ActionType::DodgeLeft && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Dodge, world, obj);
        return new DodgeLeft();
    }
    return nullptr;
}

void Dodging::on_enter(World&, GameObject& obj) {
    elapsed = cooldown;
    obj.color = {100, 0, 255, 123};
}

Action* Dodging::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::Jump && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move ,world, obj);
        return new MoveLeft();
    }
    return nullptr;
}

void Dodging::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}
