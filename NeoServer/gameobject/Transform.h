#pragma once
#include"Vector3.h"
#include"Vector2.h"
namespace neo::object
{
	struct Transform
	{
		Vector3 position;
		Vector3 rotation;
		Vector2 veloctiy;
	};
}
