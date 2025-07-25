#ifndef BIRD_HPP
#define BIRD_HPP

#include "game_object.hpp"
#include <allegro5/allegro.h>

class Bird : public GameObject {
private:
    float velocity;
    float gravity;
    float jumpForce;

public:
    Bird(float x = 100, float y = 300);
    void update() override;
    void draw() const override;

    void jump();
    void setGravity(float g);
    void setJumpForce(float jf);
};

#endif
