#include"ObjectManager.h"
neo::server::ObjectManager::ObjectManager()
{

}

neo::server::ObjectManager::~ObjectManager()
{
}

void neo::server::ObjectManager::ObjectUpdate(const double& deltaTime)
{
	for (auto const& it : mGameObjects)
	{
		if (it.second->GetActive())
		{
			it.second->Update(deltaTime);
		}

		if (it.second->Session != nullptr && !it.second->Session->IsConnect())
		{
			mCloseSession.push_back(it.first);
		}
	}

	for (auto const& it : mCloseSession)
	{
		mGameObjects.erase(it);
	}

	mCloseSession.clear();
}

void neo::server::ObjectManager::ObjectStart()
{
	for (auto const& it : mGameObjects)
	{
		it.second->Start();
	}
}

void neo::server::ObjectManager::ObjectEnd()
{
	for (auto const& it : mGameObjects)
	{
		it.second->End();
	}
}

void neo::server::ObjectManager::RegisterObject(const wstring& name,  std::shared_ptr<object::GameObject>& gameObject)
{
	if (mGameObjects.find(name) == mGameObjects.end())
	{
		mGameObjects.insert({ name, gameObject });
		auto gameObject = mGameObjects[name];
		gameObject->Name = name;
		gameObject->Session = nullptr;
		gameObject->Manager = weak_from_this();

		//gameObject Start
		gameObject->Start();
	}
}

void neo::server::ObjectManager::RegisterObject(const wstring& name, std::shared_ptr<object::GameObject>& gameObject, neo::server::Session* session)
{
	if (mGameObjects.find(name) == mGameObjects.end())
	{
		mGameObjects.insert({ name, gameObject });
		auto gameObject = mGameObjects[name];
		gameObject->Name = name;
		gameObject->Session = session;
		gameObject->Manager = weak_from_this();

		//gameObject Start
		gameObject->Start();
	}
}

void neo::server::ObjectManager::RegisterObject(const wstring& name)
{
	if (mGameObjects.find(name) == mGameObjects.end())
	{
		mGameObjects.insert({ name, std::make_shared<object::GameObject>() });
		auto gameObject = mGameObjects[name];
		gameObject->Name = name;
		gameObject->Session = nullptr;
		gameObject->Manager = weak_from_this();

		//gameObject Start
		gameObject->Start();
	}
}

void neo::server::ObjectManager::RegisterObject(const wstring& name, neo::server::Session* session)
{
	if (mGameObjects.find(name) == mGameObjects.end())
	{
		mGameObjects.insert({ name, std::make_shared<object::GameObject>() });
		auto gameObject = mGameObjects[name];
		gameObject->Name = name;
		gameObject->Session = session;
		gameObject->Manager = weak_from_this();

		//gameObject Start
		gameObject->Start();
	}
}

void neo::server::ObjectManager::UnregisterObject(const wstring& name)
{
	if (mGameObjects.find(name) != mGameObjects.end())
	{
		//mGameObjects.erase(name);
		mCloseSession.push_back(name);
	}
}

weak_ptr<neo::object::GameObject> neo::server::ObjectManager::GetGameObject(const wstring& name)
{
	if (mGameObjects.find(name) != mGameObjects.end())
	{
		return weak_ptr<object::GameObject>(mGameObjects[name]);
	}
	else
		return weak_ptr<object::GameObject>();
}

nlohmann::json neo::server::ObjectManager::JsonGetAllObjectName()
{
	nlohmann::json root;
	nlohmann::json array;
	for (auto const& object : mGameObjects)
	{
		string str = std::string().assign(object.second->Name.begin(), object.second->Name.end());
		array.push_back(std::move(str));
	}
	root["array"] = array;
	return root;
}

nlohmann::json neo::server::ObjectManager::JsonGetAllObjectName(const wstring& name)
{
	nlohmann::json root;
	nlohmann::json array;
	for (auto const& object : mGameObjects)
	{
		if (name == object.second->Name)
			continue;
		nlohmann::json obj;
		obj["name"] = std::string().assign(object.second->Name.begin(), object.second->Name.end());
		array.push_back(std::move(obj));
	}
	root["array"] = array;
	return root;
}
