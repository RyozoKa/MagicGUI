/* OS specific includes and definitions for Linux
	Revision:
		Christopher B�k�si - initial build
*/

#include <dlfcn.h>
#include <sys/stat.h>
//Forward declarations

typedef unsigned char		BYTE;
typedef unsigned int		UNIT;
typedef float				FLOAT;
typedef int					INT;
typedef __int64				INT64;
typedef unsigned __int64	UNIT64;


#pragma warning(disable: 4996)
#pragma warning(disable: 4521)
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)
#define _CRT_SECURE_NO_WARNINGS

#define LIBEXT	".so"
#define ldfunc(handle,funcname) dlsym(handle,funcname)
#define ldlib(c) dlopen(c)
#define DIRJOIN '/'
#define DIRJOINSTR "/"
#define ODIRJOIN '\\'