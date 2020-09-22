/*
	Basic templates

	Revision:
	2020-03-10 Christopher Békési
*/
#pragma once

#ifdef BUILD
	#define MAGICGUIAPI	__attribute__((dllexport))
#else
	#define MAGICGUIAPI __attribute__((dllimport))
#endif

typedef unsigned char		BYTE;
typedef unsigned int		UINT;
typedef float				FLOAT;
typedef double				DOUBLE;
typedef int					INT;
typedef unsigned long long	UINT64;
typedef long long			INT64;
typedef unsigned short USHORT;
typedef short			SHORT;
#ifndef _WIN32
typedef long				INT64;
typedef unsigned long		UNIT64;
#endif

#include <math.h>
#include <vector>
#include "fasttrigo.h"

#include <cstdio>

//Open GL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "HDynamicArray.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

void ErrorMessage(const char* Msg, const char* Title);	//Simple, crossplatform implementation of showing error messages during startup.

//GLFW overrides.
struct GLFWwindow
{

	//Custom impl
	class Window* Wnd;

	//We don't care about the other members for now. The structure will still align accordingly.
};

#include "CallbackChain.h"
#include "Shader.h"

inline long long Abs(long long num)
{
	return num > 0 ? num : -num;
}

inline float FAbs(float f)
{
  (*reinterpret_cast<unsigned int*>(&f)) &= 0x7fffffff;
  return f;
}

//Euiler angles
struct Rotator
{
	float Rot = 0;

	Rotator() = default;
	Rotator(float _Rot)
	{
		Rot = _Rot;
	}

	Rotator operator +(const Rotator& R2) const
	{
		return { Rot + R2.Rot };
	}

	Rotator& operator += (const Rotator& R2)
	{
		Rot += R2.Rot;
		return *this;
	}

	Rotator& operator += (const float Ang)
	{
		Rot += Ang;
		return *this;
	}

	Rotator& operator = (const float F)
	{
		Rot = F;
		return *this;
	}

	Rotator operator -(const Rotator&R2) const
	{
		return { Rot - R2.Rot };
	}

	Rotator operator - (const float Ang) const
	{
		return { Rot - Ang };
	}
};

// 2x2 matrix
struct Mat2
{
	float M[2][2];
	Rotator Rot;
	Mat2(const Rotator& R)
	{
		float fsin = FT::sin(R.Rot * 0.0174532925f);
		float fcos = FT::cos(R.Rot * 0.0174532925f);
		M[0][0] = fcos;
		M[1][0] = -fsin;
		M[0][1] = fsin;
		M[1][1] = fcos;
		Rot = R;
	}
	Mat2()
	{
		M[0][0] = 1.f;
		M[1][0] = 0.f;
		M[0][1] = 0.f;
		M[1][1] = 1.f;
	}
};

struct Vect2
{	
	float X;
	float Y;

	Vect2(int X, int Y) : X(X), Y(Y)
	{}

	Vect2(float X, float Y) : X(X), Y(Y)
	{}

	Vect2() : X(0.f), Y(0.f)
	{}

	Vect2(const Rotator R)
	{
		//Normalized
		X = 1.f;
		Y = 1.f;

		float RSin = FT::sin(R.Rot * 0.0174532925f);
		float RCos = FT::cos(R.Rot * 0.0174532925f);
		X = (X * RCos) - (Y * RSin);
		Y = (X * RSin) + (Y * RCos);
	}

	void Rotate(const float Deg)
	{
		float RSin = FT::sin(Deg * 0.0174532925f);
		float RCos = FT::cos(Deg * 0.0174532925f);
		float X2 = (X * RCos) - (Y * RSin);
		float Y2 = (X * RSin) + (Y * RCos);
		X = X2;
		Y = Y2;
	}

	void Rotate(const Rotator R)
	{
		float RSin = FT::sin(R.Rot * 0.0174532925f);
		float RCos = FT::cos(R.Rot * 0.0174532925f);
		X = (X * RCos) - (Y * RSin);
		Y = (X * RSin) + (Y * RCos);
	}

	Vect2& Negate()
	{
		X = -X;
		Y = -Y;
		return *this;
	}

	Vect2 operator+(const Vect2& S) const
	{
		return { X + S.X, Y + S.Y  };
	}

	Vect2 operator-(const Vect2& S) const
	{
		return { X - S.X, Y - S.Y };
	}

