//로직을 처리하기 위한 쓰레드
//일정 틱을 가지고 패킷을 처리함

#pragma once
#include<vector>
#include"../gameobject/GameObject.h"

#include<system/LogSystem.h>
#include"../BasePacketThread.h"
#include"../ObjectManager.h"

#include"Time.h"

namespace neo::system {
	class LogicThread : public BasePacketThread
	{
	public:
		LogicThread(const int& id);
		~LogicThread();

	public:
		void Start() override;
		void Stop() override;

	public:
		inline packet::PacketProcess* GetPacketProcess() override
		{
			return mPacketProcess.get();
		}

		std::weak_ptr<server::ObjectManager> GetObjectManager() const
		{
			return mObjectManager;
		}

	protected:
		void ThreadUpdate() override;
	private:
		void ComponentUpdate(const double& deltaTime);

	private:
		std::shared_ptr<server::ObjectManager> mObjectManager;
		//time
		std::unique_ptr<Time> mTime;
	};
}
