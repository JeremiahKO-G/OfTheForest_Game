#pragma once
#include "input.h"

class AiInput : public Input {
    void get_input() override;
    void handle_input(World& world, GameObject& obj) override;
};
