#pragma once
#include <iostream>

class Vector3D
{

public:
    float x;
    float y;
    float z;

    Vector3D();
    Vector3D(float x, float y, float z);

    Vector3D& Add(const Vector3D& vec);
    Vector3D& Subtract(const Vector3D& vec);
    Vector3D& Multiply(const Vector3D& vec);
    Vector3D& Divide(const Vector3D& vec);

    friend Vector3D& operator+(Vector3D& v1, const Vector3D& v2);
    friend Vector3D& operator-(Vector3D& v1, const Vector3D& v2);
    friend Vector3D& operator*(Vector3D& v1, const Vector3D& v2);
    friend Vector3D& operator/(Vector3D& v1, const Vector3D& v2);

    Vector3D& operator+=(const Vector3D& vec);
    Vector3D& operator-=(const Vector3D& vec);
    Vector3D& operator*=(const Vector3D& vec);
    Vector3D& operator/=(const Vector3D& vec);

    Vector3D& operator*(const int& i);
    Vector3D& Zero();

    friend std::ostream& operator<<(std::ostream& stream, const Vector3D& vec);

};