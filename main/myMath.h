//specific math functions, wanted to avoid using std for math (although cmath is used still for trig and sqrt)

class Vector2{
public:
    float x,y;

    Vector2();
    Vector2(float x, float y);

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;

    float dot(const Vector2& other) const;
    float angle(const Vector2& other) const;
    float magnitude() const;
    Vector2 normalize() const;
};