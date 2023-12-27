#ifndef MACROS_H
#define MACROS_H


#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/Loading/FileSystem.h"


#define CHECKNULL(codeblock)			\
		if(codeblock == nullptr)		\
		{  LOG_ERROR("Null Reference at : {0} | Line {1} ", FileSystem::GetFileNameWithExt(__FILE__), __LINE__ ); }  \

#if defined(_MSC_VER)
#define DISABLE_PUSH_WARNING __pragma(warning(push))
#define DISABLE_POP_WARNING __pragma(warning(pop))

#define DISABLE_WARNING_CONVERSION_TO_SMALLER_TYPE __pragma(warning(disable : 4267))
#endif

#endif // !MACROS_H


