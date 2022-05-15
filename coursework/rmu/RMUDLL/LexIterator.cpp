#include "stdafx.h"

#ifdef RMU_UTIL
#include "RMUUtilDLL.h"
#include "RMUDLLPriv.h"

#include "Lib/Xception.h"
#include "Lib/SCArray.h"
#include "Lib/Xml.h"
#include "Lib/FileStream.h"
#include "RMStockManager.h"

class LexIterator
{
	IRMStockManager * _pStock;
	char * _buffer;
	int _buffer_size;
	CRMLexems _lexems;
	LexIterator(const char * BaseFileName)
	{
		_pStock = IRMStockManager::Create();
		_pStock->LoadFromFile(BaseFileName, IRMStockManager::SM_FORMAT_BINARY);
		_buffer_size = 0;
		_buffer = NULL;
	}
	~LexIterator()
	{
		delete [] _buffer;
	}
public:
	static LexIterator * Create(const char * BaseFileName)
	{
		return new LexIterator (BaseFileName);
	}
	static void Delete(LexIterator * li)
	{
		delete li;
	}
};

///////////////////////////////////////////////////////////////////////////
//
//		DLL export
//
///////////////////////////////////////////////////////////////////////////

RMULexIterator CreateLexIterator(const char * FileName)
{
	return (RMULexIterator) LexIterator::Create(FileName);
}

void DeleteLexIterator(RMULexIterator it)
{
	LexIterator::Delete((LexIterator*)it);
}

void GetNextLexIterator(RMULexIterator it);

const char * GetLemma(RMULexIterator it);

static void ListLexeme(const String& stem, const CRMLexem& Lexeme, CSCDictionary<int>& lexems)
{

	String lemma;
	if (Lexeme.Paradigm && Lexeme.Paradigm->Items.Count() > 0)
	{
		lemma = stem + Lexeme.Paradigm->GetFirstFlex();
	}
	else
		lemma = stem;
	int dummy=0;
	if (!lexems.FindKey(lemma, dummy))
	{
		printf("%s %d %s\n", lemma.c_str(), Lexeme.GetSynClass(), Lexeme.GetSynClassName().c_str());
		lexems.Add(lemma, new int[1]);
	}

}

static void ListLexemes(const String& stem, const CRMLexems& Lexems, CSCDictionary<int>& lexems)
{
	for (int i = 0; i < Lexems.Count(); ++i)
	{
		ListLexeme(stem, *Lexems[i], lexems);
	}
}

void RMUListLexems(const char * FileName)
{
	IRMStockManager * pStock = IRMStockManager::Create();
	pStock->LoadFromFile(FileName, IRMStockManager::SM_FORMAT_BINARY);
	String stem;
	CSCDictionary<int> lexems;
	int i = 0;
	while (pStock->GetStem(i++, stem))
	{
		CRMLexems * pLexems = pStock->FindCognates(stem);
		ListLexemes(stem, *pLexems, lexems);
		delete pLexems;
	}
}

#endif
