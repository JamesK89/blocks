#pragma once
#ifndef _INCLUDE_Vector3_H_
#define _INCLUDE_Vector3_H_
#include <math.h>

#define sqrtr	sqrtf

typedef float real;

class Vector3
{
public:
	real x_, y_, z_;
	
	Vector3(void);
	Vector3(real value);
	Vector3(real x, real y, real z);
	Vector3(const Vector3& copy);
	
	~Vector3(void);
	
	static Vector3 Empty(void);
	
	/* Basic Operations */
	
	real MagnitudeSq(void) const;
	real Magnitude(void) const;
	static real Magnitude(const Vector3& v);
	static real MagnitudeSq(const Vector3& v);
	
	real Dot(const Vector3& rhs) const;
	static real Dot(const Vector3& lhs, const Vector3& rhs);
	
	Vector3& Normalize(void);
	Vector3 Normalized(void) const;
	static Vector3 Normalized(const Vector3& v);
	
	Vector3 Cross(const Vector3& rhs) const;
	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	
	/* Subscript */
	
	real& operator[](const unsigned int index);
	
	/* Assignment */
	
	Vector3& Set(const Vector3& copy);
	Vector3& Set(const real value);
	
	Vector3& operator=(const Vector3& rhs);
	Vector3& operator=(const real rhs);
	
	/* Add */
	
	Vector3& Add(const Vector3& rhs);
	Vector3& Add(const real rhs);
	
	static Vector3 Add(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Add(const Vector3& lhs, const real rhs);
	
	Vector3 operator+(const Vector3& rhs);
	Vector3& operator+=(const Vector3& rhs);
	
	Vector3 operator+(const real rhs);
	Vector3& operator+=(const real rhs);
	
	/* Subtract */
	
	Vector3& Subtract(const Vector3& rhs);
	Vector3& Subtract(const real rhs);
	
	static Vector3 Subtract(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Subtract(const Vector3& lhs, const real rhs);
	
	Vector3 operator-(const Vector3& rhs);
	Vector3& operator-=(const Vector3& rhs);
	
	Vector3 operator-(const real rhs);
	Vector3& operator-=(const real rhs);
	
	/* Multiply */
	
	Vector3& Multiply(const Vector3& rhs);
	Vector3& Multiply(const real rhs);
	
	static Vector3 Multiply(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Multiply(const Vector3& lhs, const real rhs);

	Vector3 operator*(const Vector3& rhs);
	Vector3& operator*=(const Vector3& ths);
	
	Vector3 operator*(const real rhs);
	Vector3& operator*=(const real rhs);
	
	/* Divide */
	
	Vector3& Divide(const Vector3& rhs);
	Vector3& Divide(const real rhs);
	
	static Vector3 Divide(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Divide(const Vector3& lhs, const real rhs);
	
	Vector3 operator/(const Vector3& rhs);
	Vector3& operator/=(const Vector3& rhs);
	
	Vector3 operator/(const real rhs);
	Vector3& operator/=(const real rhs);
};

#endif