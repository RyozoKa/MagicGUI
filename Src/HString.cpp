#include "Template.h"

/*
////////////////////////////////////////////////////////////
WString
////////////////////////////////////////////////////////////
*/

WString::WString(void) : nstring(NULL)
{
		nlength = 1;
		nstring = NewWCharStr();
}
int WString::Findlast(const wchar_t* in, int index) const
{
		int len = wcslen(in);
		int k = -1;
		for (int i = index; i<Length(); ++i)
		{
			for (int j = 0; j < len; ++j)
			{
				if (nstring[i] == in[j])
					k = i;
			}
		}
		return k;
}
WString::WString(BYTE*& bytes) : nstring(NULL)
{
		nlength = *(USHORT*)bytes;
		nstring = NewWCharStr();
		bytes += 2;
		memcpy(nstring, bytes, nlength * 2);
		bytes += nlength - 1;
}
WString::WString(FileReader* f) : nstring(NULL)
{
		nlength = f->Read<USHORT>();
		nstring = NewWCharStr();
		f->Read(nstring, nlength * 2);
}
void WString::Deserialize(BYTE** bytes)
{
		nlength = **(USHORT**)bytes;
		nstring = NewWCharStr();
		*bytes += 2;
		memcpy(nstring, *bytes, nlength * 2);
		*bytes += (nlength * 2) - 2;
}
void WString::Deserialize(FileReader* f)
{
		nlength = f->Read<USHORT>();
		nstring = NewWCharStr();
		f->Read(nstring, (nlength * 2) + 2);
}
void WString::Serialize(BYTE ** data)
{
	**(USHORT**)data = nlength - 1;
	*data += 2;
	wcscpy((wchar_t*)*data, nstring);
	*data += (nlength * 2) - 2;
}

void WString::Serialize(FileWriter *F)
{
	F->Write<USHORT>(nlength);
	F->Write(nstring, nlength * 2);
}

