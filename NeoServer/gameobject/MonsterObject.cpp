#include "MonsterObject.h"
#include "MapData.h"
#include"PlayerObject.h"
#include<system/NeoLog.h>
#include<random>

neo::object::MonsterObject::MonsterObject() :mMoveTimer(0), mCurrentHp(0), mDamage(0), mNextPosition(), mStartPosition(), mHp(0)
{
	mActive = true;
}

neo::object::MonsterObject::~MonsterObject()
{

}

void neo::object::MonsterObject::Update(const double& deltaTime)
{
	if (mMoveTimer > 7.0f)
	{
		random_device rd;
		mt19937_64 gen(rd());
		uniform_int_distribution<> dist(-3, 3);

		mStartPosition = transform.position;
		//x 16
		//y 7.5
		float x = dist(gen);
		if (transform.position.x + x > 16.0f)
			mNextPosition.x = transform.position.x - x;
		else if (transform.position.x + x < -16)
			mNextPosition.x = transform.position.x + (x * (-1.f));
		else
			mNextPosition.x = transform.position.x + x;

		float y = dist(gen);
		if (transform.position.y + y > 7.0f)
			mNextPosition.y = transform.position.y - y;
		else if (transform.position.y + y < -7.0f)
			mNextPosition.y = transform.position.y + (y * (-1.f));
		else
			mNextPosition.y = transform.position.y + y;

		mMoveTimer = 0.0f;
	}

	mMoveTimer += deltaTime;
	mAttackTimer += deltaTime;
	transform.position = Vector2::Lerp(mStartPosition, mNextPosition, mMoveTimer / 5.0f);

	if (MonsterManager.expired())
		return;

	auto mapData = MonsterManager.lock()->GetMapData();
	auto mapDataPtr = mapData.lock();

	if (mIsAttackTime && mAttackTimer > 2.0f)
	{
		mIsAttackTime = false;
	}
	
	if (!mIsAttackTime)
	{
		for (const auto& player : mapDataPtr->GetInMapPlayers())
		{
			auto objectPtr = player.second.lock();
			auto playerPtr = std::dynamic_pointer_cast<object::PlayerObject>(objectPtr);
			float distance = Vector3::Distance(playerPtr->transform.position,
				this->transform.position);
			if (distance < 1.5f)
			{
				P_S_NOTIFY_CHAR_UPDATE update;
				update.exp = playerPtr->GetEXP();
				update.hp = playerPtr->GetHp();
				update.level = playerPtr->GetLevel();
				update.hp -= mDamage;
				objectPtr->Session->SendPacket(update);

				playerPtr->SetHp(update.hp);
				mIsAttackTime = true;
				mAttackTimer = 0.0f;
			}

			if (distance < 4.0f && distance>1.0f)
			{
				mNextPosition = player.second.lock()->transform.position;
			}
		}
	}

}

void neo::object::MonsterObject::Start()
{
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<> dist(-3, 3);

	transform.position.x = dist(gen);
	transform.position.y = dist(gen);
}

void neo::object::MonsterObject::End()
{

}

int32_t neo::object::MonsterObject::GetHp() const
{
	return mCurrentHp;
}

void neo::object::MonsterObject::SetHp(const int32_t& value)
{
	if (mHp == 0)
		mHp = value;
	mCurrentHp = value;
}

void neo::object::MonsterObject::SetExp(const int32_t& value)
{
	mExp = value;
}

int32_t neo::object::MonsterObject::GetExp() const
{
	return mExp;
}

void neo::object::MonsterObject::SetDamage(const int32_t& value)
{
	mDamage = value;
}

void neo::object::MonsterObject::ResetMonster()
{
	mCurrentHp = mHp;
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<> dist(-3, 3);

	transform.position.x = dist(gen);
	transform.position.y = dist(gen);
}
