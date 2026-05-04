#include "game.h"

#include "ai_input.h"
#include "asset_manager.h"
#include "fsm.h"
#include "input.h"
#include "keyboard_input.h"
#include "level.h"
#include "states.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height}, camera{graphics, 64}, dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {

    // load events
    get_events();

    // Give player its assets then put it in the correct state
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);

    // load first level
    load_level();
}

Game::~Game() {
    delete world;
    for (auto [_, event]: events) {
        delete event;
    }
}

void Game::handle_event(SDL_Event* event) {
    switch (mode) {
        case GameMode::Playing:
            auto action = player->input->collect_discrete_event(event);
            if (action) {
                action->perform(*world, *player);
                delete action;
            }
            break;
    }
}

void Game::input() {
    switch (mode) {
        case GameMode::Playing:
            player->input->get_input();
            camera.handle_input();
            break;
    }
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        switch (mode) {
            case GameMode::Playing:
                for (auto obj : world->game_objects) {
                    obj->input->handle_input(*world, *obj);
                }
                world->update(dt);
                // put the camera slightly ahead of the player
                float L = length(player->physics.velocity);
                Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
                camera.update(player->physics.position + displacement, dt);

                // check for level end
                if (world->end_level) {
                    load_level();
                }
                if (world->end_game) {
                    mode = GameMode::GameOver;
                }
                if (final_level) {
                    mode = GameMode::Win;
                }
                break;
        }
        lag -= dt;
    }
}

void Game::render() {
    // clear screen
    graphics.clear();

    // draw the backgrounds
    camera.render(world->backgrounds);

    // draw the world
    camera.render(world->tilemap);

    // draw the player
    camera.render(*player);

    for (auto& obj : world->game_objects) {
        camera.render(*obj);
    }

    camera.render(world->fg_tilemap);

    if (mode == GameMode::GameOver) {
        camera.render_game_over();
    }
    if (mode == GameMode::Win) {
        camera.render_win();
    }

    // update
    graphics.update();
}

void Game::get_events() {
    events["next_level"] = new NextLevel;
}

void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    if (current_level == 4) {
        final_level = true;
    }
    else {
        Level level{level_name};
        AssetManager::get_level_details(graphics, level);

        // create the world
        delete world;
        world = new World(level, audio, player.get(), events);

        // assets for objs
        for (auto obj : world->game_objects) {
            if (obj == world->player) continue;
            update_enemy(*obj);
            AssetManager::get_game_object_details(obj->obj_name + "-enemy", graphics, *obj, true);
        }

        player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
        player->fsm->current_state->on_enter(*world, *player);
        camera.set_location(player->physics.position);
        audio.play_sounds("background", true);
    }
}


void Game::create_player() {
    // create FSM
    Transitions transitions ={
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::Dodge}, StateType::Dodging},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Standing, Transition::AttackAll}, StateType::AttackAll},
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::InAir, Transition::Move}, StateType::Running},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running , Transition::Jump}, StateType::InAir},
        {{StateType::Running, Transition::Sprint}, StateType::Sprinting},
        {{StateType::Running, Transition::Dodge}, StateType::Dodging},
        {{StateType::Sprinting, Transition::Stop}, StateType::Standing},
        {{StateType::Sprinting, Transition::Move}, StateType::Running},
        {{StateType::Sprinting, Transition::Jump}, StateType::InAir},
        {{StateType::Sprinting, Transition::Dodge}, StateType::Dodging},
        {{StateType::Dodging, Transition::Stop}, StateType::Standing},
        {{StateType::Dodging, Transition::Move}, StateType::Running},
        {{StateType::Dodging, Transition::Sprint}, StateType::Sprinting},
        {{StateType::AttackAll, Transition::Stop}, StateType::Standing}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprinting, new Sprinting()},
        {StateType::Dodging, new Dodging},
        {StateType::AttackAll, new AttackAllEnemies()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    // player input
    KeyboardInput* input = new KeyboardInput();

    player = std::make_unique<GameObject>("player", fsm, input, Color{255, 0, 0, 255});
}

void Game::update_enemy(GameObject& obj) {
    Transitions transitions;
    States states;

    if (obj.obj_name == "stump" || obj.obj_name == "bush") {
        transitions = {
            {{StateType::Standing, Transition::Move}, StateType::Patrolling},
            {{StateType::Patrolling, Transition::Stop}, StateType::Standing}
        };
        states = {
            {StateType::Standing, new Standing()},
            {StateType::Patrolling, new Patrolling()}
        };
    }
    else {
        // throw an error
    }

    FSM* fsm = new FSM{transitions, states, StateType::Patrolling};
    obj.fsm = fsm;

    Input* input = new AiInput{};
    input->next_action_type = ActionType::MoveRight;
    obj.input = input;
}

