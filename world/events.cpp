#include "events.h"
#include "world.h"

void NextLevel::perform(World& world, GameObject& obj) {
    if (&obj != world.player) return; // avoids enemies starting the next level tile
    world.end_level = true; // allows player to move to next level
}
