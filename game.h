#pragma once
#include "world.h"
#include "camera.h"
#include "game_object.h"
#include "graphics.h"
#include "audio.h"
#include "events.h"

enum class GameMode{Playing, GameOver};

using Events = std::map<std::string, Event*>;

class Game {
public:
    Game(std::string title, int width, int height);
    ~Game();
    void handle_event(SDL_Event* event);
    void input();
    void update();
    void render();

private:
    std::unique_ptr<GameObject> player;
    World* world = nullptr;
    Graphics graphics;
    Camera camera;
    Audio audio;

    // timing
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;

    // events
    Events events;
    void get_events();

    // level help
    void create_player();
    void update_enemy(GameObject& );
    int current_level{0};
    void load_level();

    // game state
    GameMode mode{GameMode::Playing};
};
