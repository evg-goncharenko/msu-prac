#ifndef __RMUDLL_H__
#define __RMUDLL_H__

#ifdef WIN32
#ifdef RMUDLL_EXPORTS
#define DECLSPEC	__declspec (dllexport)
#else
#define DECLSPEC	__declspec (dllimport)
#endif
#else
#define DECLSPEC
#endif

typedef void * RMUHandle;
typedef void * RMUAnswer;
//typedef struct RMUResult * PRMUResult;

#define RMU_OK 0
#define RMU_ERROR_DATAFILE		-1
#define RMU_ERROR_INV_HANDLE	-2
#define RMU_ERROR_UNKNOWN		-3
#define RMU_BUFFER_TOO_SMALL	-4
#define RMU_BAD_ARG				-5
#define RMU_ERROR_LAST			RMU_BAD_ARG
#include <iostream>
// initialise RMU structures 
#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////////
// 
//		RMUInit
//	Initialize all structures, load data files, allocate handle etc
//	Return value - RMU_OK if success, error code otherwise (see RMU_XXXX_XXXX constants)
//
//////////////////////////////////////////////////////////////////////////////
DECLSPEC int RMUInit(RMUHandle * hRMU);

//////////////////////////////////////////////////////////////////////////////
// 
//		RMUFree
//	Free RMU structures, deallocate handle etc
//  Return value - RMU_OK if success, RMU_ERROR_INV_HANDLE if handle or structure is corrupted 
//
//////////////////////////////////////////////////////////////////////////////
DECLSPEC int RMUFree(RMUHandle hRMU);

//////////////////////////////////////////////////////////////////////////////
// 
//		RMUGetAnswer
//	Analyse the word (in win-1251 encoding) and return handle to RMUAnswer
//	Return value - RMU_OK if success, error coded otherwise (see RMU_XXXX_XXXX constants) 
//
////////////////////////////////////////////////////////////////////////////// 
DECLSPEC int RMUGetAnswer(RMUHandle hRMU, const char * word, RMUAnswer * hAnsw);

//////////////////////////////////////////////////////////////////////////////
// 
//		RMUGetXML
// get SGML (in win-1251 encoding) from analysed answer
// writes SGML text to pBuffer. pBuffer shuld be allocated by calling proc, 
// bufSize should be large enough to get string with zero byte end
// returns the number of bytes written (including zero byte)
// if pBuffer is NULL then the length of buffer is returned
//
//////////////////////////////////////////////////////////////////////////////
DECLSPEC int RMUGetXML(RMUAnswer hAnsw, char * pBuffer, std::size_t bufSize, int formatted);


//////////////////////////////////////////////////////////////////////////////
// 
//		RMUFreeAnswer
//  Free answer data
//  Return value - RMU_OK if success, RMU_ERROR_INV_HANDLE if handle or structure is corrupted 
//
//////////////////////////////////////////////////////////////////////////////
DECLSPEC int RMUFreeAnswer(RMUAnswer hAnsw);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
