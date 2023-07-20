
#pragma once

class Vector {
      public:
        Vector(float X = 0, float Y = 0);

        Vector operator+(const Vector &othervec);
        Vector operator-(const Vector &othervec);

        Vector &operator+=(const Vector &othervec);
        Vector &operator-=(const Vector &othervec);

        Vector operator*(const float &scalar);
        Vector operator/(const float &scalar);

        float x;
        float y;
};