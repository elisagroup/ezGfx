
#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class Vec2
{
public:
	Vec2() : m_x(0), m_y(0) {}
	Vec2(float x, float y) : m_x(x), m_y(y) {}

	Vec2 operator-(const Vec2& other) const
	{
		return Vec2(this->m_x - other.m_x, this->m_y - other.m_y);
	}
	
	Vec2 operator*(float val) const
	{
		return Vec2(this->m_x * val, this->m_y * val);
	}
	
	Vec2& operator+=(const Vec2& other)
	{
		this->m_x += other.m_x;
		this->m_y += other.m_y;
		return *this;
	}

	Vec2& operator-=(const Vec2& other)
	{
		this->m_x -= other.m_x;
		this->m_y -= other.m_y;
		return *this;
	}

	Vec2& operator*=(float val)
	{
		this->m_x *= val;
		this->m_y *= val;
		return *this;
	}

	Vec2 operator-() const
	{
		return Vec2(-this->m_x, -this->m_y);
	}

	float length()
	{
		return sqrt(m_x * m_x + m_y * m_y);
	}
	Vec2 normalized()
	{
		float len = length();
		return Vec2(m_x / len, m_y / len);
	}
	void normalize()
	{
		float len = length();
		m_x /= len;
		m_y /= len;
	}

	float& x()
	{
		return m_x;
	}
	float& y()
	{
		return m_y;
	}
	float x() const
	{
		return m_x;
	}
	float y() const
	{
		return m_y;
	}

private:
	float m_x, m_y;
};

#endif // VEC2_H