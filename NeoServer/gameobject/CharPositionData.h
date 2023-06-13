#pragma once
#include<string>
#include<vector>
#include"Vector3.h"
#include"Vector2.h"
namespace neo::object {
	struct CharPositionData
	{
	public:
		std::wstring name;
		Vector3 position;
		Vector2 veleoctiy;
		float speed;
		int32_t animation;
	};
}