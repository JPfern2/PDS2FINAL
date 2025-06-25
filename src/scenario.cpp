
#include "../include/scenario.hpp"
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <cstdlib>

Scenario::Scenario(float gravity, float speed)
    : gravity(gravity), obstacleSpeed(speed), score(0) {

    // Criar 3 pares de obstáculos (canos superior e inferior)
    for (int i = 0; i < 3; i++) {
        float x = 800 + i * 250; // SCREEN_WIDTH + spacing
        float topHeight = rand() % (600 - 250 - 100) + 50; // Altura aleatória
        float bottomY = topHeight + 250; // PIPE_GAP

        // Cano superior
        obstacles.push_back(new Obstacle(x, 0, 80, topHeight, obstacleSpeed));
        // Cano inferior  
        obstacles.push_back(new Obstacle(x, bottomY, 80, 600 - bottomY, obstacleSpeed));
    }
}

Scenario::~Scenario() {
    for (auto* obstacle : obstacles) {
        delete obstacle;
    }
}

bool Scenario::update(Bird& bird) {
    // Atualizar todos os obstáculos
    for (auto* obstacle : obstacles) {
        obstacle->update();
    }

    // Verificar colisões e pontuação
    for (size_t i = 0; i < obstacles.size(); i += 2) {
        Obstacle* topPipe = obstacles[i];
        Obstacle* bottomPipe = obstacles[i + 1];

        // Verificar colisão
        if (bird.collidesWith(*topPipe) || bird.collidesWith(*bottomPipe)) {
            return true; // Retorna true se houve colisão
        }

        // Verificar pontuação (quando o pássaro passa pelo meio do cano)
        if (!topPipe->hasScored() &&
            bird.getX() > topPipe->getX() + topPipe->getWidth()) {
            topPipe->markScored();
            bottomPipe->markScored();
            score++;
        }

        // Reciclar obstáculos que saíram da tela
        if (topPipe->isOffScreen()) {
            float newX = 800; // SCREEN_WIDTH
            float newTopHeight = rand() % (600 - 250 - 100) + 50;
            float newBottomY = newTopHeight + 250;

            // Reposicionar cano superior
            topPipe->move(newX - topPipe->getX());
            topPipe->setHeight(newTopHeight);

            // Reposicionar cano inferior
            bottomPipe->move(newX - bottomPipe->getX());
            bottomPipe->setY(newBottomY);
            bottomPipe->setHeight(600 - newBottomY);

            // Reset scored flag
            topPipe->resetScored();
            bottomPipe->resetScored();
        }
    }

    return false; // Retorna false se não houve colisão
}

void Scenario::draw() const {
    for (const auto* obstacle : obstacles) {
        obstacle->draw();
    }
}

void Scenario::reset() {
    score = 0;

    // Reposicionar todos os obstáculos
    for (size_t i = 0; i < obstacles.size(); i += 2) {
        float x = 800 + (i / 2) * 250;
        float topHeight = rand() % (600 - 250 - 100) + 50;
        float bottomY = topHeight + 250;

        // Reset cano superior
        obstacles[i]->setX(x);
        obstacles[i]->setY(0);
        obstacles[i]->setHeight(topHeight);

        // Reset cano inferior
        obstacles[i+1]->setX(x);
        obstacles[i+1]->setY(bottomY);
        obstacles[i+1]->setHeight(600 - bottomY);
    }
}

int Scenario::getScore() const {
    return score;
}

void Scenario::increaseScore() {
    score++;
}


void Scenario::setObstacleSpeed(float speed) {
    obstacleSpeed = speed;
    for (Obstacle* obs : obstacles) {
        obs->setSpeed(speed);
    }
}


