#include "Template.h"

void* AllocStaticContainer()
{
	static char Block[10240 * (sizeof(void*) * 2)];
	static int Idx = 0;
	void* Res = (void*)(&Block[Idx]);
	Idx += sizeof(void*) * 2;
	return Res;
}

void* AllocStaticClassContainer()
{
	static char Block[10240 * (sizeof(void*) * 4)];
	static int Idx = 0;
	void* Res = (void*)(&Block[Idx]);
	Idx += sizeof(void*) * 4;
	return Res;
}