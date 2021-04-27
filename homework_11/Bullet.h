#ifndef BULLET_H_
#define BULLET_H_

#include "constants.h"

class Bullet : public Entity
{
public:
    Bullet(Animation& a, float X, float Y, float Angle = 0.0f, float radius = 1) :
        Entity(constants::EntityName::Bullet,a, X, Y, Angle, radius){}
private:
    void  movement() override
    {
        dx = std::cos(angle * constants::kDegToRad) * 6;
        dy = std::sin(angle * constants::kDegToRad) * 6;
        x += dx;
        y += dy;

        if (x > constants::kWidth || x < 0 || y > constants::kHeight || y < 0) life = false;
    }

};

#endif //BULLET_H_
