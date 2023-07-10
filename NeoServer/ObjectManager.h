#pragma once
//게임 오브젝트 관리를 위한 매니저
#include"gameobject/GameObject.h"
#include<memory>
#include<unordered_map>
#include<string>


namespace neo::server
{
	class ObjectManager : public std::enable_shared_from_this<ObjectManager>
	{
	public:
		ObjectManager();
		~ObjectManager();

	public:
		void ObjectUpdate(const double& deltaTime);
		void ObjectStart();
		void ObjectEnd();
		
		
		void RegisterObject(const wstring& name, std::shared_ptr<object::GameObject>& gameObject);
		void RegisterObject(const wstring& name, std::shared_ptr<object::GameObject>& gameObject,neo::server::Session* session);
		void RegisterObject(const wstring& name);
		void RegisterObject(const wstring& name, neo::server::Session* session);
		void UnregisterObject(const wstring& name);

		weak_ptr<object::GameObject> GetGameObject(const wstring& name);

		/*std::unordered_map<std::wstring, std::shared_ptr<object::GameObject>>& GetGameObjects() 
		{
			return mGameObjects;
		}*/

		nlohmann::json JsonGetAllObjectName();
		nlohmann::json JsonGetAllObjectName(const wstring& name);
		
		size_t GetSize() const
		{
			return mGameObjects.size();
		}
	private:
		std::unordered_map<std::wstring, std::shared_ptr<object::GameObject>> mGameObjects;
		std::list<std::wstring> mCloseSession;
	};
}

