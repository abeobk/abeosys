#pragma once
#ifdef  ABEOSYSLIB_EXPORTS
#	define ABEOSYSLIB __declspec(dllexport) 
#else
#	ifndef _MANAGED
#		define ABEOSYSLIB __declspec(dllimport) 
#	endif
#endif