	Vect2 operator+(const float F) const
	{
		return { X + F, Y + F };
	}

	Vect2 operator-(const float F) const
	{
		return { X - F, Y - F };
	}

	Vect2& ToAbs()
	{
		X = FAbs(X);
		Y = FAbs(Y);
		return *this;
	}

	void Zero()
	{
		X = 0.f;
		Y = 0.f;
	}

	//auto operator<=>(const Vect2&) const = default;	//C++20 compliant

	friend bool operator>(const Vect2&, const Vect2&);

	friend bool operator<(const Vect2&, const Vect2&);

	friend bool operator>=(const Vect2&, const Vect2&);

	friend bool operator<=(const Vect2&, const Vect2&);

	friend bool operator>(const Vect2&, const float);

	friend bool operator<(const Vect2&, const float);

	friend bool operator>=(const Vect2&, const float);

	friend bool operator<=(const Vect2&, const float);

	friend bool operator != (const Vect2&, const Vect2&);
	friend bool operator != (const Vect2&, const float);
	friend bool operator == (const Vect2&, const float);
	friend bool operator == (const Vect2&, const Vect2&);



	Vect2 operator/(const Vect2& S)
	{
		return { X / S.X, Y / S.Y };
	}
	Vect2 operator*(const Vect2& S)
	{
		return { S.X * X, S.Y * Y };
	}

	Vect2 operator*(const float S)
	{
		return { S * X, S * Y };
	}

	Vect2 operator/(const float S)
	{
		return { X / S, Y / S };
	}

	Vect2& operator+=(const Vect2 S)
	{
		X += S.X;
		Y += S.Y;
		return *this;
	}

	Vect2& operator-=(const Vect2 S)
	{
		X -= S.X;
		Y -= S.Y;
		return *this;
	}

	Vect2& operator+=(const float F)
	{
		X += F;
		Y += F;
		return *this;
	}

	Vect2& operator-=(const float F)
	{
		X -= F;
		Y -= F;
		return *this;
	}

	Vect2& operator *=(const float F)
	{
		X *= F;
		Y *= F;
		return *this;
	}

	Vect2& operator *=(const Vect2& F)
	{
		X *= F.X;
		Y *= F.Y;
		return *this;
	}

	//We'll never have larger values. These conversions are super fast.
	void RoundUp()
	{
		X = (float)(int) 32768 - (int)(32768. - X);
		Y = (float)(int) 32768 - (int)(32768. - Y);
	}

	void RoundDown()
	{
		X = (float)(int)(X + 32768.) - 32768;
		Y = (float)(int)(Y + 32768.) - 32768;
	}

	float VSize()
	{
		return sqrt((X * X) + (Y * Y));
	}

	//Block math helper functions

	//Gets the block vector in which this vector belongs in
	Vect2 GetBlock() const
	{
		//The assembly output of this is faster than using float only.
		//This is a super efficient way to get the position of the current box, simply chop off the last bits.
		unsigned int Xb = X, Yb = Y;
		Xb &= ~(63);
		Yb &= ~(63);
		return Vect2((float)Xb, (float)Yb);
	}

	Vect2 GetBlockDelta() const
	{
		//This calculates the delta between the position and the upper left corner of the block it belongs to. Can also be seen as an offset
		unsigned int Xb = X, Yb = Y;
		Xb &= (63);
		Yb &= (63);
		return Vect2((float)Xb, (float)Yb);
	}

	//Return smallest vector
	static const Vect2& Min(const Vect2& V1, const Vect2& V2)
	{
		if (V1 < V2)
			return V1;
		return V2;
	}

	//Return largest vector
	static const Vect2& Max(const Vect2& V1, const Vect2& V2)
	{
		if (V1 > V2)
			return V1;
		return V2;
	}

	bool IsEmpty()
	{
		return X == 0.f && Y == 0.f;
	}
};

inline bool operator>(const Vect2& B, const Vect2& A)
{
	return B.X > A.X || B.Y > A.Y;
}

inline bool operator<(const Vect2& B, const Vect2& A)
{
	return B.X < A.X || B.Y < A.Y;
}

inline bool operator>=(const Vect2& B, const Vect2& A)
{
	return B.X >= A.X || B.Y >= A.Y;
}

inline bool operator<=(const Vect2& B, const Vect2& A)
{
	return B.X <= A.X || B.Y <= A.Y;
}

