#include "events.h"
#include "world.h"

void NextLevel::perform(World& world, GameObject& obj) {
    world.end_level = true; // allows player to move to next level
}
