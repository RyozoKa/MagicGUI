#include "CallbackChain.h"

void* AllocStaticContainer()
{
	static char Block[1024 * (sizeof(void*) * 2)];
	static int Idx = 0;
	void* Res = (void*)(&Block[Idx]);
	Idx += sizeof(void*) * 2;
	return Res;
}
