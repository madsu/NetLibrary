#pragma once
#include <functional>

class TcpConnection;
class Buffer;
typedef TcpConnection* TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void(const TcpConnectionPtr&, Buffer*)> MessageCallback;