WString::WString(const wchar_t* in, int len) : nstring(NULL)
{
		nlength = len > 0 ? len +1 : wcslen(in)+1;
		nstring = NewWCharStr();
		memcpy(nstring, in, nlength*2);
		nstring[nlength-1]=L'\0';
}
WString::WString(const char* in, int len) : nstring(NULL)
{
		nlength = len > 0 ? len +1 : strlen(in)+1;
		nstring = NewWCharStr();
		for(int i=0;i<nlength-1;++i)
			nstring[i] = in[i];
		nstring[nlength-1]=L'\0';
}
WString::WString(const wchar_t in) : nstring(NULL)
{
		nlength = wcslen(&in)+1;
		nstring = NewWCharStr();
		memcpy(nstring,&in,nlength*2);
		nstring[nlength-1]=L'\0';
}
WString::WString(const WString& in) : nstring(NULL)
{
		nlength = in.nlength;
		nstring = NewWCharStr();
		memcpy(nstring, in.nstring, nlength*2);
}
WString::WString(WString&& in)
{
	nstring = in.nstring;
	nlength = in.nlength;
	in.nstring = NULL;
	in.nlength = 1;
}
WString::WString(const String& in)
{
	nstring = NewWCharStr();
	nlength = in.nlength;
	char* str = in.nstring;
	wchar_t* tstr = nstring;
	while (*str) *tstr++ = *str++;
	*tstr = L'\0';
}
WString::~WString()
{
	FreeWCharStr(nstring);
}
HDynamicArray<WString> WString::Split(const wchar_t *str) const
{
	
	HDynamicArray<WString> words;
	WString word;
	wchar_t last;
	if(IsEmpty()) return words;
	for(int i=0;i<=Length();++i)
	{
		/*if((at(i) == L'/' && at(i+1) == L'*'))
			bCommentBlock = true;
		else if(at(i) == L'/' && at(i+1) == L'/')
			bCommentLine = true;
		*///else if(bCommentBlock && (at(i) == L'*' && at(i+1) == L'/'))
		//{
		//	bCommentBlock = false;
		//	++i;
		//	continue;
		//}
		//else if(bCommentLine && at(i) == L'\n')
		//{
		//	bCommentLine = false;
		//	continue;
		//}
		//if(bCommentBlock || bCommentLine) continue;

		/*if(at(i) == ';' || at(i) == L'(' || at(i) == L'[' || at(i) == L']' || at(i) == L')' || at(i) == L'{' || at(i) == L'}' || at(i) == L',' || at(i) == L'.') //Special characters
		{
			if(!word->IsEmpty())
			{
				words.push_back((word));
				word = new WString();
			}
			words.push_back(new WString(L";"));
			continue;
		}*/
		if(!word.IsEmpty() && (At(i) == str[0] || (At(i) == L'\n' && last != L'\n')))
		{
			words.Insert( WString(word) );
			word.Clear();
		}
		else if(At(i) != L'\n' && At(i) != L'\0')
			word.Append(At(i));
		last = At(i);
	}
	if(!word.IsEmpty())
		words.Insert(word);//Last word might be missing
	return words;
	
}
HDynamicArray<HASH> WString::SplitHash(const wchar_t *str) const
{

	HDynamicArray<HASH> words;
	WString word;
	wchar_t last;
	if (IsEmpty()) return words;
	for (int i = 0; i <= Length(); ++i)
	{
		/*if((at(i) == L'/' && at(i+1) == L'*'))
		bCommentBlock = true;
		else if(at(i) == L'/' && at(i+1) == L'/')
		bCommentLine = true;
		*///else if(bCommentBlock && (at(i) == L'*' && at(i+1) == L'/'))
		//{
		//	bCommentBlock = false;
		//	++i;
		//	continue;
		//}
		//else if(bCommentLine && at(i) == L'\n')
		//{
		//	bCommentLine = false;
		//	continue;
		//}
		//if(bCommentBlock || bCommentLine) continue;

		/*if(at(i) == ';' || at(i) == L'(' || at(i) == L'[' || at(i) == L']' || at(i) == L')' || at(i) == L'{' || at(i) == L'}' || at(i) == L',' || at(i) == L'.') //Special characters
		{
		if(!word->IsEmpty())
		{
		words.push_back((word));
		word = new WString();
		}
		words.push_back(new WString(L";"));
		continue;
		}*/
		if (!word.IsEmpty() && (At(i) == str[0] || (At(i) == L'\n' && last != L'\n')))
		{
			words.Insert(fnv1a_64(word, word.Length()));
			word.Clear();
		}
		else if (At(i) != L'\n' && At(i) != L'\0')
			word.Append(At(i));
		last = At(i);
	}
	if (!word.IsEmpty())
		words.Insert(fnv1a_64(word,word.Length()));//Last word might be missing
	return words;

}
/*
std::vector<WString> WString::splitCode()
{
	
	std::vector<WString> words;
	WString word;
	bool bCommentBlock=false,bCommentLine=false;
	wchar_t last;
	if(IsEmpty()) return words;
	for(int i=0;i<=Length();++i)
	{
		if((at(i) == L'/' && at(i+1) == L'*'))
			bCommentBlock = true;
		else if(at(i) == L'/' && at(i+1) == L'/')
			bCommentLine = true;
		else if(bCommentBlock && (at(i) == L'*' && at(i+1) == L'/'))
		{
			bCommentBlock = false;
			++i;
			continue;
		}
		else if(bCommentLine && at(i) == L'\n')
		{
			bCommentLine = false;
			continue;
		}
		if(bCommentBlock || bCommentLine) continue;
		if( (i + 3 < nlength) &&
			((at(i) == L'>' && at(i + 1) == L'>' && at(i + 2) == L'>' && at(i + 3) == L'=')
			))
		{
			if(!word.IsEmpty())
			{
				words.push_back((word));
				word.Clear();
			}
			words.push_back(WString( nstring + i, 4) );
			++i;
			continue;
		}
		
		if( (i + 2 < nlength) &&
			((at(i) == L'>' && at(i + 1) == L'>' && at(i + 2) == L'>')
			|| (at(i) == L'>' && at(i + 1) == L'>' && at(i + 2) == L'=')
			|| (at(i) == L'<' && at(i + 1) == L'<' && at(i + 2) == L'=')
			))
		{
			if(!word.IsEmpty())
			{
				words.push_back((word));
				word.Clear();
			}
			words.push_back(WString( nstring + i, 3) );
			++i;
			continue;
		}
		if( (i + 1 < nlength) && 
		   ((at(i) == L'#' && at(i + 1) == L'#')
		||	(at(i) == L'+' && at(i + 1) == L'+')
		||	(at(i) == L'-' && at(i + 1) == L'-')
		||	(at(i) == L'|' && at(i + 1) == L'|')
		||	(at(i) == L'&' && at(i + 1) == L'&')
		||	(at(i) == L'+' && at(i + 1) == L'=')
		||	(at(i) == L'-' && at(i + 1) == L'=')
		||	(at(i) == L'*' && at(i + 1) == L'=')
		||	(at(i) == L'/' && at(i + 1) == L'=')
		||	(at(i) == L'&' && at(i + 1) == L'=')
		||	(at(i) == L'|' && at(i + 1) == L'=')
		||	(at(i) == L'=' && at(i + 1) == L'=')
		||	(at(i) == L'~' && at(i + 1) == L'=')
		||	(at(i) == L'<' && at(i + 1) == L'<')
		||	(at(i) == L'>' && at(i + 1) == L'>')
		))
		{
			if(!word.IsEmpty())
			{
				words.push_back((word));
				word.Clear();
			}
			words.push_back(WString( nstring + i, 2) );
			++i;
			continue;
		}

		
		if(
				at(i) == L':' 
			||	at(i) == L';' 
			||	at(i) == L'(' 
			||	at(i) == L'[' 
			||	at(i) == L']' 
			||	at(i) == L')' 
			||	at(i) == L'{' 
			||	at(i) == L'}' 
			||	at(i) == L',' 
			||	at(i) == L'>'
			||	at(i) == L'<'
			||	( at(i) == L'.' && ( at(i-1) > L'9' || at(i-1) < L'0') )
			||	at(i) == L'+'
			||	at(i) == L'-'
			||	at(i) == L'*'
			||	at(i) == L'/'
			||	at(i) == L'^'
			||	at(i) == L'%'
			||	at(i) == L'!'
			||	( at(i) == L'#' && Substr(i,7) != L"#define") 
			||	at(i) == L'=' ) //Special characters
		{
			if(!word.IsEmpty())
			{
				words.push_back((word));
				word.Clear();
			}
			words.push_back(WString(at(i)));
			continue;
		}
		if(!word.IsEmpty() && (at(i) == L' ' || (at(i) == L'\n' && last != L'\n')))
		{
			words.push_back((word));
			word.Clear();
		}
		else if(at(i) != L'\n' && at(i) > L' ')
			word.Append(at(i));
		last = at(i);
	}
	if(!word.IsEmpty())
		words.push_back(word);//Last word might be missing
	return words;
	
}
*/
bool WString::ReplaceAt(const wchar_t* str,UINT start,UINT count)
{ 
		INT len = wcslen(str);
		UINT stop = start + count;
		if (start > nlength || (start + count) > nlength || count < 1)
			return false;
		INT diff = len - count;
		if (diff > 0)
		{
			for (int i = nlength - 1; i > stop; --i)
				nstring[i + diff] = nstring[i];
		}
		else if (diff < 0)
		{
			for (int i = stop; i < nlength + diff; ++i)
				nstring[i + diff] = nstring[i];
		}
		memcpy(nstring + start, str, len*2);
		return true;
}
bool WString::ReplaceWith(const wchar_t* str,const wchar_t* with)
{
		INT index = this->FindExact(str);
		if(index != -1)
			return ReplaceAt(with,index,wcslen(str) + index);
		return false;
}
void WString::ReplaceAll(const wchar_t str, const wchar_t with)
{
	for (int i = 0; i < nlength; ++i)
		if (nstring[i] == str)
			nstring[i] = with;
}

