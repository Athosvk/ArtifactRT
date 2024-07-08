#pragma once

#include "Vector3.h"

#include <cmath>
#include <algorithm>
#include <limits>
#include <stdexcept>

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
		default: throw std::out_of_range("Index >= 3");
	}
}

inline double& Vector3::operator[](int index)
{
	switch (index)
	{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		default: throw std::out_of_range("Index >= 3");
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
	return *this;
}

inline Vector3& Vector3::operator/=(const double scalar)
{
	X /= scalar;
	Y /= scalar;
	Z /= scalar;
	return *this;
}

inline Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(X + other.X, Y + other.Y, Z + other.Z);
}

inline Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(X - other.X, Y - other.Y, Z - other.Z);
}

inline Vector3 Vector3::operator*(const Vector3& other) const
{
	return Vector3(X * other.X, Y * other.Y, Z * other.Z);
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

inline bool Vector3::IsNearZero() const
{
	const auto epsilon = -1e8;
	return std::abs(X) < epsilon && std::abs(Y) < epsilon && std::abs(Z) < epsilon;
}

inline Vector3 Vector3::Reflect(const Vector3& normal) const
{
	// Project this vector on the normal to get the length of the reflected vector
	double projected_length = GetDotProduct(normal);
	// Offset by normal projection * 2 to scale in opposite direction
	// Assumes inward vector (this) ends at tail of normal
	return *this - ( 2 * projected_length * normal);
}

inline Vector3 Vector3::Refract(const Vector3& normal, double etaOverEtaPrime) const
{
	double cos_theta = std::min(GetDotProduct(-normal), 1.0);
	Vector3 r_out_perpendicular = etaOverEtaPrime * (*this + cos_theta * normal);
	Vector3 r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perpendicular.GetLengthSquared())) * normal;
	return r_out_perpendicular + r_out_parallel;
}

inline Vector3 Vector3::Min(const Vector3& other) const
{
	return Vector3(std::min(X, other.X), std::min(Y, other.Y), std::min(Z, other.Z));
}

inline Vector3 Vector3::Max(const Vector3& other) const
{
	return Vector3(std::max(X, other.X), std::max(Y, other.Y), std::max(Z, other.Z));
}

constexpr inline Vector3 Vector3::Zero()
{
	return Vector3::Fill(0.0);
}

inline constexpr Vector3 Vector3::PosInfinity()
{
	return Vector3::Fill(std::numeric_limits<float>::infinity());
}

inline constexpr Vector3 Vector3::NegInfinity()
{
	return Vector3::Fill(-std::numeric_limits<float>::infinity());
}

inline constexpr Vector3 Vector3::Fill(double value)
{
	return Vector3(value, value, value);
}

inline Vector3 operator*(double scalar, const Vector3& vector)
{
	return vector * scalar;
}
