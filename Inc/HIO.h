/*
	General file input/output help library
	Revision:
		Christopher Békési - initial build 2015-06-22
*/
#pragma once


#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <io.h>
#include <vector>
#include <assert.h>
#include "dirent.h"

#pragma warning(disable:4251)

//File ops
#define F_BINARY "b"
#define F_APPEND "a"


//Forward declarations


//Constants
#define MAX_IMPORT 128
#define OBJECT_POS 33

//File baseclass.
class MAGICGUIAPI FileBase
{
public:

	/*Closing doesn't invalidate the file. It just closes the handle. It can easily be reopened.*/
	inline void Close()
	{
		if (fd != -2)
		{
			close(fd);
			fd = -2;
		}
	}

	inline bool IsEof() 
	{ 
		return !(filestat.st_size - lseek(fd, 0, SEEK_CUR));
	}

	inline bool IsOpen()
	{
		return fd != -2;
	}

	inline UINT64 GetSize()
	{
		return filestat.st_size;
	}

	inline UINT64 GetPos()
	{
		return lseek(fd, 0, SEEK_CUR);
	}

	inline char* GetFilename()
	{
		return filename;
	}

	~FileBase()
	{
		if (fd != -2)
			close(fd);
		if (filename)
			free(filename);
	}


	static bool Exists(unsigned char*);
protected:
	int fd;
	struct stat filestat;
	char* filename;	//We're not maintaining a string stack in C++
	FileBase() : fd(-2), filename(NULL) {}
};

//FileReader class; Reads content of a file in binary mode.
class MAGICGUIAPI FileReader : public FileBase
{
public:
	/*Public fields*/
	inline bool Open(const char* _Filename)
	{
		//We wont perform any sanity checks here. It should be the responsibility of the caller
		fd = open(_Filename, O_BINARY | O_RDONLY, S_IREAD);
		int len = strlen(_Filename);
		filename = (char*)malloc(len + 1);
		memcpy(filename, _Filename, len + 1);
		if (fd == -1)
			return false;
		if (fstat(fd, &filestat))
		{
			printf("Failed to get filestat: %s reason: %s", _Filename, strerror(errno));
		}
		return true;
	}

	/*This function attempts to reopen the file if possible*/
	inline bool ReOpen()
	{
		/*It's closed*/
		if (fd == -2)
		{
			fd = open(filename, O_BINARY | O_RDONLY, S_IREAD);
			fstat(fd, &filestat);	//Reload file info
		}
		return fd != -1;
		/*No additional fstat needs to be done.*/
	}

	/*Relative position*/
	inline void SetPosOffset(UINT64 pos)
	{
		lseek(fd, pos, SEEK_CUR);
	}
	/*Absolute position*/
	inline void SetPos(UINT64 pos)
	{
		lseek(fd, pos, SEEK_SET);
	}

	template<typename T> T Read()
	{
		T data;
		read(fd, &data, sizeof(T));
		return data;
	}
	inline void Read(void* _Buffer, UINT64 _Size)
	{
		read(fd,_Buffer, _Size);
	}

};

//FileWriter class; Writes content to a file in binary mode
class MAGICGUIAPI FileWriter : public FileBase
{
public:
												/*Set bAppend to false to erase the file*/
	inline bool Open(const char* _Filename, bool bAppend = true)
	{
		fd = open(_Filename, O_BINARY | O_TRUNC | O_CREAT | (bAppend ? O_WRONLY | O_APPEND : O_WRONLY), S_IWRITE);
		if (fd == -1)
			return false;
		fstat(fd, &filestat);
		return true;
	}

	inline bool ReOpen(bool bAppend = true)
	{
		if (fd == -2)
		{
			fd = open(filename, O_BINARY | O_TRUNC | O_CREAT | (bAppend ? O_WRONLY | O_APPEND : O_WRONLY | O_TRUNC), S_IWRITE);
			if (fd == -1)
				return false;
			fstat(fd, &filestat);
		}
		return true;
	}

