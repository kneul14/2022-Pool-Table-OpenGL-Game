#include "HowToFindDotProduct.h"

float vectorthree::GetX()
{
    return x;
}

float vectorthree::GetY()
{
    return y;
}

float vectorthree::GetZ()
{
    return z;
}

vec3 vectorthree::vector3()
{
    x, y, z = 0.0f;
    return vec3(0, 0, 0);
}

vec3 vectorthree::vector3(float _x, float _y, float _z)
{
     _x = x;
     _y = y;
     _z = z;
     return vec3(0, 0, 0);
}

float vectorthree::DotProduct(vectorthree other)
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

vec3 vectorthree::CrossProduct(vectorthree other)
{
    return vec3(
        this->y * other.z - z * other.y, 
        z - other.z - x * other.z,
        x * other.y - y * other.x
    );
}
