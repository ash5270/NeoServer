#pragma once
#include"DatabaseCommon.h"
#include<Singleton.h>
#include<memory>

namespace neo::db {
	class DataBaseManager  : public neo::Singleton<DataBaseManager>
	{
	public:
		void Init();
		void Stop();

		MYSQL* GetNewConnection();
		MYSQL* GetConnection();
	private:
		MYSQL* mConntion;
	};
}

