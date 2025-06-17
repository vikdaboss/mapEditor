#include <myMath.h>
#include <cmath>


//VECTOR2 IMPLEMENTATION
Vector2::Vector2() : x(0),y(0) {};
Vector2::Vector2(float x, float y) : x(x), y(y) {};

Vector2 Vector2::operator+(const Vector2& other) const{
    return Vector2(other.x + x, other.y + y);
};

Vector2 Vector2::operator-(const Vector2& other) const{
    return Vector2(x - other.x, y - other.y);
};

Vector2 Vector2::operator*(float scalar) const{
    return Vector2(x * scalar, y * scalar);
};

float Vector2::dot(const Vector2& other) const{
    return x * other.x + y * other.y; 
};

float Vector2::angle(const Vector2& other) const{
    return acos(dot(other) / (magnitude() * other.magnitude()));
};

float Vector2::magnitude() const{
    return sqrt( x*x + y*y);
};

Vector2 Vector2::normalize() const{
    float mag = magnitude();
    if(mag == 0) return Vector2(0,0);
    return Vector2(x/mag,y/mag);
};