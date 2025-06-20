#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

class GameObject {
protected:
    float x, y;
    float width, height;

public:
    GameObject(float x, float y, float w, float h);
    virtual ~GameObject() = default;

    virtual void update() = 0;
    virtual void draw() const = 0;
    virtual bool collidesWith(const GameObject& other) const;

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
};

#endif
