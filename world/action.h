#pragma once
#include "game_object.h"

enum class ActionType {
    None,
    Jump,
    MoveLeft,
    MoveRight,
    SprintLeft,
    SprintRight,
    DodgeLeft,
    DodgeRight
};

class World;
class GameObject;

class Action {
public:
    virtual ~Action() = default;
    virtual void perform(World& world, GameObject& obj) = 0;
};

class Jump : public Action {
    void perform(World&, GameObject& obj) override;
};

class MoveRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class MoveLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class SprintLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class SprintRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class DodgeLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class DodgeRight : public Action {
    void perform(World&, GameObject& obj) override;
};