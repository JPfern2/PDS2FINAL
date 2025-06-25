
#include "../include/obstacle.hpp"
#include <allegro5/allegro_primitives.h>

Obstacle::Obstacle(float x, float y, float w, float h, float speed)
    : GameObject(x, y, w, h), speed(speed), scored(false) {
}

void Obstacle::update() {
    x -= speed;
}

void Obstacle::draw() const {
    al_draw_filled_rectangle(x, y, x + width, y + height,
        al_map_rgb(0, 128, 0));
}

void Obstacle::move(float dx) {
    x += dx;
}

bool Obstacle::isOffScreen() const {
    return x + width < 0;
}

bool Obstacle::hasScored() const {
    return scored;
}

void Obstacle::markScored() {
    scored = true;
}

void Obstacle::resetScored() {
    scored = false;
}


void Obstacle::setSpeed(float s) {
    speed = s;
}


