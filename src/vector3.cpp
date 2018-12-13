#include <blocks.hpp>
#include <vector3.hpp>

Vector3::Vector3(void) : 
	x_(0), y_(0), z_(0)
{
}

Vector3::Vector3(real value) :
	x_(value), y_(value), z_(value)
{
}

Vector3::Vector3(real x, real y, real z) :
	x_(x), y_(y), z_(z)
{	
}

Vector3::Vector3(const Vector3& copy) :
	x_(copy.x_), y_(copy.y_), z_(copy.z_)
{
}

Vector3::~Vector3(void)
{
}

Vector3 Vector3::Empty(void)
{
	return Vector3(0, 0, 0);
}

real Vector3::Magnitude(void) const
{
	return Magnitude(*this);
}

real Vector3::MagnitudeSq(void) const
{
	return MagnitudeSq(*this);
}

real Vector3::Magnitude(const Vector3& v)
{
	return sqrtr(MagnitudeSq(v));
}

real Vector3::MagnitudeSq(const Vector3& v)
{
	return (v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_);
}

real Vector3::Dot(const Vector3& rhs) const
{
	return Dot(*this, rhs);
}

real Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
{
	return (lhs.x_ * rhs.x_) + (lhs.y_ * rhs.y_) + (lhs.z_ * rhs.z_);
}

Vector3& Vector3::Normalize(void)
{
	return Set(Normalized());
}

Vector3 Vector3::Normalized(void) const
{
	return Normalized(*this);
}

Vector3 Vector3::Normalized(const Vector3& v)
{
	real mag = Magnitude(v);
	Vector3 result = Vector3::Empty();
	
	if (mag != 0)
	{
		result.x_ = v.x_ / mag;
		result.y_ = v.y_ / mag;
		result.z_ = v.z_ / mag;
	}
	
	return result;
}

Vector3 Vector3::Cross(const Vector3& rhs) const
{
	return Cross(*this, rhs);
}

Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		(lhs.y_ * rhs.z_) - (lhs.z_ * rhs.y_),
		(lhs.z_ * rhs.x_) - (lhs.x_ * rhs.z_),
		(lhs.x_ * rhs.y_) - (lhs.y_ * rhs.x_)
	);
}

/* Subscript */

real& Vector3::operator[](const unsigned int index)
{
	return (((real*)&x_)[index % ((sizeof(Vector3) / sizeof(real)) - 1)]);
}

/* Assignment */

Vector3& Vector3::Set(const Vector3& copy)
{
	x_ = copy.x_;
	y_ = copy.y_;
	z_ = copy.z_;
	
	return *this;
}

Vector3& Vector3::Set(const real value)
{
	x_ = value;
	y_ = value;
	z_ = value;
	
	return *this;
}

Vector3& Vector3::operator=(const Vector3& rhs)
{
	x_ = rhs.x_;
	y_ = rhs.y_;
	z_ = rhs.z_;
	
	return *this;
}

Vector3& Vector3::operator=(const real rhs)
{
	x_ = rhs;
	y_ = rhs;
	z_ = rhs;
	
	return *this;
}

/* Add */

Vector3& Vector3::Add(const Vector3& rhs)
{
	x_ += rhs.x_;
	y_ += rhs.y_;
	z_ += rhs.z_;
	
	return *this;
}

Vector3& Vector3::Add(const real rhs)
{
	x_ += rhs;
	y_ += rhs;
	z_ += rhs;
	
	return *this;
}

Vector3 Vector3::Add(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.x_ + rhs.x_,
		lhs.y_ + rhs.y_,
		lhs.z_ + rhs.z_
	);
}

Vector3 Vector3::Add(const Vector3& lhs, const real rhs)
{
	return Vector3(
		lhs.x_ + rhs,
		lhs.y_ + rhs,
		lhs.z_ + rhs
	);
}

Vector3 Vector3::operator+(const Vector3& rhs)
{
	return Add(*this, rhs);
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
	return Add(rhs);
}

Vector3 Vector3::operator+(const real rhs)
{
	return Add(*this, rhs);
}

Vector3& Vector3::operator+=(const real rhs)
{
	return Add(rhs);
}

/* Subtract */

Vector3& Vector3::Subtract(const Vector3& rhs)
{
	x_ -= rhs.x_;
	y_ -= rhs.y_;
	z_ -= rhs.z_;
	
	return *this;
}

Vector3& Vector3::Subtract(const real rhs)
{
	x_ -= rhs;
	y_ -= rhs;
	z_ -= rhs;
	
	return *this;
}

Vector3 Vector3::Subtract(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.x_ - rhs.x_,
		lhs.y_ - rhs.y_,
		lhs.z_ - rhs.z_
	);
}

Vector3 Vector3::Subtract(const Vector3& lhs, const real rhs)
{
	return Vector3(
		lhs.x_ - rhs,
		lhs.y_ - rhs,
		lhs.z_ - rhs
	);
}

Vector3 Vector3::operator-(const Vector3& rhs)
{
	return Subtract(*this, rhs);
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
	return Subtract(rhs);
}

Vector3 Vector3::operator-(const real rhs)
{
	return Subtract(*this, rhs);
}

Vector3& Vector3::operator-=(const real rhs)
{
	return Subtract(rhs);
}

/* Multiply */

Vector3& Vector3::Multiply(const Vector3& rhs)
{
	x_ *= rhs.x_;
	y_ *= rhs.y_;
	z_ *= rhs.z_;
	
	return *this;
}

Vector3& Vector3::Multiply(const real rhs)
{
	x_ *= rhs;
	y_ *= rhs;
	z_ *= rhs;
	
	return *this;
}

Vector3 Vector3::Multiply(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.x_ * rhs.x_,
		lhs.y_ * rhs.y_,
		lhs.z_ * rhs.z_
	);
}

Vector3 Vector3::Multiply(const Vector3& lhs, const real rhs)
{
	return Vector3(
		lhs.x_ * rhs,
		lhs.y_ * rhs,
		lhs.z_ * rhs
	);
}

Vector3 Vector3::operator*(const Vector3& rhs)
{
	return Multiply(*this, rhs);
}

Vector3& Vector3::operator*=(const Vector3& rhs)
{
	return Multiply(rhs);
}

Vector3 Vector3::operator*(const real rhs)
{
	return Multiply(*this, rhs);
}

Vector3& Vector3::operator*=(const real rhs)
{
	return Multiply(rhs);
}

/* Divide */

Vector3& Vector3::Divide(const Vector3& rhs)
{
	x_ /= rhs.x_;
	y_ /= rhs.y_;
	z_ /= rhs.z_;
	
	return *this;
}

Vector3& Vector3::Divide(const real rhs)
{
	x_ /= rhs;
	y_ /= rhs;
	z_ /= rhs;
	
	return *this;
}

Vector3 Vector3::Divide(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.x_ / rhs.x_,
		lhs.y_ / rhs.y_,
		lhs.z_ / rhs.z_
	);
}

Vector3 Vector3::Divide(const Vector3& lhs, const real rhs)
{
	return Vector3(
		lhs.x_ / rhs,
		lhs.y_ / rhs,
		lhs.z_ / rhs
	);
}

Vector3 Vector3::operator/(const Vector3& rhs)
{
	return Divide(*this, rhs);
}

Vector3& Vector3::operator/=(const Vector3& rhs)
{
	return Divide(rhs);
}

Vector3 Vector3::operator/(const real rhs)
{
	return Divide(*this, rhs);
}

Vector3& Vector3::operator/=(const real rhs)
{
	return Divide(rhs);
}
