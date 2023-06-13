#pragma once

#include"Object.h"
#include"Transform.h"
#include"../gameserver/Session.h"
#include<system/NeoLog.h>

namespace neo::server
{
	class ObjectManager;
}

namespace neo::object
{
	struct GameObject: public Object
	{
	public:
		virtual void Update(const double& deltaTime){
			//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"update object is Position (%f,%f,%f)\n", transform.position.x, transform.position.y, transform.position.z);
		}
		virtual void Start(){}
		virtual void End(){}
		
		bool GetActive() const
		{
			return mActive;
		}

		void SetActive(const bool& active)
		{
			mActive = active;
		}

	public:
		Transform transform;
		int32_t Animation;
		neo::server::Session* Session;
		std::weak_ptr< neo::server::ObjectManager> Manager;

	protected:
		bool mActive;
	};
}
