//log data�� ó���ϱ� ���� Ŭ����
#pragma once

#include "../Singleton.h"

namespace util::system
{
	class LogSystem : Singleton<LogSystem>
	{
	public:
		void StartLogSystem();
		void StopLogSystem();
	private:
		LogSystem();
		virtual ~LogSystem();
	};
}
