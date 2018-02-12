#pragma once
#include <functional>

class TcpConnection;
typedef TcpConnection* TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void(const TcpConnectionPtr&, char*)> MessageCallback;