#pragma once
#include"GameObject.h"

namespace neo::object
{
	struct PlayerObject : public GameObject
	{
	public:
		void Update(const double& deltaTime) override {}
		void Start() override {}
		void End() override {}


		void SetHp(const int32_t& value)
		{
			mHp = value;
		}

		void SetEXP(const int32_t& value)
		{
			if (value + mExp >= 100)
			{
				mLevel+=1;
				mExp = 0;
			}
			else
				mExp = value;
		}

		void SetLevel(const int32_t& value)
		{
			mLevel = value;
		}

		int32_t GetHp() const {
			return mHp;
		}

		int32_t GetEXP() const
		{
			return mExp;
		}

		int32_t GetLevel() const
		{
			return mLevel;
		}

		void Respwan() {
			mHp = mInitHp;
		}

		void InitData(const int32_t& max_hp, const int32_t& hp, const int32_t& exp, const int32_t& level)
		{
			mInitHp = max_hp;
			mInitExp = exp;
			mInitLevel = level;

			mHp = hp;
			mExp = exp;
			mLevel = level;
		}

	private:
		int32_t mHp = 0;
		int32_t mExp= 0;
		int32_t mLevel=0;

		int32_t mInitHp;
		int32_t mInitExp;
		int32_t mInitLevel;
	};
}