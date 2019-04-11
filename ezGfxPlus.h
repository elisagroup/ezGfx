
#ifndef EZGFXPLUS_STATIC
#	ifdef EZGFXPLUS_EXPORTS
#		define EZGFXPLUS_API __declspec(dllexport)
#	else
#		define EZGFXPLUS_API __declspec(dllimport)
#	endif
#else
#	define EZGFXPLUS_API
#endif

#include <string>
#include <bitset>