#include<network/IOCPClient.h>

using namespace neo::network;
using namespace neo;

int main()
{
	IOCPSession* session= new IOCPSession();
	IOCPClient client("127.0.0.1", 45699);
	client.InitializeClient(session);

	while(true)
	{
		
	}
	
	return 0;
}