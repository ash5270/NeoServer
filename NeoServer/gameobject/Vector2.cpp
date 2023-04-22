#include"Vector2.h"
#include<cmath>
neo::object::Vector2::Vector2()
{
	x = y = 0;
}

neo::object::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

neo::object::Vector2::Vector2(const Vector2& vector2)
{
	this->x = vector2.x;
	this->y = vector2.y;
}

neo::object::Vector2::Vector2(const Vector3& vector3)
{
	this->x = vector3.x;
	this->y = vector3.y;
}

void neo::object::Vector2::operator+=(const Vector2& vector2)
{
	this->x += vector2.x;
	this->y += vector2.y;
}

void neo::object::Vector2::operator-=(const Vector2& vector2)
{
	this->x += vector2.x;
	this->y += vector2.y;
}

neo::object::Vector2 neo::object::Vector2::operator+(const Vector2& vector2)
{
	Vector2 new_vec = Vector2(x + vector2.x, y + vector2.y);
	return  new_vec;
}

neo::object::Vector2 neo::object::Vector2::operator-(const Vector2& vector2)
{
	Vector2 new_vec = Vector2(x + vector2.x, y + vector2.y);
	return  new_vec;
}

float neo::object::Vector2::Distance(const Vector2& a, const Vector2& b)
{
	float dis = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
	return sqrt(dis);
	//return 0;
}
