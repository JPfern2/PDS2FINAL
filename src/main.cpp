// src/scenario.cpp

#include "scenario.hpp"
#include <allegro5/allegro_primitives.h>

Scenario::Scenario(float g, float speed)
    : gravity(g), obstacleSpeed(speed), score(0) {}

Scenario::~Scenario() {
    for (auto ob : obstacles) delete ob;
}

void Scenario::update(Bird& bird) {
    // mover obstáculos, verificar colisões, atualizar score
    for (auto ob : obstacles) {
        ob->move(-obstacleSpeed);
        if (ob->collidesWith(bird)) {
            // talvez setar um estado de "fim de jogo"
        }
    }
    // lógica para gerar novos obstáculos
}

void Scenario::draw() const {
    // fundo
    al_clear_to_color(al_map_rgb(135, 206, 235)); // azul claro
    for (auto ob : obstacles) {
        ob->draw();
    }
}

void Scenario::reset() {
    score = 0;
    for (auto ob : obstacles) delete ob;
    obstacles.clear();
}

int Scenario::getScore() const {
    return score;
}

void Scenario::increaseScore() {
    score++;
}
