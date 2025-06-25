#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "game_object.hpp"
#include <allegro5/allegro.h>

class Obstacle : public GameObject {
private:
    float speed;
    bool scored;

public:
    Obstacle(float x, float y, float w, float h, float speed = 2.0);

    void update() override;
    void draw() const override;

    void move(float dx);
    bool isOffScreen() const;
    bool hasScored() const;
    void markScored();
    void resetScored();
    void setSpeed(float s);
};

#endif