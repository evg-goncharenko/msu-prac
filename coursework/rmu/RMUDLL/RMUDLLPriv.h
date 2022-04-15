#ifndef __RMUDLLPRIV_H__
#define __RMUDLLPRIV_H__

#include "RMUDLL.h"
#include <memory.h>
#include "Lib/String.h"
#include "RMUnit.h"
#include "RMStockManager.h"
#include <malloc.h>
#include "RMUUtilDLL.h"
#include <vector>
#include <algorithm>

class RMUAnswerWrapper
{
	static const char _MagicConst[];
	char _Magic[8];
	char * _pAnswer;
public:
	static  RMUAnswerWrapper * Create(const String& answer)
	{
		return new RMUAnswerWrapper(answer);
	}
	bool Check()
	{
		return memcmp(_Magic, _MagicConst, sizeof(_Magic)) == 0;
	}
	const char * GetXML(int formatted);
	~RMUAnswerWrapper();
private:
	RMUAnswerWrapper(const String& answer);
};

class RMUWrapper
{
	static const char _MagicConst[];
	char _Magic[8];
	IRMUnit * _pUnit;
	static bool _initialised;
	static IRMStockManager::FileFormat _load_format;
	static char _BASE_FILE_NAME[];
	static void Initialise();
public:
	static  int Create(RMUHandle&);
	bool Check()
	{
		return memcmp(_Magic, _MagicConst, sizeof(_Magic)) == 0;
	}
	RMUAnswerWrapper * Analyse(const char * word);
#ifdef RMU_UTIL
	RMUAnswerWrapper * ProcessQuery(const char * query);
#endif
private:
	RMUWrapper();
};

#endif