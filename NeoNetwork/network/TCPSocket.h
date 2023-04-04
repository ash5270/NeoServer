//TCP 家南 包府
#include"SocketCommon.h"
#include"SocketAddress.h"
namespace neo::network
{
	class TCPSocket {
	public:
		~TCPSocket();
		TCPSocket();

		int CreateSocket();
		int Connect(const SocketAddress& socketAddress);
		int Bind(const SocketAddress& socketAddress);

		TCPSocket* Accept(SocketAddress& fromAddress); 
		TCPSocket* AcceptEX(PVOID outputBuffer, LPOVERLAPPED overlapped);

		int Listen(int backLog = SOMAXCONN);
		int Send(const char* buffer, int len);
		int Recv(char* buffer, int len);
		int SetNoDelay(const bool& setOption);

		//WSA function 
		int WSARecv(LPWSABUF lpBuffes, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL);
		int WSASend(LPWSABUF lpBuffes, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL);

		SOCKET GetSOCKET() {
			return mSocket;
		}

	private:
		TCPSocket(const SOCKET& socket) : mSocket(socket) {};
		SOCKET mSocket;
	};
}