	template<typename T> void Write(T data)
	{
		write(fd, &data, sizeof(T));
		filestat.st_size += sizeof(T);
	}

	inline void Write(void* Buffer, UINT64 Size)
	{
		write(fd, Buffer, Size);
		filestat.st_size += Size;
	}
};

class MAGICGUIAPI TextFileReader : public FileReader
{
private:
	String Buf;
	int pos;
	//Don't access these
	template<typename T> T Read();
	inline void Read(void* _Buffer, UINT64 _Size);

public:

	TextFileReader() : pos(0), Buf() {}

	String & ReadLine()
	{
		//Read data in chunks of 64 bytes
		//TODO: This should be implemented more efficiently with
		int start = pos;
		int index;
		Buf.nlength = 0;
		while ((index = Buf.Find("\n\0")) == -1 && !IsEof() && (Buf.nlength) < 512 /*Avoid buffer overflow*/)
		{
			Buf.nlength += read(fd, Buf.Tochar() + Buf.nlength, 64);	//Continuously read max 64 bytes into the string
		}
		//Jump back to last position if any
		if (index != -1)
		{
			SetPos(pos = (start + index) + 1);
			++index;
			Buf[index] = '\0';
			Buf.nlength = index + 1;
		}
		else if (IsEof())
		{
			++Buf.nlength;
		}
		return Buf;
	}

	String & ReadString()
	{
		int start = pos;
		int index;
		Buf.nlength = 0;
		while ((index = Buf.Find("\0")) == -1 && !IsEof() && (Buf.nlength) < 512 /*Avoid buffer overflow*/)
		{
			Buf.nlength += read(fd, Buf.Tochar() + Buf.nlength, 64);	//Continuously read max 64 bytes into the string
		}
		//Jump back to last position if any
		if (index != -1)
		{
			SetPos(pos = (start + index) + 1);
			if (Buf[index - 1] == '\r')	//Win
				--index;
			Buf[index] = '\0';
			Buf.nlength = index + 1;
		}
		return Buf;
	}
};

class MAGICGUIAPI FileInfo
{
public:
	FileInfo(const char*);
	FileInfo(class Directory*, const char* File);

	bool Exists();
	bool ExistsLive();

	String& GetFilename() { return File; }
private:
	struct stat filestat;
	bool bExists;
	String File;
	FileInfo();
};

//A class representing a directory.
//The constructor argument can hold an absolute or a relative directory
class MAGICGUIAPI Directory
{
public:
	Directory* Parent;
	dense_hash_map<HASH, Directory*> Children;
	dense_hash_map<HASH, FileInfo*> Files;

	String* DirName;
	String* PathName;

	HASH DirNameHash;
	HASH PathNameHash;

	//Directory local functions

	//Returns true if the directory exists
	bool Exists();

	//Return a new directory relative to this one
	Directory* GoToDirectory(const char*);
	
	bool CreateDir();
	bool CreatePath();

	bool IndexDirectory();

	//Static functions

	//TODO: Add support for ../../.. directory step-back

	//Returns a directory representation from an absolute path.
	//It needs to contain a path all the way to a device or mounted storage unit
	//Windows C:\Example1\Example2
	//Linux   /dev/sda1/... or /var/ftp/... Linux root starts with a forward slash
	static Directory* GetAbsoluteDirectory(String);

	//Does the same as above but operates on relative paths
	static Directory* GetRelativeDirectory(String);

	static void SetCurrentWorkingDir(const char*);

	static void DebugPrint();

	static bool Exists(const char*);
	static bool Exists(Directory*);
	static bool IsRelative(const char*);

	inline static Directory* GetRootDirectory() { return PWDIR; }

private:
	inline void* operator new(size_t t)
	{
		return ::operator new(t);
	}
	inline void operator delete(void* d)
	{
		::operator delete(d);
	}
	static dense_hash_map<HASH, Directory*> CachedDirectories;
	static String PWD;
	static Directory* PWDIR;

	Directory(void) {};
};


/*
THE END
*/