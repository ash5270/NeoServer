#include "DataBaseManager.h"

void neo::db::DataBaseManager::Init()
{
	mConntion = mysql_init(0);
	mConntion= mysql_real_connect(mConntion, MYSQL_IP.c_str(), MYSQL_ID.c_str(), MYSQL_PASSWORD.c_str(), MYSQL_DB.c_str(), MYSQL_PORT, NULL, 0);
}

void neo::db::DataBaseManager::Stop()
{
	mysql_close(mConntion);
}

MYSQL* neo::db::DataBaseManager::GetNewConnection()
{
	auto connection = mysql_init(0);
	connection = mysql_real_connect( connection, MYSQL_IP.c_str(), MYSQL_ID.c_str(), MYSQL_PASSWORD.c_str(), MYSQL_DB.c_str(), MYSQL_PORT, NULL, 0);
	return connection;
}

MYSQL* neo::db::DataBaseManager::GetConnection()
{
	return mConntion;
}

