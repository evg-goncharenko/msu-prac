#include "stdafx.h"

#include "RMUUtilDLL.h"

#ifdef RMU_UTIL

#include "Lib/String.h"
#include "RMUnit.h"
#include "RMStockManager.h"

static IRMUnit * pUnit;

static void Initialise()
{
	pUnit = IRMUnit::Create();
}

int RMUDecode(const char * BaseName)
{
	Initialise();
	String FileName(BaseName);
	pUnit->LoadFromFile(BaseName, IRMStockManager::SM_FORMAT_BINARY);
	pUnit->SaveToFile("xml"+FileName, IRMStockManager::SM_FORMAT_XML);
	return 0;
}

int RMUEncode(const char * BaseName)
{
	Initialise();
	String FileName(BaseName);
	pUnit->LoadFromFile(BaseName, IRMStockManager::SM_FORMAT_XML);
	pUnit->SaveToFile(FileName, IRMStockManager::SM_FORMAT_BINARY);
	return 0;
}

#endif
