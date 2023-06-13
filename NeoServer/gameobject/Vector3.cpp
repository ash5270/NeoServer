#include"Vector3.h"
#include"Vector2.h"
#include<cmath>
neo::object::Vector3::Vector3()
{
	x = y = z = 0;
}

neo::object::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

neo::object::Vector3::Vector3(const Vector3& vector3)
{
	this->x = vector3.x;
	this->y = vector3.y;
	this->z = vector3.z;
}

neo::object::Vector3::Vector3(const Vector2& vector2)
{
	this->x = vector2.x;
	this->y = vector2.y;
	this->z = 0;
}

void neo::object::Vector3::SetVector3(const float& x, const float& y, const float& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void neo::object::Vector3::operator=(const Vector3& vector3)
{
	this->x = vector3.x;
	this->y = vector3.y;
	this->z = vector3.z;
}

void neo::object::Vector3::operator+=(const Vector3& vector3)
{
	this->x += vector3.x;
	this->y += vector3.y;
	this->z += vector3.z;
}

void neo::object::Vector3::operator-=(const Vector3& vector3)
{
	this->x += vector3.x;
	this->y += vector3.y;
	this->z += vector3.z;
}

void neo::object::Vector3::operator+=(const Vector2& vector3)
{
	this->x += vector3.x;
	this->y += vector3.y;
}

void neo::object::Vector3::operator-=(const Vector2& vector3)
{
	this->x += vector3.x;
	this->y += vector3.y;
}

neo::object::Vector3 neo::object::Vector3::operator+(const Vector3& vector3)
{
	Vector3 new_vec = Vector3(x + vector3.x, y + vector3.y, z+vector3.z);
	return new_vec;
}

neo::object::Vector3 neo::object::Vector3::operator-(const Vector3& vector3)
{
	Vector3 new_vec = Vector3(x - vector3.x, y - vector3.y, z-vector3.z);
	return new_vec;
}

neo::object::Vector3 neo::object::Vector3::operator+(const Vector2& vector2)
{
	Vector3 new_vec = Vector3(x + vector2.x, y + vector2.y, z);
	return new_vec;
}

neo::object::Vector3 neo::object::Vector3::operator-(const Vector2& vector2)
{
	Vector3 new_vec = Vector3(x - vector2.x, y - vector2.y, z);
	return new_vec;
}

float neo::object::Vector3::Distance(const Vector3& a, const Vector3& b)
{
	float dis = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z));
	return sqrt(dis);
	//return 0;
}

neo::object::Vector3 neo::object::Vector3::Lerp(const Vector3& start, const Vector3& target,const float& d1)
{
	Vector3 result;
	result.x = (1 - d1) * start.x + d1 * target.x;
	result.y = (1 - d1) * start.y + d1 * target.y;
	result.z = (1 - d1) * start.z + d1 * target.z;
	return result;
}
