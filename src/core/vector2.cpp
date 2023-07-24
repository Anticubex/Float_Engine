#include "vector2.h"
#include "math.h"
#include "xmath.h"

Vector::Vector(float X, float Y) : x(X), y(Y) {}

Vector Vector::operator+(const Vector &other) {
        return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(const Vector &other) {
        return Vector(x - other.x, y - other.y);
}

Vector &Vector::operator+=(const Vector &other) {
        x += other.x;
        y += other.y;
        return *this;
}

Vector &Vector::operator-=(const Vector &other) {
        x -= other.x;
        y -= other.y;
        return *this;
}

Vector Vector::operator*(const float &scalar) {
        return Vector(x * scalar, y * scalar);
}

Vector Vector::operator/(const float &scalar) {
        return Vector(x / scalar, y / scalar);
}

float Vector::magSquared() { return (x * x) + (y * y); }

float Vector::magnitude() { return sqrt((x * x) + (y * y)); }

Vector Vector::normalized() {
        float invMag = xmath::fastInvSqrt(magSquared());
        return this->operator/(invMag);
}
