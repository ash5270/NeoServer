#include "TCPSocket.h"
neo::network::TCPSocket::~TCPSocket()
{
    closesocket(mSocket);
}

neo::network::TCPSocket::TCPSocket()
{
}

int neo::network::TCPSocket::CreateSocket()
{
    mSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    return mSocket;
}

int neo::network::TCPSocket::Connect(const SocketAddress& socketAddress)
{
    int result = ::connect(mSocket, socketAddress.GetSockAddr(), socketAddress.GetSize());
    return result;
}

int neo::network::TCPSocket::Bind(const SocketAddress& socketAddress)
{
    int result = ::bind(mSocket, &socketAddress.mSockAddr, socketAddress.GetSize());
    return result;
}

neo::network::TCPSocket* neo::network::TCPSocket::Accept(SocketAddress& fromAddress)
{
    int len = fromAddress.GetSize();
    SOCKET newClient = ::accept(mSocket, &fromAddress.mSockAddr, &len);
    if (newClient != INVALID_SOCKET)
    {
        return new TCPSocket(newClient);
    }

    return nullptr;
}

neo::network::TCPSocket* neo::network::TCPSocket::AcceptEX(PVOID outputBuffer, LPOVERLAPPED overlapped)
{
    DWORD dwBytes;
    TCPSocket* socket = new TCPSocket();
    socket->CreateSocket();
    socket->SetNoDelay(true);
    int result = 0; 
    result= ::AcceptEx(this->mSocket,socket->GetSOCKET(),
        outputBuffer,
        0,
        sizeof(sockaddr_in) + 16,
        sizeof(sockaddr_in) + 16,
        &dwBytes,
        overlapped);
    if (!result && (WSAGetLastError() != WSA_IO_PENDING))
    {
        return nullptr;
    }
    return socket;
}

int neo::network::TCPSocket::Listen(int backLog)
{
    int result = ::listen(mSocket, backLog);
    return result;
}

int neo::network::TCPSocket::Send(const char* buffer, int len)
{
    int tranferSize = ::send(mSocket, buffer, len,0);
    if (tranferSize >= 0)
    {
        return tranferSize;
    }
    return GetLastError();
}

int neo::network::TCPSocket::Recv(char* buffer, int len)
{
    int tranferSize = ::recv(mSocket, buffer, len, 0);
    if (tranferSize >= 0)
        return tranferSize;
    return GetLastError();
}

int neo::network::TCPSocket::SetNoDelay(const bool& setOption)
{
   
    const int option = static_cast<int>(setOption);
    int result = setsockopt(mSocket, IPPROTO_TCP,
        TCP_NODELAY,
        reinterpret_cast<const char*>(&option),
        sizeof(option));
    if (result != 0)
        return false;
    return true;
}

int neo::network::TCPSocket::WSARecv(LPWSABUF lpBuffes, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    DWORD flags = 0;
    DWORD len = 0;
    return ::WSARecv(mSocket, lpBuffes, dwBufferCount, NULL, &flags, lpOverlapped, lpCompletionRoutine);
}

int neo::network::TCPSocket::WSASend(LPWSABUF lpBuffes, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    DWORD flags = 0;
    DWORD len = 0;
    return ::WSASend(mSocket, lpBuffes, dwBufferCount, NULL, flags, lpOverlapped, lpCompletionRoutine);
}
