/*	Global math functions.
	
*/

MAGICGUIAPI UINT64 appAbs(INT64 v);

MAGICGUIAPI void numDigits(UINT64 number, UINT64 *OutCount, UINT64 *OutFactor);

MAGICGUIAPI UINT64 RoundUp(UINT64 number, UINT64 multiple);

MAGICGUIAPI UINT RoundPow2Down(UINT x);

MAGICGUIAPI UINT64 RoundPow2Down(UINT64 x);


MAGICGUIAPI UINT64 StaticPow10(BYTE num);

MAGICGUIAPI INT64 ToNum(char*);

__m128 MD5(BYTE* b, unsigned int size);
