#pragma once

#include "Vector3.h"

#include <cmath>

constexpr inline Vector3::Vector3(double x, double y, double z) :
	X(x), Y(y), Z(z)
{
}

inline Vector3 Vector3::operator-() const
{
	return Vector3(-X, -Y, -Z);
}

inline double Vector3::operator[](int index) const
{
	switch (index)
	{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
	}
}

inline double& Vector3::operator[](int index)
{
	switch (index)
	{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
	}
}

inline Vector3& Vector3::operator+=(const Vector3& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	return *this;
}

inline Vector3& Vector3::operator*=(const double scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;
}

inline Vector3& Vector3::operator/=(const double scalar)
{
	X /= scalar;
	Y /= scalar;
	Z /= scalar;
}

inline Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(X + other.X, Y + other.Y, Z + other.Z);
}

inline Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(X - other.X, Y - other.Y, Z - other.Z);
}

inline Vector3 Vector3::operator*(const double scalar) const
{
	return Vector3(X * scalar, Y * scalar, Z * scalar);
}

inline Vector3 Vector3::operator/(const double scalar) const
{
	return Vector3(X / scalar, Y / scalar, Z / scalar);
}

inline std::ostream& Vector3::operator<<(std::ostream& outputStream) const
{
	return outputStream << X << ", " << Y << ", " << Z;
}

inline double Vector3::GetLength() const
{
	return std::sqrt(GetLengthSquared());
}

inline double Vector3::GetLengthSquared() const
{
	return X * X + Y * Y + Z * Z;
}

inline double Vector3::GetDotProduct() const
{
	return GetLengthSquared();
}

inline double Vector3::GetDotProduct(const Vector3& other) const
{
	return X * other.X + Y * other.Y + Z * other.Z;
}

inline Vector3 Vector3::GetCrossProduct(const Vector3 other) const
{
	return Vector3( Y * other.Z - Z * other.Y, 
					Z * other.X - X * other.Z,
					X * other.Y - Y * other.X);
}

inline Vector3 Vector3::Normalize() const
{
	return *this / GetLength();
}

constexpr inline Vector3 Vector3::GetZero()
{
	return Vector3(0.0, 0.0, 0.0);
}

inline Vector3 operator*(double scalar, const Vector3& vector)
{
	return vector * scalar;
}
