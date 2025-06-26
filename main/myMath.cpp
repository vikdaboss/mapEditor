#include <myMath.h>
#include <cmath>
#include <iostream>

//VECTOR2 IMPLEMENTATION
Vector2::Vector2() : x(0),y(0) {};
Vector2::Vector2(float x, float y) : x(x), y(y) {};

Vector2 Vector2::operator+(const Vector2& other) const{
    return Vector2(other.x + x, other.y + y);
};

Vector2 Vector2::operator+(const float* other) const{
    return Vector2(other[0] + x, other[1] + y);
};

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Vector2& Vector2::operator+=(const float* other) {
    x += other[0];
    y += other[1];
    return *this;
}

Vector2 Vector2::operator-(const Vector2& other) const{
    return Vector2(x - other.x, y - other.y);
};

Vector2 Vector2::operator-(const float* other) const{
    return Vector2( x- other[0], y - other[1]);
};

Vector2 Vector2::operator-(){
    return Vector2( -x, -y);
};

Vector2 Vector2::operator*(float scalar) const{
    return Vector2(x * scalar, y * scalar);
};

Vector2 Vector2::operator/(float scalar) const{
    return Vector2(x / scalar, y / scalar);
};

Vector2& Vector2::operator=(const Vector2& other) {
    x = other.x;
    y = other.y;
    return *this;
}

Vector2& Vector2::operator=(const float* other) {
        x = other[0];
        y = other[1];
        return *this;    
}

float Vector2::dot(const Vector2& other) const{
    return x * other.x + y * other.y; 
};

float Vector2::angle(const Vector2& other) const{
    return acos(dot(other) / (magnitude() * other.magnitude()));
};

float Vector2::magnitude() const{
    return sqrt( x*x + y*y);
};

Vector2 Vector2::normalized() const{
    float mag = magnitude();
    if(mag == 0) return Vector2(0,0);
    return Vector2(x/mag,y/mag);
};

//orthogonal projection matrix
void ortho(float left, float right, float bottom, float top, float near, float far, float* out) {
//https://learnwebgl.brown37.net/08_projections/projections_ortho.html

    out[0] = 2.0f / (right - left);
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;

    out[4] = 0.0f;
    out[5] = 2.0f / (top - bottom);
    out[6] = 0.0f;
    out[7] = 0.0f;

    out[8] = 0.0f;
    out[9] = 0.0f;
    out[10] = -2.0f / (far - near);
    out[11] = 0.0f;

    out[12] = -(right + left) / (right - left);
    out[13] = -(top + bottom) / (top - bottom);
    out[14] = -(far + near) / (far - near);
    out[15] = 1.0f;
}
//translation matrix
void translate(Vector2 vec, float* out){
    out[0] = 1.0f; out[4] = 0.0f; out[8] = 0.0f; out[12] = vec.x;
    out[1] = 0.0f; out[5] = 1.0f; out[9] = 0.0f; out[13] = vec.y;
    out[2] = 0.0f; out[6] = 0.0f; out[10] = 1.0f; out[14] = 0.0f;
    out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

// mutliplying 2 4x4 matrices
void multMatrix(const float* a, const float* b, float* out) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            out[col + row * 4] =
                a[0 + row * 4] * b[col + 0] +
                a[1 + row * 4] * b[col + 4] +
                a[2 + row * 4] * b[col + 8] +
                a[3 + row * 4] * b[col + 12];
        }
    }
}


void invertMatrix(const float* m, float* out)
{//from the GLU library
    float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det != 0)
    {
        det = 1.0 / det;
        for (i = 0; i < 16; i++)
            out[i] = inv[i] * det;
    }    
}

void multMatrixVec4(float* mat, float* vec, float* out){
    out[0]= vec[0]* mat[0]+ vec[1]* mat[1]+ vec[2]* mat[2]+ vec[3]*mat[3];
    out[1]= vec[0]* mat[4]+ vec[1]* mat[5]+ vec[2]* mat[6]+ vec[3]*mat[7];
    out[2]= vec[0]* mat[8]+ vec[1]* mat[9]+ vec[2]* mat[10]+ vec[3]*mat[11];
    out[3]= vec[0]* mat[12]+ vec[1]* mat[13]+ vec[2]* mat[14]+ vec[3]*mat[15];
} 

void multVec4Matrix(float* vec, float* mat, float* out){
    out[0]= vec[0]* mat[0]+ vec[1]* mat[4]+ vec[2]* mat[8]+ vec[3]*mat[12];
    out[1]= vec[0]* mat[1]+ vec[1]* mat[5]+ vec[2]* mat[9]+ vec[3]*mat[13];
    out[2]= vec[0]* mat[2]+ vec[1]* mat[6]+ vec[2]* mat[10]+ vec[3]*mat[14];
    out[3]= vec[0]* mat[3]+ vec[1]* mat[7]+ vec[2]* mat[11]+ vec[3]*mat[15];
} 