#pragma once
#include <iostream>

struct Vector3
{
	Vector3() = default;
	inline constexpr Vector3(double x, double y, double z);
	
	inline Vector3 operator-() const;
	inline double operator[](int index) const;
	inline double& operator[](int index);
	
	inline Vector3& operator+=(const Vector3& other);
	inline Vector3& operator*=(const double scalar);
	inline Vector3& operator/=(const double scalar);
	
	inline Vector3 operator+(const Vector3& other) const;
	inline Vector3 operator-(const Vector3& other) const;
	inline Vector3 operator*(const Vector3& other) const;
	inline Vector3 operator*(const double scalar) const;
	inline Vector3 operator/(const double scalar) const;
	
	inline std::ostream& operator<<(std::ostream& outputStream) const;
	
	inline double GetLength() const;
	inline double GetLengthSquared() const;
	inline double GetDotProduct() const;
	inline double GetDotProduct(const Vector3& other) const;
	inline Vector3 GetCrossProduct(const Vector3 other) const;
	inline Vector3 Normalize() const;
	inline bool IsNearZero() const;
	inline Vector3 Reflect(const Vector3& normal) const;
	inline Vector3 Refract(const Vector3& normal, double refractiveIndex) const;
	inline Vector3 Min(const Vector3& other) const;
	inline Vector3 Max(const Vector3& other) const;
	constexpr inline static Vector3 Zero();
	constexpr inline static Vector3 PosInfinity();
	constexpr inline static Vector3 NegInfinity();
	constexpr inline static Vector3 Fill(double value);

	double X = 0.0;
	double Y = 0.0;
	double Z = 0.0;
};

using Point3 = Vector3;
using RGBColor = Vector3;

std::ostream& WriteColor(std::ostream& outputStream, const RGBColor& color);
inline Vector3 operator*(double scalar, const Vector3& vector);
