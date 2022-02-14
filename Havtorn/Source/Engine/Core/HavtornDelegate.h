// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Delegate.h"

namespace Havtorn
{
	template<typename... BroadcastTypes>
	class CMulticastDelegate
	{
	public:
		CMulticastDelegate()
			: Delegate(MulticastDelegate<BroadcastTypes...>())
		{}

		~CMulticastDelegate() = default;
		CMulticastDelegate(const CMulticastDelegate&) = default;
		CMulticastDelegate(CMulticastDelegate&&) = default;
		CMulticastDelegate& operator=(const CMulticastDelegate&) = default;
		CMulticastDelegate& operator=(CMulticastDelegate&&) = default;

		template<typename LambdaType, typename... LambdaArgs>
		void AddLambda(LambdaType&& lambda, LambdaArgs&&... args)
		{
			Delegate.AddLambda(std::forward<LambdaType>(lambda), std::forward<LambdaArgs>(args)...);
		}

		template<typename ObjectType, typename FunctionReturnType>
		void AddMember(const ObjectType* object, FunctionReturnType(ObjectType::* function)(BroadcastTypes...) const)
		{
			Delegate.AddRaw(object, function);
		}

		template<typename ObjectType, typename FunctionReturnType>
		void AddMember(ObjectType* object, FunctionReturnType(ObjectType::* function)(BroadcastTypes...))
		{
			Delegate.AddRaw(object, function);
		}

		template<typename FunctionType, typename... FunctionArgs>
		void AddStatic(FunctionType&& function, FunctionArgs&&... args)
		{
			Delegate.AddStatic(std::forward<FunctionType>(function), std::forward<FunctionArgs>(args)...);
		}

		void Broadcast(BroadcastTypes... args)
		{
			Delegate.Broadcast(std::forward<BroadcastTypes>(args)...);
		}

	public:
		MulticastDelegate<BroadcastTypes...> Delegate;
	};
}