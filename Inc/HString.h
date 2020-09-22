/*
	WString class
	16bit characters
	Revision:
		Christopher Békési - Initial build 2015-06-21
*/
#pragma once

#include <string.h>
#include <cstring>

//String stack functions
MAGICGUIAPI char* NewCharStr();
MAGICGUIAPI wchar_t* NewWCharStr();
MAGICGUIAPI void FreeCharStr(char* str);
MAGICGUIAPI void FreeWCharStr(wchar_t* str);
void AppToLower(wchar_t* w); //A-Z
void AppToLower(char* w);
void AppToUpper(wchar_t* w); //A-Z
void AppToUpper(char* w);
void AppCheckAlpha(const wchar_t* s);
void AppCheckAlpha(const char* s);
bool AppIsAlpha(const wchar_t* s);
bool AppIsAlpha(const char* s);

/*
	UTF-8 and UTF-16 string classes, they can be assigned to eachother interchangebly.
*/
class MAGICGUIAPI WString
{
public:
	friend String;

	 WString(void);
	 template<std::size_t N>
	 WString(const wchar_t(&a)[N])
	 {
		 nstring = a;
		 nlength = N;
	 }
	 WString(const wchar_t *str,int len = 0);
	 WString(const char *str,int len = 0);
	 WString(const wchar_t str);
	 WString(const WString& in);
	 WString(WString&& in);	//Move constructor
	 WString(const String& in);
	 WString(BYTE*& bytes);//for a bytearray
	 WString(FileReader*);
	 ~WString(void);
	 operator const wchar_t*()
	 {
		 return nstring;
	 }
	 void						Deserialize(BYTE**);
	 void						Deserialize(FileReader*);
	 void						Serialize(BYTE**);
	 void						Serialize(FileWriter *);
	 HDynamicArray<WString>		Split(const wchar_t* str) const;
	 HDynamicArray<HASH>		SplitHash(const wchar_t* str) const;
	 bool						ReplaceAt(const wchar_t* str,UINT start,UINT count);
	 bool						ReplaceWith(const wchar_t* str,const wchar_t* with);
	 void						ReplaceAll(const wchar_t str, const wchar_t with);
	 void						Trim();
	 void						Chop(int num);
	 void						ChopStart(int num);
	 INT						Count(const wchar_t ch) const;
	 int						Length() const;
inline const wchar_t&			At(int pos) const { return nstring[pos]; }
	 bool						IsEmpty() const;
inline wchar_t*					Towchar() const { return nstring; }
	 char*						Tochar() const;
	 WString*					ToLower() const;
	 void						Dolower();
	 void						Tobytes(BYTE*& b) const;
	 void						ToFile(FileWriter*);
	 void						Clear();
	 WString*					Toupper() const;
	 void						Doupper();
	 void						Swap(WString& from);
	 WString					Substr(int start, int num=0) const;
	 void						Detach(int start, int end=0);
	 int						Find(const wchar_t*,int index=0) const;//Returns -1 if nothing found
	 int						Findlast(const wchar_t*, int index=0) const;
	 int						FindExact(const wchar_t*,int index=0) const;
	 bool						BeginsWith(const wchar_t*) const;
	 bool						EndsWith(const wchar_t*) const;
	 wchar_t&					operator [] (int pos)
	 {
		 return nstring[pos];
	 }
	 friend						std::wostream& operator << (std::wostream & OS, WString *S);
	 friend						std::wostream& operator << (std::wostream & OS, WString S);
	 friend						std::wistream& operator >> (std::wistream & IS, WString *S);
	 WString&					Append(const wchar_t* in, int len = 0);
	 WString&					Append(const WString& in);
	 WString&					Append(const wchar_t in);
	 WString&					AppendStart(const wchar_t*);
	 WString&					operator = (const WString& S);
	 WString&					operator = (const String& S);
	 WString&					operator = (WString&& S);
	 WString&					operator = (const wchar_t* S);
	 WString&					operator = (const INT S);
	 WString&					operator = (const UINT S);
	 WString&					operator = (const INT64 S);
	 WString&					operator = (const UINT64 S);
	 WString&					operator = (const SHORT S);
	 WString&					operator = (const USHORT S);
	 WString&					operator = (const DOUBLE S);
	 WString&					operator = (const FLOAT S);
	 WString&					operator = (const BYTE S);
	 WString&					operator = (const wchar_t S);
	MAGICGUIAPI friend  bool		operator ==(const WString&, const WString&);
	MAGICGUIAPI friend  bool		operator |=(const WString&, const WString&);
	MAGICGUIAPI friend  bool		operator |=(const WString&, const wchar_t*);
	MAGICGUIAPI friend  bool		operator ==(const WString&, const wchar_t*);
	MAGICGUIAPI friend  bool		operator !=(const WString&, const WString&);
	MAGICGUIAPI friend  bool		operator !=(const WString&, const wchar_t*);
	MAGICGUIAPI friend WString	operator + (const WString&, WString &);
	MAGICGUIAPI friend WString	operator + (const WString&, const wchar_t*);
	WString* operator+= (const WString& S1);
	WString* operator+= (const wchar_t* S1);
	WString* operator+= (UINT64 S1);
	WString* operator+= (INT64 S1);
	WString* operator+= (UINT S1);
	WString* operator+= (INT S1);
	WString* operator+= (USHORT S1);
	WString* operator+= (SHORT S1);
	WString* operator+= (BYTE S1);
	WString* operator+= (float S1);
	WString* operator+= (double S1);
	WString* operator-= (const wchar_t* s1);

