#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include <vector>
#include <allegro5/allegro.h>
#include "obstacle.hpp"
#include "bird.hpp"

class Scenario {
protected:
    float gravity;
    float obstacleSpeed;
    int score;

    std::vector<Obstacle*> obstacles;

public:
    Scenario(float gravity = 0.5, float speed = 2.0);

    virtual ~Scenario();

    virtual bool update(Bird& bird); // Retorna true se houve colisão e atualiza obstáculos e colisões
    virtual void draw() const;       // desenha cenário e obstáculos

    void reset();
    int getScore() const;

    void increaseScore();
};

#endif