inline bool operator>(const Vect2& B, const float A)
{
	return B.X > A || B.Y > A;
}

inline bool operator<(const Vect2& B, const float A)
{
	return B.X < A || B.Y < A;
}

inline bool operator>=(const Vect2& B, const float A)
{
	return B.X >= A || B.Y >= A;
}

inline bool operator<=(const Vect2& B, const float A)
{
	return B.X <= A || B.Y <= A;
}

inline bool operator !=(const Vect2& A, const Vect2& B)
{
	return A.X != B.X || A.Y != B.Y;
}

inline bool operator !=(const Vect2& A, const float B)
{
	return A.X != B || A.Y != B;
}

inline bool operator ==(const Vect2& A, const Vect2& B)
{
	return A.X == B.X || A.Y == B.Y;
}

inline bool operator ==(const Vect2& A, const float B)
{
	return A.X == B || A.Y == B;
}

struct Vect3
{
	float X;
	float Y;
	float Z;
};

struct Vect4
{
	float X;
	float Y;
	float Z;
	float W;
};

struct Color
{
	unsigned char Red	= 0;
	unsigned char Green = 0;
	unsigned char Blue	= 0;
	unsigned char Alpha = 0;

	Color() = default;
	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255)
	{
		Red = R;
		Green = G;
		Blue = B;
		Alpha = A;
	}
};




//String hasing
typedef unsigned long long HASH;
#include <cstddef>



