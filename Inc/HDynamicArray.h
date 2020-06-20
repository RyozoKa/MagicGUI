
/*
	Class wrapper for a dynamic array, usable across the OS-native environment and the Engine-native environment.

	Revision:
		-Christopher Békési 17-04-2018 Initial build
*/
#pragma once
#include <string.h>

template<class T> class HDynamicArray
{
private:
	T * _Data;
	UINT64 _Size;
	UINT64 _Alloc;

public:

	//Constructors
	HDynamicArray() : _Size(0), _Alloc(16)
	{
		_Data = new T[_Alloc];
	}
	HDynamicArray(const UINT64 Alloc) : _Size(0), _Alloc(Alloc)
	{
		_Data = new T[Alloc];
	}
	//Destructors
	~HDynamicArray()
	{
		delete[] _Data;
	}

	void Insert(T& D)
	{
		//Do we need to reallocate?
		if (_Size >= _Alloc)
		{
			_Alloc <<= 1;
			T* New = new T[_Alloc];
			memcpy(New, _Data, _Size * sizeof(T));
			delete[] _Data;
			_Data = New;
		}
		_Data[_Size++] = D;
	}

	void Insert(T&& D)
	{
		//Do we need to reallocate?
		if (_Size >= _Alloc)
		{
			_Alloc <<= 1;
			T* New = new T[_Alloc];
			memcpy(New, _Data, _Size * sizeof(T));
			delete[] _Data;
			_Data = New;
		}
		_Data[_Size++] = D;
	}

	T& operator[](UINT64 i)
	{
		return _Data[i];
	}

	UINT64 Size()
	{
		return _Size;
	}

	void Clear()
	{
		if (_Alloc > 16)
		{
			_Alloc = 16;
			delete[] _Data;
			_Data = new T[_Alloc];
		}
		_Size = 0;
	}

	void Erase(UINT64 index, UINT64 count = 0)
	{
		if (!count)	//No need to actually move anything.
		{
			_Size = index - 1;
			return;
		}
		//	---
		//	---
		//	---
		//	---
		//	---		<---	index 4, count = 3
		//	---
		//	---		<--- 
		//	---		<--- 4 + 3; move to 4
		//	---

		for (int i = index; (i - count) < _Size; ++i)
		{
			_Data[i] = std::move(_Data[i + count]);
		}
		_Size -= count;
	}

	T* Data()
	{
		return _Data;
	}
};