#if defined(RMU_UTIL) && !defined(__RMU_UTIL_INCLUDED__)
#define __RMU_UTIL_INCLUDED__
#include "RMUDLL.h"
#ifdef __cplusplus
extern "C" {
#endif
DECLSPEC int RMUEncode(const char * BaseName);
DECLSPEC int RMUDecode(const char * BaseName);
typedef void * RMULexIterator;
DECLSPEC RMULexIterator CreateLexIterator(RMUHandle hRMU);
DECLSPEC void DeleteLexIterator(RMULexIterator it);
DECLSPEC void GetNextLexIterator(RMULexIterator it);
DECLSPEC const char * GetLemma(RMULexIterator it);
// analyse the query (pseudo-xml) (in win-1251 encoding) and return handle to RMUAnswer
DECLSPEC int RMUProcessQuery(RMUHandle hRMU, const char * query, RMUAnswer * hAnsw);


DECLSPEC void RMUListLexems(const char * FileName);


enum RMU_UTIL_CODE
{
	UC_UNDEFINED = 0,
	UC_PCLASS_FREQ = 1,
	UC_ADD_LEXEME = 2,
	UC_CHECK_STRESS_SCHEMAS = 3,
	UC_CHECK_LEMMAS = 4,
	// add new items here
	UC_LAST
};

DECLSPEC void RMUUtils(RMU_UTIL_CODE Number, ...);
DECLSPEC bool RMUAddToken(RMUHandle hRMU, const char * SyntClass, const char * Stem, const char * PClassID, const char * SchemaID);
DECLSPEC bool RMUSaveFile(RMUHandle hRMU, const char * FileNamePrefix);
#ifdef __cplusplus
} // extern "C"
#endif
#endif

