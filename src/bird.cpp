#include "../include/bird.hpp"  
#include <allegro5/allegro_primitives.h>  

Bird::Bird(float x, float y)  
    : GameObject(x, y, 20, 20), velocity(0), gravity(0.5), jumpForce(-8) {  
}  

void Bird::update() {  
    velocity += gravity;  
    y += velocity;  
}  

void Bird::draw() const {  
    al_draw_filled_circle(x + width / 2, y + height / 2, width / 2,  
        al_map_rgb(255, 255, 0));  
}  

void Bird::jump() {  
    velocity = jumpForce;  
}  

void Bird::setGravity(float g) {  
    gravity = g;  
}
