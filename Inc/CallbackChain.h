/*
	This is a simple template that handles callback chains
	Revision:
		Christopher Békési 2020-06-12
*/
#pragma once

MAGICGUIAPI void* AllocStaticContainer();
MAGICGUIAPI void* AllocStaticClassContainer();

class Widget;

struct AClassInfo
{
	Widget* Owner;
	void(*Callback)() ;
};

template<typename... Args> class Callbacks
{
public:
	typedef void (*ACallback)(Args...);
	typedef void (Widget::*ClassCallback)(Args...);

	

	struct CallbackStruct
	{
		CallbackStruct* Next;
		ACallback Delegate;
	} *CallbackList = 0, *Tail = 0;

	struct ClassCallbackStruct
	{
		ClassCallbackStruct* Next;
		AClassInfo Delegate;
	} *ClassCallbackList = 0, *CTail = 0;



	void DelegateCallbacks(Args... args)
	{
		for (CallbackStruct* Ls = CallbackList; Ls; Ls = Ls->Next)
			Ls->Delegate(args...);

		for (ClassCallbackStruct* Ls = ClassCallbackList; Ls; Ls = Ls->Next)
		{
			ClassCallback CB = reinterpret_cast<ClassCallback &>(Ls->Delegate.Callback);
			(Ls->Delegate.Owner ->* CB)(args...);
		}
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
		Tail->Next = nullptr;
	}

	void operator += (AClassInfo CB)
	{
		if(!ClassCallbackList)
			ClassCallbackList = CTail = (ClassCallbackStruct*)AllocStaticClassContainer();
		else
		{
			CTail->Next = (ClassCallbackStruct*)AllocStaticClassContainer();
			CTail = CTail->Next;
		}
		CTail->Delegate = CB;
		CTail->Next = nullptr;
	}
	
};

//TODO: This needs to be changed!
inline AClassInfo ClassDelegate(void * ClassCallback, Widget* Owner)
{
	return { Owner, reinterpret_cast<void(*)()>(ClassCallback) };
}