#ifndef ENTITY_H_
#define ENTITY_H_

#include "constants.h"

class Entity
{
public:
    Entity(Animation& a, float X, float Y, float Angle = 0.0f, float radius = 1) :
            life(true), animation(a),
            x(X), y(Y), angle(Angle),
            r(radius){}

    Entity(constants::EntityName name, Animation& a, float X,
           float Y, float Angle = 0.0f, float radius = 1) :
            life(true), animation(a),
            x(X), y(Y), angle(Angle),
            r(radius), name(name){}

    void set_parameters(Animation& a, float X, float Y, float Angle = 0.0f, float radius = 1)
    {
        animation = a;
        x = X; y = Y;
        angle = Angle;
        r = radius;
    }

    virtual void movement() {};

    void draw(sf::RenderWindow& app)
    {
        animation.sprite().setPosition(x, y);
        animation.sprite().setRotation(angle + 90);
        app.draw(animation.sprite());
    }

    virtual ~Entity() = default;

public:
    Animation animation;
    float x, y, dx, dy, r, angle;
    bool life;
    constants::EntityName name;

};

#endif //ENTITY_H_
