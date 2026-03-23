#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL.h>

#include "sprite.h"
#include "vec.h"

class Color {
public:
    int red{255}, green{0}, blue{0}, alpha{255};
};

class Graphics {
public:
    Graphics(std::string title, int width, int height);
    void clear();
    void update();

    void draw(const SDL_FRect& rect, const Color& color, bool filled=true);
    void draw_sprite(const Vec<float>& pixel, const Sprite& sprite);

    int get_texture_id(const std::string& image_filename);
    void set_title(const std::string& title);

    const int width, height;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> textures;
    std::unordered_map<std::string, int> texture_ids;
};