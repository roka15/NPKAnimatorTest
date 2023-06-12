#include "Math.h"

Vector2 Vector2::Up = { 0.0f,-1.0f };
Vector2 Vector2::Right = { 1.0f,0.0f };

Vector2 Vector2::One = { 1.0f,1.0f };
Vector2 Vector2::Zero = { 0.0f,0.0f };

Vector2 operator-(const Vector2& _value1, const Vector2& _value2)
{
    Vector2 temp;
    temp.x = _value1.x - _value2.x;
    temp.y = _value1.y - _value2.y;
    return temp;
}
