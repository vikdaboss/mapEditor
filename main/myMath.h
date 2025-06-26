//specific math functions, wanted to avoid using std for math (although cmath is used still for trig and sqrt)

class Vector2{
public:
    float x,y;

    Vector2();
    Vector2(float x, float y);

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator+(const float* other) const;

    Vector2& operator+=(const Vector2& other);
    Vector2& operator+=(const float* other);

    Vector2 operator-(const Vector2& other) const;
    Vector2 operator-(const float* other) const;
    Vector2 operator-();

    Vector2 operator*(float scalar) const;

    Vector2 operator/(float scalar) const;

    Vector2& operator=(const Vector2& other);
    Vector2& operator=(const float* other);

    float dot(const Vector2& other) const;
    float angle(const Vector2& other) const;
    float magnitude() const;
    Vector2 normalized() const;
};

void ortho(float left, float right, float bottom, float top, float near, float far, float* out);
void translate(Vector2 vec,float* out);
void multMatrix(const float* a, const float* b, float* out);
void invertMatrix(const float* m, float* out);
void multMatrixVec4(float* mat, float* vec, float* out);
void multVec4Matrix(float* vec, float* mat, float* out);