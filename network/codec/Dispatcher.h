#pragma once

#include "../net/TcpConnection.h"
#include "Codec.h"

class Callback
{
public:
	Callback() = default;
	virtual ~Callback() = default;
	Callback(const Callback&) = delete;
	Callback& operator = (const Callback&) = delete;

	virtual void onMessage(const TcpConnectionPtr& conn, const MessagePtr& message) const = 0;
};

template<typename T>
class CallbackT : public Callback
{
public:
	typedef std::function<void(const TcpConnectionPtr&, const std::shared_ptr<T>&)> ProtobufMessageTCallback;

	explicit CallbackT(const ProtobufMessageTCallback& cb)
		: callback_(cb)
	{
	}

	virtual void onMessage(const TcpConnectionPtr& conn, const MessagePtr& message) const
	{
		//std::shared_ptr<T> concreate(&google::protobuf::down_cast<T>(message));

		//callback_(conn, concreate);
	}

private:
	ProtobufMessageTCallback callback_;
};

class ProtobufDispatcher
{
public:
	ProtobufDispatcher() = default;
	~ProtobufDispatcher() = default;

	ProtobufDispatcher(const ProtobufDispatcher&) = delete;
	ProtobufDispatcher& operator = (const ProtobufDispatcher&) = delete;

	void OnMessage(const TcpConnectionPtr& conn, const MessagePtr& msg)
	{
		CallbackMap::const_iterator iter = callbacks_.find(msg->GetDescriptor());
		if (iter != callbacks_.end())
		{
			iter->second->onMessage(conn, msg);
		}
	}

	//需要记录类型与回调的绑定
	template<typename T>
	void RegisterCallback(const typename CallbackT<T>::ProtobufMessageTCallback& callback)
	{
		std::shared_ptr<CallbackT<T>> pd(new CallbackT<T>(callback));
		callbacks_[T::descriptor()] = pd;
	}

private:
	typedef std::map<const google::protobuf::Descriptor*, std::shared_ptr<Callback>> CallbackMap;

	CallbackMap callbacks_;
};
