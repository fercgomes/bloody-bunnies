#include "Vector3D.h"

Vector3D::Vector3D()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector3D::Vector3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3D& Vector3D::Add(const Vector3D& vec)
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    return *this;
}

Vector3D& Vector3D::Subtract(const Vector3D& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    return *this;
}

Vector3D& Vector3D::Multiply(const Vector3D& vec)
{
    this->x *= vec.x;
    this->y *= vec.y;
    this->z *= vec.z;
    return *this;
}

Vector3D& Vector3D::Divide(const Vector3D& vec)
{
    this->x /= vec.x;
    this->y /= vec.y;
    this->z /= vec.z;
    return *this;
}

Vector3D& operator+(Vector3D& v1, const Vector3D& v2)
{
    return v1.Add(v2);
}

Vector3D& operator-(Vector3D& v1, const Vector3D& v2)
{
    return v1.Subtract(v2);
}

Vector3D& operator*(Vector3D& v1, const Vector3D& v2)
{
    return v1.Multiply(v2);
}

Vector3D& operator/(Vector3D& v1, const Vector3D& v2)
{
    return v1.Divide(v2);
}

Vector3D& Vector3D::operator+=(const Vector3D& vec)
{
    return this->Add(vec);
}

Vector3D& Vector3D::operator-=(const Vector3D& vec)
{
    return this->Subtract(vec);
}

Vector3D& Vector3D::operator*=(const Vector3D& vec)
{
    return this->Multiply(vec);
}

Vector3D& Vector3D::operator/=(const Vector3D& vec)
{
    return this->Divide(vec);
}

Vector3D& Vector3D::operator*(const int& i)
{
    this->x *= i;
    this->y *= i;
    this->z *= i;
    return *this;
}

Vector3D& Vector3D::Zero()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector3D& vec)
{
    stream << "(" << vec.x << "," << vec.y << "," << vec.y << ")";
    return stream;
}