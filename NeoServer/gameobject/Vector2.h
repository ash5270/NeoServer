#pragma once
#include"Vector3.h"
namespace neo::object
{
	class Vector2
	{
	public:
		Vector2();
		Vector2(float x, float y);
		Vector2(const Vector2& vector2);
		Vector2(const Vector3& vector3);

		void operator+=(const Vector2& vector2);
		void operator-=(const Vector2& vector2);

		Vector2 operator+(const Vector2& vector2);
		Vector2 operator-(const Vector2& vector2);

		static float Distance(const Vector2& a, const Vector2& b);
		static Vector2 Lerp(const Vector2& start, const Vector2 target, const float& d1);



		float x;
		float y;



	};
}