	wchar_t* nstring;
	USHORT nlength;
	unsigned char DecimalPlaces = 6;
};

class MAGICGUIAPI String
{
public:
	friend WString;

	String(void);
	String(const wchar_t *str, int len = 0);
	String(const char *str, int len = 0);
	String(const String& in);
	String(const WString& in);
	String(const char s)
	{
		nstring = NewCharStr();
		*nstring = s;
		nlength = 2;
	}
	String(String&&);
	String(String&);
	String(BYTE*& bytes);//for a bytearray
	String(FileReader*);

	//Initialize a constant string literal
	//Really needs to be supported by the compiler so that it preallocates the string size
	template<std::size_t N>
	String(const char(&a)[N])
	{
		nstring = NewCharStr();
		memcpy(nstring, a, nlength);
		nlength = N;
	}
	~String(void);
	operator const char*()
	{
		return nstring;
	}
	void						Deserialize(BYTE**);
	void						Deserialize(FileReader*);
	void						Serialize(BYTE**);
	void						Serialize(FileWriter *);
	HDynamicArray<String>		Split(const char* str) const;
	HDynamicArray<String>		SplitCode(const char* str) const;
	HDynamicArray<HASH>		SplitHash(const char* str) const;
	bool						ReplaceAt(const char* str, UINT start, UINT count);
	bool						ReplaceWith(const char* str, const char* with);
	void						ReplaceAll(const char str, const char with);
	void						Trim();	//Trims off spaces and tab spaces from both ends of the string.
	void						Chop(int num);
	void						ChopStart(int num);
	INT							Count(const char ch) const;
	int							Length() const;
inline const char&				At(int pos) const { return nstring[pos]; }
	bool						IsEmpty() const;
	wchar_t*					Towchar() const;
inline char*					Tochar() const { return nstring; }
	String*						ToLower() const;
	void						Dolower();
	void						Tobytes(BYTE*& b) const;
	void						ToFile(FileWriter*);
	void						Clear();
	String*						Toupper() const;
	void						Doupper();
	void						Swap(String& from);
	String						Substr(int start, int num = 0) const;
	void						Detach(int start, int end=0);
	int							Find(const char*, int index = 0) const;//Returns -1 if nothing found
	int							Findlast(const char*, int index = 0) const;
	int							FindExact(const char*, int index = 0) const;
	bool						BeginsWith(const char*) const;
	bool						EndsWith(const char*) const;
	char&						operator [] (int pos)
	{
		return nstring[pos];
	}
	friend						std::ostream& operator << (std::ostream & OS, String *S);
	friend						std::ostream& operator << (std::ostream & OS, String S);
	friend						std::istream& operator >> (std::istream & IS, String *S);
	String&					Append(const char* in, int len = 0);
	String&					Append(const String& in);
	String&					Append(const char);
	String&					AppendStart(const char*);
	String&					operator = (const String& S);
	String&					operator = (String&& S);
	String&					operator = (const WString& S);
	String&					operator = (const char* S);
	String&					operator = (const INT S);
	String&					operator = (const UINT S);
	String&					operator = (const INT64 S);
	String&					operator = (const UINT64 S);
	String&					operator = (const SHORT S);
	String&					operator = (const USHORT S);
	String&					operator = (const DOUBLE S);
	String&					operator = (const FLOAT S);
	String&					operator = (const BYTE S);
	String&					operator = (const char S);
	friend  bool	operator ==(const String&, const String&);
	friend  bool	operator |=(const String&, const String&);
	friend  bool	operator |=(const String&, const char*);
	friend  bool	operator ==(const String&, const char*);
	friend  bool	operator !=(const String&, const String&);
	friend  bool	operator !=(const String&, const char*);
	friend String	operator + (const String&, const String &);
	friend String	operator + (const String&, const char*);
	String& operator+= (const String& S1);
	String& operator+= (const char* S1);
	String& operator+= (const UINT64 S1);
	String& operator+= (const INT64 S1);
	String& operator+= (const UINT S1);
	String& operator+= (const INT S1);
	String& operator+= (const USHORT S1);
	String& operator+= (const SHORT S1);
	String& operator+= (const BYTE S1);
	String& operator+= (const float S1);
	String& operator+= (const double S1);
	String& operator+= (const char S1);
	String& operator-= (const char* s1);

	char* nstring;
	USHORT nlength;
	unsigned char DecimalPlaces = 6;
};

//Primitive string functions
 bool appwcscmp_ci(const wchar_t*,const wchar_t*);
 bool appstrcmp_ci(const char*, const char*);

 //a ends with b
 bool appstrend(const char* a, const char* b);

 //Returns true if string contains char
 bool appcntstr(const char*, const char);

/*
THE END
*/