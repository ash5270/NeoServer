//log data를 처리하기 위한 클래스
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
