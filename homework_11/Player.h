#ifndef PLAYER_H_
#define PLAYER_H_

#include "constants.h"
#include "Entity.h"

class Player : public Entity
{
public:
    Player(Animation& a, float X, float Y, float Angle = 0.0f, float radius = 1) :
            Entity(constants::EntityName::Player, a, X, Y, Angle, radius),
            lives(3), score(0){}

    bool is_alive() const { return (lives > 0); }


    void score_increase() {
        score++;
    }

private:
    void movement() override
    {
        if (thrust)
        {
            dx += std::cos(angle * constants::kDegToRad) * 0.2f;
            dy += std::sin(angle * constants::kDegToRad) * 0.2f;
        }
        else
        {
            dx *= speed_mult;
            dy *= speed_mult;
        }

        float speed = std::sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed)
        {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        x += dx;
        y += dy;

        if (x > constants::kWidth) x = 0; if (x < 0) x = constants::kWidth;
        if (y > constants::kHeight) y = 0; if (y < 0) y = constants::kHeight;
    }

public:
    bool thrust;
    int lives;
    int score;
private:
    static inline const float maxSpeed = 15.0;
    static inline const float speed_mult = 0.99;
};

#endif //PLAYER_H_
