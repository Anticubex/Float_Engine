#include "vector.h"

Vector::Vector(float X, float Y) {
        x = X;
        y = Y;
}

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