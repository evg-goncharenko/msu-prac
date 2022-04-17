// RMUDLL.cpp : Defines the exported functions for the DLL application.
//

///////////////////////////////////////////////////////////////////////////
//
//	History
//	09.10.2012
//		two versions: 0.1.1.x - for distribution and 0.1.2.x for utilities
//						(RMU_UTIL symbol defined). Interface to utils version - RMUUtilDll.h
//		Encode function and XML2BIN project added
//	11.10.2012
//		Iterator added to utils version
//	30.10.2012
//		error with generation of adjectives XML fixed
//		RMUAddLexeme and RMUSaveFile fixed
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Lib/FileConfig.h"
#include "Lib/Xml.h"
#include "RMUDLLPriv.h"
//#include "RMUResult.h"
//#include "RMUResultPriv.h"
//#include <algorithm>

#ifdef WIN32
#define strdup _strdup
#endif

static char* GetFormattedXml(char* xml);

///////////////////////////////////////////////////////////////////////////
//
//		DLL export
//
///////////////////////////////////////////////////////////////////////////

int RMUInit(RMUHandle* hRmu) {
    return RMUWrapper::Create((RMUHandle&)(*hRmu));
}

static inline bool CheckInvalidMemory(RMUWrapper* pRMU) {
    return pRMU == 0 || !pRMU->Check();
}

int RMUFree(RMUHandle hRMU) {
    RMUWrapper* pRMU = (RMUWrapper*)hRMU;
    if (CheckInvalidMemory(pRMU)) return RMU_ERROR_INV_HANDLE;
    delete pRMU;
    return RMU_OK;
}

int RMUGetAnswer(RMUHandle hRMU, const char* word, RMUAnswer* hAnsw) {
    RMUWrapper* pRMU = (RMUWrapper*)hRMU;
    if (CheckInvalidMemory(pRMU)) return RMU_ERROR_INV_HANDLE;
    *hAnsw = pRMU->Analyse(word);
    return (*hAnsw != 0 ? RMU_OK : RMU_ERROR_UNKNOWN);
}

#ifdef RMU_UTIL
int RMUProcessQuery(RMUHandle hRMU, const char* query, RMUAnswer* hAnsw) {
    RMUWrapper* pRMU = (RMUWrapper*)hRMU;
    if (CheckInvalidMemory(pRMU)) return RMU_ERROR_INV_HANDLE;
    *hAnsw = pRMU->ProcessQuery(query);
    return (*hAnsw != 0 ? RMU_OK : RMU_ERROR_UNKNOWN);
}
#endif

static inline bool CheckInvalidMemory(RMUAnswerWrapper* pAnswer) {
    return pAnswer == 0 || !pAnswer->Check();
}

int RMUGetXML(RMUAnswer hAnsw, char* pBuffer, size_t bufSize, int formatted) {
    RMUAnswerWrapper* pAnsw = (RMUAnswerWrapper*)hAnsw;
    if (CheckInvalidMemory(pAnsw)) return RMU_ERROR_INV_HANDLE;
    const char* pText = pAnsw->GetXML(formatted);
    if (!pText)
        return RMU_ERROR_UNKNOWN;
    int len = strlen(pText) + 1;
    if (!pBuffer)
        return len;
    else if (bufSize >= (size_t)len)
        strcpy(pBuffer, pText);
    else
        return RMU_BUFFER_TOO_SMALL;
    return RMU_OK;
}

int RMUFreeAnswer(RMUAnswer hAnsw) {
    RMUAnswerWrapper* pAnsw = (RMUAnswerWrapper*)hAnsw;
    if (CheckInvalidMemory(pAnsw)) return RMU_ERROR_INV_HANDLE;
    delete pAnsw;
    return RMU_OK;
}

#if 0
// functions for handling RMUResult

int RMUGetResult(RMUAnswer hAnsw, const RMUResult ** pResult)
{
	RMUAnswerWrapper * pAnsw = (RMUAnswerWrapper*)hAnsw;
	if (CheckInvalidMemory(pAnsw)) return RMU_ERROR_INV_HANDLE;
	*pResult = pAnsw->GetResultArray();
	return pAnsw->GetResultCount();
}

