// include/scenario.hpp

#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include <vector>
#include "obstacle.hpp" // classe dos obstáculos (parte 2)
#include "bird.hpp"     // classe do pássaro (parte 2)

class Scenario {
protected:
    float gravity;
    float obstacleSpeed;
    int score;

    std::vector<Obstacle*> obstacles;

public:
    Scenario(float gravity = 0.5, float speed = 2.0);

    virtual ~Scenario();

    virtual void update(Bird& bird); // atualiza obstáculos e colisões
    virtual void draw() const;       // desenha cenário e obstáculos

    void reset();
    int getScore() const;

    void increaseScore();
};

#endif
