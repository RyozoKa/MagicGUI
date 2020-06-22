/*
	This is a simple template that handles callback chains
	Revision:
		Christopher Békési 2020-06-12
*/
#pragma once

MAGICGUIAPI void* AllocStaticContainer();

template<typename... Args> class Callbacks
{
public:
	typedef void (*ACallback)(Args...);

	struct CallbackStruct
	{
		CallbackStruct* Next;
		ACallback Delegate;
	} *CallbackList = 0, *Tail = 0;

	void DelegateCallbacks(Args... args)
	{
		for (CallbackStruct* Ls = CallbackList; Ls; Ls = Ls->Next)
			Ls->Delegate(args...);
	}

	void operator += (ACallback CB)
	{
		if(!CallbackList)
			CallbackList = Tail = (CallbackStruct*)AllocStaticContainer();
		else
		{
			Tail->Next = (CallbackStruct*)AllocStaticContainer();
			Tail = Tail->Next;
		}
		Tail->Delegate = CB;
	}
};