const char * RMUGetSyntClassString(const RMUResult * pResult)
{
	return GetSyntClassString(pResult);
}

// return copy of character representation of the answer (in Russian Win-1251)
// the answer string should be freed using free() library call
/*
char * RMUGetStringRep(const RMUResult * pResult)
{
	string rep = pResult->GetStringRep();
	return _strdup(rep.c_str());
}
*/
//	Helper function for grammar table parser
int RMUFindSyntClassId(const char * className)
{
	return FindSyntClassId(className);
}
#endif

///////////////////////////////////////////////////////////////////////////
//
//		RMUAnswerWrapper
//
///////////////////////////////////////////////////////////////////////////

const char RMUAnswerWrapper::_MagicConst[] = "RMUA101";

RMUAnswerWrapper::RMUAnswerWrapper(const String& answer) {
    _pAnswer = strdup(answer.c_str());
    memcpy(_Magic, _MagicConst, sizeof(_Magic));
}

// since ReadFrom method (or some other stuff) change xml (lowercasing of all names & attributes)
// and ParseXMLToResult() method is written in lower-casing
//	we need some patch method to make all work together (not very beautiful)
static char* GetFormattedXml(char* xml) {
    const char* pBeg = xml;
    while (*pBeg && *pBeg == ' ') ++pBeg;  // skip spaces
    if (*pBeg != '<')                      // XML start
        return strdup(xml);
    String XmlString(xml);
    XMLElement* pXml = XMLElement::ReadFrom(XmlString);
    char* retval = strdup(pXml->Serialize().c_str());
    delete pXml;
    return retval;
}

const char* RMUAnswerWrapper::GetXML(int formatted) {
    if (formatted) {
        char* formattedXml = GetFormattedXml(_pAnswer);
        free(_pAnswer);
        _pAnswer = formattedXml;
    }
    return _pAnswer;
}

RMUAnswerWrapper::~RMUAnswerWrapper() {
    free(_pAnswer);
}

///////////////////////////////////////////////////////////////////////////
//
//		RMUWrapper
//
///////////////////////////////////////////////////////////////////////////
#ifndef WIN32
#define MAX_PATH 256
#endif

bool RMUWrapper::_initialised = false;
const char RMUWrapper::_MagicConst[] = "RMUW101";
char RMUWrapper::_BASE_FILE_NAME[MAX_PATH] = "rm";
IRMStockManager::FileFormat RMUWrapper::_load_format = IRMStockManager::SM_FORMAT_BINARY;

int RMUWrapper::Create(RMUHandle& hRMU) {
    try {
        RMUWrapper* pRes = new RMUWrapper();
        hRMU = pRes;
    } catch (...) {
        return RMU_ERROR_UNKNOWN;
    }
    return RMU_OK;
}

RMUAnswerWrapper* RMUWrapper::Analyse(const char* word) {
    String request = "<analyze form=\"" + String(word) + "\"/>\r\n";
    String answer;

    try {
        answer = _pUnit->ProcessQuery(request);
    } catch (Xception* pErr) {
        answer = pErr->GetNotification();
    }
    return RMUAnswerWrapper::Create(answer);
}

RMUWrapper::RMUWrapper() {
    if (!_initialised) {
        Initialise();
        _initialised = true;
    }
    memcpy(_Magic, _MagicConst, sizeof(_Magic));
    _pUnit = IRMUnit::Create();
    _pUnit->LoadFromFile(_BASE_FILE_NAME, _load_format);
}

void RMUWrapper::Initialise() {
    FileConfig Cfg;
    String Value;

    if (Cfg.Open("RMU.cfg")) {
        if (Cfg.Read("loadformat", Value)) _load_format = Value == "XML" ? (IRMStockManager::SM_FORMAT_XML) : (IRMStockManager::SM_FORMAT_BINARY);
        if (Cfg.Read("basefilename", Value)) strcpy(_BASE_FILE_NAME, Value.c_str());
    }
}

#ifdef RMU_UTIL
RMUAnswerWrapper* RMUWrapper::ProcessQuery(const char* query) {
    String answer;

    try {
        answer = _pUnit->ProcessQuery(query);
    } catch (Xception* pErr) {
        answer = pErr->GetNotification();
    }
    return RMUAnswerWrapper::Create(answer);
}
#endif
