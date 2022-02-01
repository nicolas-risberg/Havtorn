// Copyright 2022 Team Havtorn. All Rights Reserved.

namespace Havtorn
{
	template<typename RetVal, typename... Args>
	class CInputDelegate
	{
	public:
		CInputDelegate()
		{
			Delegate = MulticastDelegate<RetVal, Args>();
		};
		~CInputDelegate() = default;

		template<typename TLambda>
		void AddLambda(TLambda&& lambda, Args&&... args) 
		{
			Delegate.AddLambda(std::forward<TLambda>(lambda), std::forward<Args>(args)...);
		}

		//template<typename TClass>
		//void AddMember(TClass& object, std::function& func, Args... args);

		//void AddStatic(std::function& func, Args... args);

		void Broadcast(Args&&... args) { Delegate.Broadcast(std::forward<Args>(args)...); }

	public:
		MulticastDelegate<RetVal, Args...> Delegate;
	};
}