// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Delegate.h"

namespace Havtorn
{
	template<typename... BroadcastTypes>
	class CInputDelegate
	{
	public:
		CInputDelegate()
			: Delegate(MulticastDelegate<BroadcastTypes...>())
		{}

		~CInputDelegate() = default;
		CInputDelegate(const CInputDelegate&) = default;
		CInputDelegate(CInputDelegate&&) = default;
		CInputDelegate& operator=(const CInputDelegate&) = default;
		CInputDelegate& operator=(CInputDelegate&&) = default;

		template<typename LambdaType, typename... LambdaArgs>
		void AddLambda(LambdaType&& lambda, LambdaArgs&&... args)
		{
			Delegate.AddLambda(std::forward<LambdaType>(lambda), std::forward<LambdaArgs>(args)...);
		}

		//template<typename ObjectType, typename FunctionReturnType, typename... FunctionArgs>
		//void AddMember(const ObjectType* object, FunctionReturnType(ObjectType::* function)(FunctionArgs&&...) const, FunctionArgs&&... args)
		//{
		//	Delegate.AddRaw(object, function, std::forward<FunctionArgs>(args)...);
		//}

		template<typename ObjectType, typename FunctionReturnType>
		void AddMember(const ObjectType* object, FunctionReturnType(ObjectType::* function)(BroadcastTypes...) const)
		{
			Delegate.AddRaw(object, function);
		}

		//template<typename ObjectType, typename FunctionReturnType, typename... FunctionArgs>
		//void AddMember(ObjectType* object, FunctionReturnType(ObjectType::* function)(FunctionArgs&&...), FunctionArgs&&... args)
		//{
		//	Delegate.AddRaw(object, function, std::forward<FunctionArgs>(args)...);
		//}

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