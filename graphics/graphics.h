#pragma once

#include <string>
#include <SDL3/SDL.h>

class Color {
public:
    int red{255}, green{0}, blue{0}, alpha{255};
};

class Graphics {
public:
    Graphics(const std::string& title, int window_width, int window_height);
    void clear();
    void draw(const SDL_FRect& rect, const Color& color, bool filled=true);
    void update();
    int width, height;

private:
    std::string title;
    SDL_Window* window;
    SDL_Renderer* renderer;
};