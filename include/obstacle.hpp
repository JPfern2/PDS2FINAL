#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "game_object.hpp"

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
};

#endif