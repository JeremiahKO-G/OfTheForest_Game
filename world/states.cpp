#include "states.h"
#include "action.h"
#include "game_object.h"
#include "random.h"

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
    obj.set_sprite("idle");
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
    else if (action_type == ActionType::AttackAll) {
        obj.fsm->transition(Transition::AttackAll, world, obj);
    }
    return nullptr; // if we had other options like walking, it would go here
}

// InAir
void InAir::on_enter(World& world, GameObject& obj) {
    elapsed = cooldown;
    obj.set_sprite("jumping");
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

// Patrolling
void Patrolling::on_enter(World& world, GameObject& obj) {
    // set cooldown to a random amount of time 3-10 seconds
    elapsed = 0;
    cooldown = randint(3,10);
    Running::on_enter(world, obj);
}

Action* Patrolling::input(World& world, GameObject& obj, ActionType action_type) {
    if (elapsed >= cooldown) {
        return Running::input(world, obj, ActionType::None);
    }
    return Running::input(world, obj, action_type);
}

void Patrolling::update(World&, GameObject&, double dt) {
    elapsed += dt;
}

void Running::on_enter(World&, GameObject& obj) {
    obj.color = {255, 255, 0, 255};
    obj.set_sprite("walking");
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
    obj.set_sprite("running");
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
    obj.set_sprite("dashing");
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

// AttackAll
void AttackAllEnemies::on_enter(World& world, GameObject& obj) {
    obj.color = {255, 100, 0, 255};
    for (auto& enemy : world.game_objects) {
        if (enemy == world.player) continue;
        enemy->take_damage(obj.damage);
    }
    elapsed = 0;
}

void AttackAllEnemies::update(World& world, GameObject& obj, double dt) {
    elapsed += dt;
    if (elapsed >= cooldown) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}
