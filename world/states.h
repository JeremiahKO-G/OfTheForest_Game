#pragma once
#include "fsm.h"
#include "world.h"

class Standing: public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
};

class InAir: public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override; // update because of physics
    Action* input(World&, GameObject&, ActionType) override;

    const double cooldown = 0.1;
    double elapsed = 0;
};

class Running: public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override; // action because of input
};

class Sprinting: public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
};

class Dodging: public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override; // update because of physics
    Action* input(World&, GameObject&, ActionType ) override;

    const double cooldown = 120;
    double elapsed = 0;
};