constexpr HASH fnv1a_64(char const* s, std::size_t count)
{
	return ((count ? fnv1a_64(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

constexpr HASH fnv1a_64(wchar_t const* s, std::size_t count)
{
	return ((count ? fnv1a_64(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

constexpr int c_strlen(const char* s)
{
	int res = 0;
	while (*s++) ++res;
	return res;
}

constexpr inline HASH HashName(const char* s)
{
	return fnv1a_64(s, c_strlen(s));
}

#pragma warning(disable:4455)
#pragma warning(disable:4307)
#pragma GCC diagnostic ignored "-Wliteral-suffix"
constexpr HASH operator "" Name(char const *s, std::size_t size)
{
	return fnv1a_64(s, size);
}
#pragma GCC diagnostic pop

class MAGICGUIAPI Application
{
public:

	//Driver data
	static bool bDriverInit;
	static Vect2 PhysicalSize;
	static unsigned short RefreshRate;
	static GLFWmonitor* primary;
	static Vect2 ScreenRes;

	static Callbacks<> SubsystemModules;


	static void Execute();
};



//Universal Keymap, as defined by GLFW

#define 	KEY_UNKNOWN   -1
#define 	KEY_SPACE   32
#define 	KEY_APOSTROPHE   39 /* ' */
#define 	KEY_COMMA   44 /* , */
#define 	KEY_MINUS   45 /* - */
#define 	KEY_PERIOD   46 /* . */
#define 	KEY_SLASH   47 /* / */
#define 	KEY_0   48
#define 	KEY_1   49
#define 	KEY_2   50
#define 	KEY_3   51
#define 	KEY_4   52
#define 	KEY_5   53
#define 	KEY_6   54
#define 	KEY_7   55
#define 	KEY_8   56
#define 	KEY_9   57
#define 	KEY_SEMICOLON   59 /* ; */
#define 	KEY_EQUAL   61 /* = */
#define 	KEY_A   65
#define 	KEY_B   66
#define 	KEY_C   67
#define 	KEY_D   68
#define 	KEY_E   69
#define 	KEY_F   70
#define 	KEY_G   71
#define 	KEY_H   72
#define 	KEY_I   73
#define 	KEY_J   74
#define 	KEY_K   75
#define 	KEY_L   76
#define 	KEY_M   77
#define 	KEY_N   78
#define 	KEY_O   79
#define 	KEY_P   80
#define 	KEY_Q   81
#define 	KEY_R   82
#define 	KEY_S   83
#define 	KEY_T   84
#define 	KEY_U   85
#define 	KEY_V   86
#define 	KEY_W   87
#define 	KEY_X   88
#define 	KEY_Y   89
#define 	KEY_Z   90
#define 	KEY_LEFT_BRACKET   91 /* [ */
#define 	KEY_BACKSLASH   92 /* \ */
#define 	KEY_RIGHT_BRACKET   93 /* ] */
#define 	KEY_GRAVE_ACCENT   96 /* ` */
#define 	KEY_WORLD_1   161 /* non-US #1 */
#define 	KEY_WORLD_2   162 /* non-US #2 */
#define 	KEY_ESCAPE   256
#define 	KEY_ENTER   257
#define 	KEY_TAB   258
#define 	KEY_BACKSPACE   259
#define 	KEY_INSERT   260
#define 	KEY_DELETE   261
#define 	KEY_RIGHT   262
#define 	KEY_LEFT   263
#define 	KEY_DOWN   264
#define 	KEY_UP   265
#define 	KEY_PAGE_UP   266
#define 	KEY_PAGE_DOWN   267
#define 	KEY_HOME   268
#define 	KEY_END   269
#define 	KEY_CAPS_LOCK   280
#define 	KEY_SCROLL_LOCK   281
#define 	KEY_NUM_LOCK   282
#define 	KEY_PRINT_SCREEN   283
#define 	KEY_PAUSE   284
#define 	KEY_F1   290
#define 	KEY_F2   291
#define 	KEY_F3   292
#define 	KEY_F4   293
#define 	KEY_F5   294
#define 	KEY_F6   295
#define 	KEY_F7   296
#define 	KEY_F8   297
#define 	KEY_F9   298
#define 	KEY_F10   299
#define 	KEY_F11   300
#define 	KEY_F12   301
#define 	KEY_F13   302
#define 	KEY_F14   303
#define 	KEY_F15   304
#define 	KEY_F16   305
#define 	KEY_F17   306
#define 	KEY_F18   307
#define 	KEY_F19   308
#define 	KEY_F20   309
#define 	KEY_F21   310
#define 	KEY_F22   311
#define 	KEY_F23   312
#define 	KEY_F24   313
#define 	KEY_F25   314
#define 	KEY_KP_0   320
#define 	KEY_KP_1   321
#define 	KEY_KP_2   322
#define 	KEY_KP_3   323
#define 	KEY_KP_4   324
#define 	KEY_KP_5   325
#define 	KEY_KP_6   326
#define 	KEY_KP_7   327
#define 	KEY_KP_8   328
#define 	KEY_KP_9   329
#define 	KEY_KP_DECIMAL   330
#define 	KEY_KP_DIVIDE   331
#define 	KEY_KP_MULTIPLY   332
#define 	KEY_KP_SUBTRACT   333
#define 	KEY_KP_ADD   334
#define 	KEY_KP_ENTER   335
#define 	KEY_KP_EQUAL   336
#define 	KEY_LEFT_SHIFT   340
#define 	KEY_LEFT_CONTROL   341
#define 	KEY_LEFT_ALT   342
#define 	KEY_LEFT_SUPER   343
#define 	KEY_RIGHT_SHIFT   344
#define 	KEY_RIGHT_CONTROL   345
#define 	KEY_RIGHT_ALT   346
#define 	KEY_RIGHT_SUPER   347
#define 	KEY_MENU   348
#define 	KEY_LAST   KEY_MENU

//Modifier Keys
#define MOD_ALT			0x0004
#define MOD_CONTROL		0x0002
#define MOD_SHIFT		0x0001
#define MOD_SUPER		0x0008

//Mouse buttons
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_MIDDLE 2
#define MOUSE_BUTTON_RIGHT 1

bool RectOverlap(Vect2 Pos1, Vect2 Size1, Vect2 Pos2, Vect2 Size2);
bool RectWithin(Vect2 InnerPos, Vect2 InnerSize, Vect2 OuterPos, Vect2 OuterSize);
//Forward declarations
template<class T> class HDynamicArray;
class PackageReader;
class PackageWriter;
class FileBase;
	class FileReader;
	class FileWriter;
class String;
class WString;

//Includes
#include <filesystem>
#include <emmintrin.h>
#include "google\dense_hash_map.h"
#include "google\dense_hash_set.h"
#include <array>
using namespace google;

#ifdef _WIN32
	#include "HWin32.h"
	#include <direct.h>

#else
	#include "HLinux.h"
#endif

#pragma warning(disable:63)
#pragma warning(disable:4293)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
#pragma warning(disable:4806)
#pragma warning(disable:4312)
#pragma warning(disable:4018)

#include "HString.h"

#include <iostream>

#include "HMath.h"
#include "HIO.h"
#include "HDynamicArray.h"

void ScissorBox(Vect2 Pos, Vect2 Size);
void DisableScissor();
//Add new stuff here