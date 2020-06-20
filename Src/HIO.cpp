
#include "Template.h"

dense_hash_map<UINT64, Directory*> Directory::CachedDirectories;
String Directory::PWD = std::filesystem::current_path().string().c_str();
Directory* Directory::PWDIR = Directory::GetAbsoluteDirectory(std::filesystem::current_path().string().c_str());

bool Directory::Exists()
{
	return Exists(this);
}

Directory * Directory::GoToDirectory(const char *Dir)
{
	HASH Hash = HashName(Dir);
	auto it = Children.find(Hash);
	if (it != Children.end())
		return it->second;

	//Doesn't exist so we have to create it

	Directory* NewDir = new Directory;
	NewDir->Parent = this;
	NewDir->PathName = new String( *PathName + DIRJOINSTR + Dir);
	NewDir->PathNameHash = HashName(NewDir->PathName->Tochar());
	NewDir->DirName = new String(Dir);
	NewDir->DirNameHash = Hash;
	CachedDirectories[NewDir->PathNameHash] = NewDir;
	Children[Hash] = NewDir;
	return NewDir;
}

//We want this to return true if the directory was created, false otherwise
bool Directory::CreateDir()
{
	return mkdir(PathName->Tochar()) == 0;
}

bool Directory::CreatePath()
{

	if (Exists())
		return true;
	bool bSucess;
	if (Parent)
		bSucess = Parent->CreatePath();
	if(bSucess)
		return CreateDir();
	return false;
}

bool Directory::IndexDirectory()
{
	if(Files.size())
		Files.clear();
	if (!Exists())
		return false;

	DIR* dir = opendir(PathName->Tochar());
	dirent *direntry;
	if (!dir)
		return false;
	while (direntry = readdir(dir))
	{
		if (direntry->d_name[0] == '.') continue;	//Skip hidden files

		HASH Hash = HashName(direntry->d_name);
		Files[Hash] = new FileInfo(this, direntry->d_name);
	}
	return true;
}

Directory * Directory::GetAbsoluteDirectory(String Str)
{
	//We might want a better way to deal with errors later.
	if (Str.IsEmpty())
		return nullptr;

	if (Str.EndsWith(DIRJOINSTR))
		Str.Chop(1);

	if (Str.EndsWith(DIRJOINSTR))	//Malformed
		return nullptr;

#ifdef WIN32
	if (!GetDriveTypeA(Str.Tochar()))	//Not a valid drive
		return nullptr;
#else
	if (!Str->BeginsWith("/"))
		return nullptr;
#endif

	//From here we assume that the root directory is valid

	HASH Hash = HashName(Str.Tochar());

	auto it = CachedDirectories.find(Hash);

	if (it != CachedDirectories.end())
		return it->second;

	//Isn't created yet so do that now

	Directory* Dir = new Directory;

	Dir->PathNameHash = Hash;
	int index = Str.Findlast("/\\");
	if (index > 0)	// 0 would mean root in linux and -1 would mean root in Windows
	{
		Dir->DirName = new String(std::move(Str.Substr(index + 1)));
		Dir->DirNameHash = HashName(Dir->DirName->Tochar());
		String Parent = std::move(Str.Substr(0, index));

		//This must be at the end because the Str object will be rendered invalid.
		Dir->PathName = new String(std::move(Str));

		Dir->Parent = Directory::GetAbsoluteDirectory(std::move(Parent));
		//Dir->Parent should never be NULL here

		//Add the directory to the children of the parent
		Dir->Parent->Children[Dir->DirNameHash] = Dir;
	}
	else  //Root directory
	{
		Dir->DirName = new String(Str);
		Dir->PathName = new String(std::move(Str));
		Dir->DirNameHash = Dir->PathNameHash = Hash;
		Dir->Parent = nullptr;
	}
	
	//Finally, add the directory to the cache
	Directory::CachedDirectories[Hash] = Dir;
	
	return Dir;
}

Directory * Directory::GetRelativeDirectory(String Str)
{
	if (Str.BeginsWith(DIRJOINSTR))
		Str.ChopStart(1);
	else if (Str.BeginsWith(".."))	//Don't use this!
		return nullptr;
	else if (Str.BeginsWith("."))
		Str.ChopStart(2);
	return GetAbsoluteDirectory(PWD + DIRJOINSTR + Str);
}

void Directory::SetCurrentWorkingDir(const char *Dir)
{
	if (Exists(Dir))
		PWD = Dir;
}

void Directory::DebugPrint()
{
	printf("\nPWD: %s\n\n", PWD.Tochar());
	for (auto it = Directory::CachedDirectories.begin(); it != Directory::CachedDirectories.end(); ++it)
	{
		printf("Directory path: %s\n", it->second->PathName->Tochar());
		printf("Directory name: %s\n", it->second->DirName->Tochar());
		printf("\n\n");
	}
}

bool Directory::Exists(const char *Path)
{
	struct stat info;
	if (stat(Path, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}

bool Directory::Exists(Directory *Dir)
{
	return Exists(Dir->PathName->Tochar());
}

bool Directory::IsRelative(const char *f)
{
#ifdef WIN32
	return f[1] == ':';
#else
	return f[0] == '/';
#endif
}

FileInfo::FileInfo(const char *file)
{
	File = file;
	bExists = stat(file, &filestat) && S_ISREG(filestat.st_mode);
}

FileInfo::FileInfo(Directory* Dir, const char* _file)
{
	File = *Dir->PathName + DIRJOINSTR + _file;
	bExists = stat(File.Tochar(), &filestat) && S_ISREG(filestat.st_mode);
}

bool FileInfo::Exists()
{
	return bExists;
}

bool FileInfo::ExistsLive()
{
	return bExists = stat(File, &filestat) && S_ISREG(filestat.st_mode);
}
