#pragma once
#include <math.h>
#include <utility>
#define PI 3.141592

struct Vector2
{
	static Vector2 Up;
	static Vector2 Right;

	static Vector2 One;
	static Vector2 Zero;

	float x;
	float y;

	Vector2() :
		x(0.0f)
		, y(0.0f)
	{
		
	}
	Vector2(std::pair<int, int> _pair)
		:x(_pair.first),y(_pair.second)
	{

	}
	Vector2(int _x, int _y)
		:x(_x)
		, y(_y)
	{

	}
	Vector2(float _x, float _y)
		:x(_x)
		, y(_y)
	{

	}
	Vector2(long _x, long _y)
		:x(_x)
		,y(_y)
	{

	}
	/*Vector2(Vector2& _other)
	{
		x = _other.x;
		y = _other.y;
	}*/
	Vector2 operator-()
	{
		return Vector2(-x, -y);
	}
	Vector2 operator*(const Vector2& _value)
	{
		Vector2 temp;
		temp.x = x * _value.x;
		temp.y = y * _value.y;

		return temp;
	}
	Vector2 operator*(float _value)
	{
		Vector2 temp;
		temp.x = x * _value;
		temp.y = y * _value;
		return temp;
	}
	Vector2 operator/(float _value)
	{
		if (_value == 0)
			return *this;
		Vector2 temp;
		temp.x = x / _value;
		temp.y = y / _value;
		return temp;
	}
	void operator*=(float _value)
	{
		x *= _value;
		y *= _value;
	}
	void operator-=(const float& value)
	{
		x -= value;
		y -= value;
	}
	void operator+=(const Vector2& _value)
	{
		x += _value.x;
		y += _value.y;
	}
	void operator-=(const Vector2& _value)
	{
		x -= _value.x;
		y -= _value.y;
	}
	void operator*=(const Vector2& _value)
	{
		x *= _value.x;
		y *= _value.y;
	}
	Vector2 operator-(const Vector2& _value)
	{
		Vector2 temp;	
		temp.x = x - _value.x;
		temp.y = y - _value.y;
		return temp;
	}
	friend Vector2 operator-(const Vector2& _value1, const Vector2& _value2);
	Vector2 operator+(const Vector2& _value)
	{
		Vector2 temp;
		temp.x = x + _value.x;
		temp.y = y + _value.y;

		return temp;
	}
	bool operator!=(const Vector2& _other)
	{
		return this->x != _other.x || this->y != _other.y;
	}
	bool operator==(const Vector2& _other)
	{
		return (x == _other.x && y == _other.y);
	}
	

	Vector2(const Vector2&) = default;
	Vector2(Vector2&&) = default;
	Vector2& operator=(const Vector2&) = default;
	Vector2& operator=(Vector2&&) = default;

	void Clear()
	{
		x = 0.0f;
		y = 0.0f;
	}
	float Length()
	{
		return sqrtf(x * x + y * y);
	}

	Vector2& Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;

		return *this;
	}
	
};
namespace yeram_client::math
{
	inline static Vector2 Rotate(Vector2 _vector, float degree)
	{
		float radian = (degree / 180.0f) * PI;
		_vector.Normalize();
		float x = _vector.x * cosf(radian) - _vector.y * sinf(radian);
		float y = _vector.x * sinf(radian) + _vector.y * cosf(radian);

		return Vector2(x, y);
	}
	
	inline static float Dot(Vector2& v1,Vector2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	
	inline static float Cross(Vector2& v1, Vector2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}
}