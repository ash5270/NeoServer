#pragma once
#include "GameObject.h"
namespace neo::object {
	class MonsterManager;
	class MonsterObject : public GameObject 
	{
	public:
		MonsterObject();
		~MonsterObject();

	public:
		void Update(const double& deltaTime)override;
		void Start() override;
		void End()override;

	public:
		int32_t GetHp() const;
		void SetHp(const int32_t& value);
		
		void SetExp(const int32_t& value);
		int32_t GetExp() const;

		void SetDamage(const int32_t& value);
		void ResetMonster();
	public:
		std::weak_ptr<MonsterManager> MonsterManager;
	private:
		//
		int32_t mHp;
		
		//ÇöÀç HP
		int32_t mCurrentHp;
		int32_t mDamage;
		int32_t mExp;

		Vector2 mNextPosition;
		Vector2 mStartPosition;

		//timer
		float mMoveTimer =0.0f;
		float mAttackTimer = 0.0f;
		bool mIsAttackTime = false;
	};
}