int WString::Length() const
{
	return nlength-1;
}
bool WString::IsEmpty() const
{
	return nlength < 2;
}
void WString::Swap(WString& swap)
{
		wchar_t* buf = NewWCharStr();
		memcpy(buf,nstring, nlength * 2);
		memcpy(nstring,swap.nstring, nlength * 2);
		memcpy(swap.nstring,buf, nlength * 2);
		short i = nlength;
		nlength = swap.nlength;
		swap.nlength = i;
}
WString WString::Substr(int start,int num) const
{
		if(start > Length())
			throw "Start index can't be greater than the Length of the WString.";
		if(start + num > Length() || num == 0)
					num = Length() - start;
		if(num < 0)
			throw "Number of characters can't be less than 0.";
		return WString(nstring + start,num);
}
int WString::Find(const wchar_t* in,int index) const
{
		int len = wcslen(in);
		for(int i = index; i < Length(); ++i)
			for(int j=0; j < len; ++j)
				if(nstring[i] == in[j])
					return i;
		return -1;
}
int WString::FindExact(const wchar_t* in,int index) const
{
		int len = wcslen(in);
		for(int i=index;i< nlength - len;++i)
		{
			if(memcmp(nstring + i,in,len * 2) == 0)
				return i;
		}
		return -1;
}
bool WString::BeginsWith(const wchar_t* in) const 
{
		size_t len = wcslen(in);
		return wcscmp(nstring, in) == 0;
}
bool WString::EndsWith(const wchar_t* in) const
{

		size_t len = wcslen(in) * 2;
		size_t factor = (nlength * 2 - len) - 2;
		return wcscmp(nstring + factor, in) == 0;
}
void WString::Clear()
{
		*nstring = L'\0';
		nlength = 1;
}
void WString::Detach(int start, int num)
{
		if (num == 0)
		{
			nlength = start + 1;
			nstring[start] = L'\0';
		}
		else
		{
			if (start + num >= nlength)
				num = nlength - start;
			memmove(nstring + (start * 2), nstring + (start * 2) + (num * 2), (nlength * 2) - ((start * 2) + (num * 2)));
		}
}
INT WString::Count(const wchar_t str) const
{
		INT Count=0;
		for(int i=0;i<nlength-1;++i)
		{
			if(str == nstring[i])
				Count++;
		}
		return Count;
}
bool operator == (const WString& f, const WString& s)
{
		if(f.nlength == s.nlength)
			return wcscmp(f.nstring,s.nstring) == 0;
		else return false;
}
bool operator |= (const WString& g, const WString& d)
{
		if(g.nlength == d.nlength)	//NOTE: May not support locales properly
			return appwcscmp_ci(g.nstring, d.nstring) == 0;
		else return false;
}
bool operator |= (const WString& g, const wchar_t* d)
{
		size_t len = wcslen(d);
		if(g.nlength == len+1)	//NOTE: May not support locales properly
			return appwcscmp_ci(g.nstring, d) == 0;
		else return false;
}
bool operator == (const WString& g, const wchar_t* s)
{
		if(g.nlength-1 == wcslen(s))
			return wcscmp(g.nstring,s) == 0;
		else return false;
}
bool operator != (const WString& f, const WString& s)
{
		if(f.nlength == s.nlength)
			return wcscmp(f.nstring,s.nstring) != 0;
		else return true;
}
bool operator != (const WString& g, const wchar_t* s)
{
		if(g.nlength-1 == wcslen(s))
			return wcscmp(g.nstring,s) != 0;
		else return true;
}
std::wostream& operator <<(std::wostream & OS, WString *S)
{
	OS << S->nstring;
	return OS;
}
std::wostream& operator <<(std::wostream & OS, WString S)
{
	OS << S.nstring;
	return OS;
}
std::wistream& operator >>(std::wistream & IS,WString *S)
{
		wchar_t input[100];
		IS.getline(input,100);
		S->Append(input);
		return IS;
}
WString& WString::operator =(const WString &S)
{
		nlength = S.nlength;
		memcpy(nstring,S.nstring,nlength * 2);
		return *this;
}
WString & WString::operator=(const String & S)
{
	nlength = S.nlength;
	char* str = S.nstring;
	wchar_t* tstr = nstring;
	while (*str) *tstr++ = *str++;
	*tstr = L'\0';
	return *this;
}
WString & WString::operator=(WString && S)
{
	nstring = S.nstring;
	nlength = S.nlength;
	S.nstring = NULL;
	S.nlength = 0;
	return *this;
}
WString& WString::operator = (const wchar_t* S)
{
	nlength = wcslen(S)+1;
	memcpy(nstring,S, nlength * 2);
	return *this;
}
WString& WString::operator = (const INT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const UINT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const INT64 S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const UINT64 S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const SHORT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const USHORT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const DOUBLE S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const FLOAT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString& WString::operator = (const BYTE S)
{
		nlength = 0;
		*this += S;
		return *this;
}
WString & WString::operator=(wchar_t S)
{
	nstring[nlength++] = S;
	return *this;
}
WString& WString::Append(const wchar_t* in, int slen)
{
		size_t len = slen > 0 ? slen : wcslen(in);
		for (int i = 0; i < len; ++i)
			nstring[nlength - 1 + i] = in[i];
		nlength += len;
		nstring[nlength - 1] = L'\0';
		return *this;
}
WString& WString::Append(const wchar_t in)
{
		nstring[nlength - 1] = in;
		++nlength;
		nstring[nlength - 1] = L'\0';
		return *this;
}
WString& WString::Append(const WString& in)
{
	return Append(in.nstring);
}
WString& WString::AppendStart(const wchar_t* in)
{
		size_t len = wcslen(in);
		for (int i = nlength + len; i > len; ++i)
			nstring[i] = nstring[i - len];
		nlength += len;
		memcpy(nstring, in, len * 2);
		return *this;
}
WString operator + (WString& S1, WString & S2)//Concat two strings
{
	WString str(S1);
	return str.Append(S2);
}
WString operator + (WString& S1, const wchar_t* S2)//Concat two strings
{
	WString str(S1);
	return str.Append(S2);
}
char* WString::Tochar() const
{
		char* C = NewCharStr();
		for (int i = 0; i < nlength; ++i)
			C[i] = (char)nstring[i];
		return C;
}
//TODO: Optimize
WString* WString::ToLower() const
{
		WString *Str = new WString();
		Str->Dolower();
		return Str;
}
WString* WString::Toupper() const
{
		WString *Str = new WString();
		Str->Doupper();
		return Str;
}
void WString::Dolower()
{
		AppToLower(nstring);
}
void WString::Doupper()//TODO: Optimize away the duplicated array
{
	
		AppToUpper(nstring);
	
}
void WString::Tobytes(BYTE*& bytes) const
{
	
		*(USHORT*)bytes = nlength;
		bytes += 2;
		memcpy(bytes, nstring, nlength * 2);
		bytes += nlength * 2;
	
}
void WString::ToFile(FileWriter *f)
{
	
		f->Write<USHORT>(nlength);
		f->Write(nstring, nlength * 2);
	
}
WString* WString::operator += (const WString& S1)
{
	return &Append(S1);
}
WString* WString::operator += (const wchar_t* S1)
{
	Append(S1);
	return this;
}

WString* WString::operator +=(UINT64 S)
{
	
		UINT64 Factor, Count;
		numDigits(S, &Count, &Factor);
		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = L'\0';
		return this;
	
}		 
WString* WString::operator +=(INT64 S)
{
	
		UINT64 Factor, Count;
		bool bNeg = (S < 0);
		S = abs(S);
		numDigits(S, &Count, &Factor);

		if (bNeg)
			nstring[++nlength - 1] = L'-';

		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = L'\0';
		return this;
	
}	 
WString* WString::operator +=(UINT S)
{
	
		UINT64 Factor, Count;
		numDigits(S, &Count, &Factor);
		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = L'\0';
		return this;
	
}
WString* WString::operator +=(INT S)
{
	
		UINT64 Factor, Count;
		bool bNeg = (S < 0);
		S = abs(S);
		numDigits(S, &Count, &Factor);

		if (bNeg)
			nstring[++nlength - 1] = L'-';

		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = L'\0';
		return this;
	
}
WString* WString::operator +=(USHORT S)
{
	
		UINT64 Factor, Count;
		numDigits(S, &Count, &Factor);
		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = L'\0';
		return this;
	
}
WString* WString::operator +=(SHORT S)
{
	
		UINT64 Factor, Count;
		bool bNeg = (S < 0);
		S = abs(S);
		numDigits(S, &Count, &Factor);

		if (bNeg)
			nstring[++nlength - 1] = L'-';

		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = L'\0';
		return this;
	
}
WString* WString::operator +=(BYTE S)
{
	
		UINT64 Factor, Count;
		numDigits(S, &Count, &Factor);
		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = L'\0';
		return this;
	
}
WString* WString::operator +=(float S)
{
	
		UINT64 Factor, Count, Num;
	DOUBLE T;
	bool bNeg = (S < 0);
	S = fabs(S);
	Num = (UINT64)S;
	S -= (DOUBLE)Num;
	numDigits(Num, &Count, &Factor);

	if (bNeg)
		nstring[++nlength - 1] = L'-';

	for (;;)
	{
		nstring[nlength++ - 1] = L'0' + (Num / Factor);
		Num %= Factor;
		if (Factor == 1)
			break;
		Factor /= 10;
	}
	nstring[nlength++ - 1] = '.';
	for (Count = 0;; ++Count)
	{
		S *= 10;
		Factor = (UINT64)S;		//Reuse this variable, truncate the fraction
		T = (DOUBLE)Factor;		//Convert to an integer using cvtsd2si SSE2
		S -= T;
		nstring[nlength++ - 1] = L'0' + (S * 10);
		if (Count == 6)
			break;	//We're done.
	}
	nstring[nlength - 1] = L'\0';
	return this;
	
}		 
WString* WString::operator +=(double S)
{
	
		UINT64 Factor, Count, Num;
		DOUBLE T;
		bool bNeg = (S < 0);
		S = fabs(S);
		Num = (UINT64)S;
		S -= (DOUBLE)Num;
		numDigits(Num, &Count, &Factor);

		if (bNeg)
			nstring[++nlength - 1] = L'-';

		for (;;)
		{
			nstring[nlength++ - 1] = L'0' + (Num / Factor);
			Num %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[nlength++ - 1] = '.';
		for (Count = 0;; ++Count)
		{
			S *= 10;
			Factor = (UINT64)S;		//Reuse this variable, truncate the fraction
			T = (DOUBLE)Factor;		//Convert to an integer using cvtsd2si SSE2
			S -= T;
			nstring[nlength++ - 1] = L'0' + (S * 10);
			if (Count == 14)
				break;	//We're done.
		}
		nstring[nlength - 1] = L'\0';
		return this;
	
}
WString* WString::operator-= (const wchar_t* s1)
{
	int pos = FindExact((wchar_t*)s1);
	if(pos != -1)
		Detach(pos,wcslen(s1));
	return this;
}

void WString::Trim()
{
	//Move the string pointer forward, we'll lose a couple of characters here, it's not completely safe
	//-but for most situations it will be fine. It's super fast! Since we don't perform any relocation
	while (*nstring && (*nstring == ' ' || *nstring == '\t'))
	{
		++nstring;
		--nlength;
	}


	if (nlength == 1)	//Empty?
		return;

	nlength -= 2;

	while (nstring[nlength] && (nstring[nlength] == ' ' || nstring[nlength] == '\t'))
		--nlength;

	++nlength;
	nstring[nlength] = L'\0';
	++nlength;
}

void WString::Chop(int num)
{
	nlength -= num;
	nstring[nlength - 1] = L'\0';
}

void WString::ChopStart(int num)
{
	nlength -= num;
	nstring += num;
}


/*
///////////////////////////////////////////////////////////////////////////
String
///////////////////////////////////////////////////////////////////////////
*/

String::String(void) : nstring(NewCharStr()), nlength(1)
{
}
int String::Findlast(const char* in, int index) const
{
	
		int len = strlen(in);
		int k = -1;
		for (int i = index; i<Length(); ++i)
		{
			for (int j = 0; j < len; ++j)
			{
				if (nstring[i] == in[j])
					k = i;
			}
		}
		return k;
	
}
//Move constructor!!
String::String(String && in)
{
	printf("Construct move\n");

	nstring = in.nstring;
	nlength = in.nlength;
	in.nstring = nullptr;
	in.nlength = 0;
}
//Copy constructor
String::String(String& in)
{
	printf("Construct copy\n");

	nstring = NewCharStr();
	nlength = in.nlength;
	memcpy(nstring, in.nstring, nlength);
}
String::String(BYTE*& bytes)
{
	printf("Construct byte\n");

		nlength = *(USHORT*)bytes + 1;
		nstring = NewCharStr();
		bytes += 2;
		memcpy(nstring, bytes, nlength);
		nstring[nlength - 1] = '\0';
		bytes += nlength - 1;
	
}
String::String(FileReader* f)
{
	printf("Construct fr\n");

		nlength = f->Read<USHORT>() + 1;
		nstring = NewCharStr();
		f->Read(nstring, nlength);
		nstring[nlength - 1] = '\0';
	
}
void String::Deserialize(BYTE** bytes)
{
	
		nlength = **(USHORT**)bytes;
		*bytes += 2;
		memcpy(nstring, *bytes, nlength);
		*bytes += nlength;
	
}
void String::Deserialize(FileReader* f)
{
		nlength = f->Read<USHORT>();
		f->Read(nstring, nlength);
}
void String::Serialize(BYTE ** data)
{
	**(USHORT**)data = nlength;
	*data += 2;
	strcpy((char*)*data, nstring);
	*data += nlength;
}
void String::Serialize(FileWriter *FW)
{
	FW->Write<USHORT>(nlength);
	FW->Write(nstring, nlength);
}
String::String(const wchar_t* in, int len)
{
	printf("Construct wchar \n");

		nlength = len > 0 ? len + 1 : wcslen(in) + 1;
		nstring = NewCharStr();
		for (int i = 0; i < nlength; ++i)
			nstring[i] = in[i];
		nstring[nlength - 1] = '\0';
}
String::String(const char* in, int len)
{
	printf("Construct char\n");

		nlength = len > 0 ? len + 1 : strlen(in) + 1;
		nstring = NewCharStr();
		memcpy(nstring, in, nlength);
		nstring[nlength - 1] = L'\0';
}
String::String(const String& in)
{
	printf("Construct str\n");

		nlength = in.nlength;
		nstring = NewCharStr();
		memcpy(nstring, in.nstring, nlength);
	
}
String::String(const WString & in)
{
	printf("Construct wstr\n");

	nstring = NewCharStr();
	wchar_t* str = in.Towchar();
	char* tstr = nstring;
	while (*str) *tstr++ = (char)*str++;
	*tstr = '\0';
}
String::~String()
{
	if (nstring)
	{
		FreeCharStr(nstring);
		printf("Destruct\n");
	}
}
HDynamicArray<String> String::Split(const char *str) const
{
	
		HDynamicArray<String> words;
		String word;
		char last;
		if (IsEmpty()) return words;
		for (int i = 0; i <= Length(); ++i)
		{
			if (!word.IsEmpty() && ( appcntstr(str,(*(nstring + i))) || (At(i) == '\n' && last != '\n')))
			{
				words.Insert( String(word) );
				word.Clear();
			}
			else if (At(i) != '\n' && At(i) != '\0')
				word.Append(At(i));
			last = At(i);
		}
		if (!word.IsEmpty())
			words.Insert(word);//Last word might be missing
		return words;
	
}
HDynamicArray<String> String::SplitCode(const char *str) const
{
	
		HDynamicArray<String> words;
	String word;
	char last;
	if (IsEmpty()) return words;
	for (int i = 0; i <= Length(); ++i)
	{
		if (!word.IsEmpty() && (appcntstr(str, (*(nstring + i))) || (At(i) == '\n' && last != '\n')))
		{
			words.Insert(String(word));
			word.Clear();
			if (At(i) != ' ' && At(i) != '\0' && At(i) != '\n')
				words.Insert(String(At(i)));
		}
		else if (At(i) != '\n' && At(i) != '\0')
			word.Append(At(i));
		last = At(i);
	}
	if (!word.IsEmpty())
		words.Insert(word);//Last word might be missing
	return words;
	
}
HDynamicArray<HASH> String::SplitHash(const char *str) const
{
	
		HDynamicArray<HASH> words;
		String word;
		char last;
		if (IsEmpty()) return words;
		for (int i = 0; i <= Length(); ++i)
		{
			if (!word.IsEmpty() && (appcntstr(str, (*(nstring + i))) || (At(i) == '\n' && last != '\n')))
			{
				words.Insert(fnv1a_64(word,word.Length()));
				word.Clear();
			}
			else if (At(i) != '\n' && At(i) != '\0')
				word.Append(At(i));
			last = At(i);
		}
		if (!word.IsEmpty())
			words.Insert(fnv1a_64(word, word.Length()));//Last word might be missing
		return words;
	
}
bool String::ReplaceAt(const char* str, UINT start, UINT count)
{
	
		INT len = strlen(str);
		UINT stop = start + count;
		if (start > nlength || (start + count) > nlength || count < 1)
			return false;
		INT diff = len - count;
		if (diff > 0)
		{
			for (int i = nlength - 1; i >= stop; --i)
				nstring[i + diff] = nstring[i];
		}
		else if (diff < 0)
		{
			for (int i = stop; i < nlength + diff; ++i)
				nstring[i + diff] = nstring[i];
		}
		memcpy(nstring + start, str, len);
		nlength += diff;
		return true;
	
}
bool String::ReplaceWith(const char* str, const char* with)
{
	
		INT index = this->FindExact(str);
	if (index != -1)
		return ReplaceAt(with, index, strlen(str));
	return false;
	
}
void String::ReplaceAll(const char str, const char with)
{
	for (int i = 0; i < nlength; ++i)
		if (nstring[i] == str)
			nstring[i] = with;
}

void String::Trim()
{
	//Move the string pointer forward, we'll lose a couple of characters here, it's not completely safe
	//-but for most situations it will be fine. It's super fast! Since we don't perform any relocation
	while (*nstring && (*nstring == ' ' || *nstring == '\t'))
	{
		++nstring;
		--nlength;
	}

	
	if (nlength == 1)	//Empty?
		return;

	nlength -= 2;

	while (nstring[nlength] && (nstring[nlength] == ' ' || nstring[nlength] == '\t'))
	{
		--nlength;
	}
	++nlength;
	nstring[nlength] = '\0';
	++nlength;
}
void String::Chop(int num)
{
	nlength -= num;
	nstring[nlength - 1] = '\0';
}
void String::ChopStart(int num)
{
	nlength -= num;
	nstring += num;
}
int String::Length() const
{
	return nlength - 1;
}
bool String::IsEmpty() const
{
	return nlength < 2;
}
void String::Swap(String& swap)
{
	
		char* buf = NewCharStr();
		memcpy(buf, nstring, nlength);
		memcpy(nstring, swap.nstring, nlength);
		memcpy(swap.nstring, buf, nlength);
		short i = nlength;
		nlength = swap.nlength;
		swap.nlength = i;
	
}
String String::Substr(int start, int num) const
{
	
		if (start > Length())
			return "";
		if (start + num > Length() || num == 0)
			num = Length() - start;
		if (num < 0)
			return "";
			
		return String(nstring + start, num);
	
}
int String::Find(const char* in, int index) const
{
	
		int len = strlen(in);
		for (int i = index; i < Length(); ++i)
			for (int j = 0; j < len; ++j)
				if (nstring[i] == in[j])
					return i;
	return -1;
	
}
int String::FindExact(const char* in, int index) const
{
	
		int len = strlen(in);
		for (int i = index; i < nlength - len; ++i)
		{
			if (memcmp(nstring + i, in, len) == 0)
				return i;
		}
		return -1;
	
}
bool String::BeginsWith(const char* in) const
{
	
		size_t len = strlen(in);
		return memcmp(nstring, in, len) == 0;
	
}
bool String::EndsWith(const char* in) const
{
	
		size_t len = strlen(in);
		size_t factor = (nlength - len) - 1;
		return memcmp(nstring + factor, in, len) == 0;
	
}
void String::Clear()
{
	*nstring = '\0';
	nlength = 1;
}
void String::Detach(int start, int num)
{
	
		if (num == 0)
		{
			nlength = start + 1;
			nstring[start] = '\0';
		}
		else
		{
			if (start + num >= nlength)
				num = nlength - start;
			memmove(nstring + start, nstring + start + num, nlength - (start + num));
		}
	
}
INT String::Count(const char str) const
{
	
		INT Count = 0;
		for (int i = 0; i < nlength - 1; ++i)
		{
			if (str == nstring[i])
				Count++;
		}
		return Count;
	
}
bool operator == (const String& f, const String& s)
{
	if (f.nlength == s.nlength)
		return !strcmp(f.nstring, s.nstring);
	else return false;
}
bool operator |= (const String& g, const String& d)
{
	if (g.nlength == d.nlength)	//NOTE: May not support locales properly
		return appstrcmp_ci(g.nstring, d.nstring) == 0;
	else return false;
}
bool operator |= (const String& g, const char* d)
{
	return appstrcmp_ci(g.nstring, d) == 0;
}
bool operator == (const String& g, const char* s)
{
	return !strcmp(g.nstring, s);
}
bool operator != (const String& f, const String& s)
{
	return strcmp(f.nstring, s.nstring);
}
bool operator != (const String& g, const char* s)
{
	return strcmp(g.nstring, s);
}
std::ostream& operator <<(std::ostream & OS, String *S)
{
	OS << S->nstring;
	return OS;
}
std::ostream& operator <<(std::ostream & OS, String S)
{
	OS << S.nstring;
	return OS;
}
std::istream& operator >>(std::istream & IS, String *S)
{
		char input[100];
		IS.getline(input, 100);
		S->Append(input);
		return IS;
}
String& String::operator = (const String &S)
{
		nlength = S.nlength;
		memcpy(nstring, S.nstring, nlength);
		return *this;
}
String & String::operator=(String && S)
{
	nstring = S.nstring;
	nlength = S.nlength;
	S.nstring = NULL;
	S.nlength = 0;
	return *this;
}
String& String::operator = (const WString & S)
{
	nlength = S.nlength;
	char* str = S.Tochar();
	while (*str++) *nstring++ = (char)*str++;
	nstring[nlength - 1] = '\0';
	return *this;
}
String& String::operator = (const char* S)
{
		nlength = strlen(S) + 1;
		memcpy(nstring, S, nlength);
		return *this;
}
String& String::operator = (const INT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const UINT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const INT64 S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const UINT64 S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const SHORT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const USHORT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const DOUBLE S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const FLOAT S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String& String::operator = (const BYTE S)
{
		nlength = 0;
		*this += S;
		return *this;
}
String & String::operator=(const char S)
{
	nstring[nlength++] = S;
	return *this;
}
String& String::Append(const char* in, int slen)
{
		size_t len = slen>0 ? slen : strlen(in);
		for (int i = 0; i<len; ++i)
			nstring[nlength - 1 + i] = in[i];
		nlength += len;
		nstring[nlength - 1] = '\0';
		return *this;
}
String& String::Append(const String& in)
{
	return Append(in.nstring);
}
String& String::Append(const char in)
{
	nstring[nlength - 1] = in;
	++nlength;
	nstring[nlength - 1] = '\0';
	return *this;
}
String& String::AppendStart(const char* in)
{
		size_t len = strlen(in);
		for (int i = nlength + len; i > len; ++i)
			nstring[i] = nstring[i - len];
		nlength += len;
		memcpy(nstring, in, len);
		return *this;
}
String operator + (const String& S1, const String & S2)//Concat two strings
{
	String str(S1);
	return str.Append(S2);
}
String operator + (const String& S1, const char* S2)//Concat two strings
{
	String str(S1);
	return str.Append(S2);
}
wchar_t* String::Towchar() const
{
		wchar_t* C = NewWCharStr();
		for (int i = 0; i < nlength; ++i)
			C[i] = nstring[i];
		return C;
}
//TODO: Optimize
String* String::ToLower() const
{
		String *Str = new String(*this);
		Str->Dolower();
		return Str;
}
String* String::Toupper() const
{
		String *Str = new String(*this);
		Str->Doupper();
		return Str;
}
void String::Dolower()
{
	
		AppToLower(nstring);
	
}
void String::Doupper()
{
	
		AppToUpper(nstring);
	
}
void String::Tobytes(BYTE*& bytes) const
{
	
		*(USHORT*)bytes = nlength;
		bytes += 2;
		memcpy(bytes, nstring, nlength);
		bytes += nlength;
	
}
void String::ToFile(FileWriter *f)
{
	f->Write<USHORT>(nlength);
	f->Write(nstring, nlength);
}
String& String::operator += (const String& S1)
{
	return Append(S1);
}
String& String::operator += (const char* S1)
{
	Append(S1);
	return *this;
}
String& String::operator +=(UINT64 S)
{
	
		UINT64 Factor, Count;
		numDigits(S, &Count, &Factor);
		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(INT64 S)
{
	
		UINT64 Factor, Count;
		bool bNeg = (S < 0);
		S = abs(S);
		numDigits(S, &Count, &Factor);

		if (bNeg)
			nstring[++nlength - 1] = '-';

		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(UINT S)
{
	
		UINT64 Factor, Count;
		numDigits(S, &Count, &Factor);
		for (;;)
		{
			nstring[++nlength - 1] = '0' + (S % Factor);
			Factor /= 10;
			if (Factor < 10)
				break;
		}
		nstring[++nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(INT S)
{
	
		UINT64 Factor, Count;
		bool bNeg = (S < 0);
		S = abs(S);
		numDigits(S, &Count, &Factor);
		if (bNeg)
			nstring[nlength++ - 1] = '-';
		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(USHORT S)
{
	
		UINT64 Factor, Count;
		numDigits(S, &Count, &Factor);
		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(SHORT S)
{
	
		UINT64 Factor, Count;
		bool bNeg = (S < 0);
		S = abs(S);
		numDigits(S, &Count, &Factor);
		if (bNeg)
			nstring[++nlength - 1] = '-';
		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(BYTE S)
{
	
		UINT64 Factor, Count;
		bool bNeg = (S < 0);
		S = abs(S);
		numDigits(S, &Count, &Factor);
		if (bNeg)
			nstring[++nlength - 1] = '-';
		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (S / Factor);
			S %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[++nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(float S)
{
	
		UINT64 Factor, Count, Num;
		DOUBLE T;
		bool bNeg = (S < 0);
		S = fabs(S);
		Num = (UINT64)S;
		S -= (DOUBLE)Num;
		numDigits(Num, &Count, &Factor);

		if (bNeg)
			nstring[++nlength - 1] = '-';

		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (Num / Factor);
			Num %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[nlength++ - 1] = '.';
		for (Count = 0;; ++Count)
		{
			S *= 10;
			Factor = (UINT64)S;		//Reuse this variable, truncate the fraction
			T = (DOUBLE)Factor;		//Convert to an integer using cvtsd2si SSE2
			S -= T;
			nstring[nlength++ - 1] = '0' + (S * 10);
			if (Count == 6)
				break;	//We're done.
		}
		nstring[nlength - 1] = '\0';
		return *this;
	
}
String& String::operator +=(double S)
{
	
		UINT64 Factor, Count, Num;
		DOUBLE T;
		bool bNeg = (S < 0);
		S = fabs(S);
		Num = (UINT64)S;
		S -= (DOUBLE)Num;
		numDigits(Num, &Count, &Factor);

		if (bNeg)
			nstring[++nlength - 1] = '-';

		for (;;)
		{
			nstring[nlength++ - 1] = '0' + (Num / Factor);
			Num %= Factor;
			if (Factor == 1)
				break;
			Factor /= 10;
		}
		nstring[nlength++ - 1] = '.';
		for (Count = 0;; ++Count)
		{
			S *= 10;
			Factor = (UINT64)S;		//Reuse this variable, truncate the fraction
			T = (DOUBLE)Factor;		//Convert to an integer using cvtsd2si SSE2
			S -= T;
			nstring[nlength++ - 1] = '0' + (S * 10);
			if (Count == 14)
				break;	//We're done.
		}
		nstring[nlength - 1] = '\0';
		return *this;
	
}
String& String::operator+=(const char S1)
{
	return Append(S1);
}
String& String::operator-= (const char* s1)
{
	int pos = FindExact((char*)s1);
	if (pos != -1)
		Detach(pos, strlen(s1));
	return *this;
}

/*
///////////////////////////////////////////////////////////////////////////
Misc
///////////////////////////////////////////////////////////////////////////
*/

bool appwcscmp_ci(const wchar_t* a, const wchar_t* b)
{
	size_t as = wcslen(a), bs = wcslen(b);
	if (as == bs)
	{
		for (int i = 0; i<as; ++i)
		{
			if (a[i] != b[i] && (((a[i] < L'A' || a[i] > L'z') || (b[i] < L'A' || b[i] > L'z')) || abs(a[i] - b[i]) != 0x20))
				return false;
		}
		return true;
	}
	else return false;
}
bool appstrcmp_ci(const char * a, const char * b)
{
	size_t as = strlen(a), bs = strlen(b);
	if (as == bs)
	{
		for (int i = 0; i<as; ++i)
		{
			if (a[i] != b[i] && (((a[i] < 'A' || a[i] > 'z') || (b[i] < 'A' || b[i] > 'z')) || abs(a[i] - b[i]) != 0x20))
				return false;
		}
		return true;
	}
	else return false;
}
bool appstrend(const char * a, const char * b)
{
	int len = strlen(b);
	return memcmp(a + strlen(a) - len, b, len) == 0;
}
bool appcntstr(const char *a, const char b)
{
	int len = strlen(a);
	for (int i = 0; i < len; ++i)
		if (a[i] == b)
			return true;
	return false;
}
//WARNING: cross-thread safety
static char* indices[16383];
static char _strs[16383][512];
static wchar_t* windices[16383];
static wchar_t _wstrs[16383][512];

INT InitStrings()
{
	for (int i = 0; i < 16383; ++i)
		indices[i] = _strs[i];
	return 16382;
}

INT InitWStrings()
{
	for (int i = 0; i < 16383; ++i)
		windices[i] = _wstrs[i];
	return 16382;
}

static INT idx = -1;

char* NewCharStr()
{
	if (idx == -1)
		idx = InitStrings();

	idx &= 0x3FFF;
	*indices[idx] = '\0';
	return indices[idx--];
}

void FreeCharStr(char* str)
{
	indices[++idx] = str;
}

static INT widx = -1;

wchar_t * NewWCharStr()
{
	if (widx == -1)
		widx = InitWStrings();

	widx &= 0x3FFF;
	*windices[idx] = L'\0';
	return windices[widx--];
}

void FreeWCharStr(wchar_t* str)
{
	windices[++widx] = str;
}

void AppToLower(wchar_t* w) //A-Z
{
	int Len = wcslen(w);
	for(int i=0;i<Len;++i)
	{
		if (w[i] >= L'A' && w[i] <= L'Z')
			w[i] += 0x20;//w[i] = L'a' + (w[i] - L'A');
	}
}
void AppToLower(char* w)
{
	int Len = strlen(w);
	for(int i=0;i<Len;++i)
	{
		if (w[i] >= L'A' && w[i] <= L'Z')
			w[i] += 0x20;//w[i] = L'a' + (w[i] - L'A');
	}
}
void AppToUpper(wchar_t* w) //A-Z
{
	int Len = wcslen(w);
	for (int i = 0; i<Len; ++i)
	{
		if (w[i] >= L'a' && w[i] <= L'z')
			w[i] -= 0x20;//w[i] = L'A' + (w[i] - L'A');
	}
}
void AppToUpper(char* w)
{
	int Len = strlen(w);
	for (int i = 0; i<Len; ++i)
	{
		if (w[i] >= L'a' && w[i] <= L'z')
			w[i] -= 0x20;//w[i] = L'a' + (w[i] - L'A');
	}
}
void AppCheckAlpha(const wchar_t* s)
{
	const wchar_t* st=s;
	
	while(*s)
	{
		if((*s > L'z' || *s < L'a') && (*s < L'0' || *s > L'9') && *s != L'_' && *s != L'@' && *s != L'?')
			throw "Not an alpha std::string.";
		s++;
	}
}
void AppCheckAlpha(const char* s)
{
	
	while(*s)
	{
		if((*s > 'ö' || *s < 'a') && (*s < '0' || *s > '9') && *s != '_' && *s != '@' && *s != '?')
			throw "Not an alpha std::string.";
		s++;
	}
	
}
bool AppIsAlpha(const wchar_t* s)
{
	const wchar_t* st=s;
	
	while(*s)
	{
		if(( *s > L'z' || *s < L'a') && ( *s > L'Z' || *s < L'A') && (*s < L'0' || *s > L'9'))
			return false;
		s++;
	}
	return true;
}
bool AppIsAlpha(const char* s)
{
	
	while(*s)
	{
		if ((*s > 'z' || *s < 'a') && (*s > 'Z' || *s < 'A') && (*s < '0' || *s > '9'))
			return false;
		s++;
	}
	return true;
	
}