#include "stdafx.h"
#ifdef RMU_UTIL
#include "RMUUtilDLL.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <stdarg.h>

#include "Lib/Xception.h"
#include "Lib/SCArray.h"
#include "Lib/XML.h"
#include "Lib/FileStream.h"
#include "RMStockManager.h"


struct pclass
{
	//pclass (const std::string& word, int cnt, const std::string& sample) : UID(word), count(cnt), firstWord(sample) {} 
	pclass () : count(0) { }
	std::string UID;
	int count;
	std::string firstWord;
};

typedef std::map<std::string, pclass> dict;

bool operator < (const pclass& p1, const pclass& p2)
{
	return p1.count < p2.count || (p1.count == p2.count && p1.UID < p2.UID);
}

static void PClassFreq()
{
	const std::string static_word = "static_word";
	IRMStockManager * pStock = IRMStockManager::Create();
	pStock->LoadFromFile("rm", IRMStockManager::SM_FORMAT_BINARY);
	dict pclasses;
	String stem;
	int i = 0;
	int cnt = 0;
	while (pStock->GetStem(i++, stem))
	{
		CRMLexems * pLexems = pStock->FindCognates(stem);
		for (int j = 0; j < pLexems->Count(); ++j)
		{
			CRMLexem& Lexeme = *(*pLexems)[j];
			std::string word;
			if (Lexeme.Paradigm && Lexeme.Paradigm->Items.Count() > 0)
			{
				word = Lexeme.Paradigm->UID;

			}
			else
				word = static_word;
			pclass& pcl = pclasses[word];
			if (pcl.count == 0) 
			{
				pcl.UID = word;
				pcl.firstWord = Lexeme.UID;
			}
			++pcl.count;
			++cnt;			
		}
		delete pLexems;
	}
	std::vector <pclass> v;
	for (dict::iterator it = pclasses.begin(); it != pclasses.end(); ++it)
		v.push_back(it->second);
	printf("%d p-classes for %d lexemes found\n----------------------------------\n", pclasses.size(), cnt);typedef std::map<std::string, int> dict;
typedef std::map<std::string, int> dict;
typedef std::map<std::string, int> dict;

	std::stable_sort(v.begin(), v.end());
	int c = 0;
	int num = 0;
	for (std::vector <pclass>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it,++num)
		printf("%5d%20s\t%20s\t%d\t%f\t %f\n", num, it->UID.c_str(), it->firstWord.c_str(), it->count, ((double)it->count*100)/cnt, ((double)(c+=it->count)*100)/cnt);
}

static void AddWordsWithoutPClass(const char * FileNamePrefix, const char * WordFileName);
static void AddWords(const char * FileNamePrefix, const char * WordFileName);
static void CheckStressSchemas(const char * FileNamePrefix);
static void CheckLemmas(const char * FileNamePrefix);

void RMUUtils(RMU_UTIL_CODE Number, ...)
{
	switch (Number)
	{
	case UC_PCLASS_FREQ:
		PClassFreq();
		break;
	case UC_ADD_LEXEME:
		{
			va_list args;
			va_start(args, Number);
			const char * pFileNamePrefix = "rm";
			const char * pWordFileName = "addwordlist.txt";
			const char * arg;
			arg = va_arg(args, const char * );
			if (arg != 0)
			{
				pFileNamePrefix = arg;
				arg = va_arg(args, const char * );
				if (arg != 0)
					pWordFileName = arg;
			}
			va_end(args);
			AddWords(pFileNamePrefix, pWordFileName);
		}
		break;
	case UC_CHECK_STRESS_SCHEMAS:
		{
			va_list args;
			va_start(args, Number);
			const char * pFileNamePrefix = "rm";
			const char * arg;
			arg = va_arg(args, const char * );
			if (arg != 0)
			{
				pFileNamePrefix = arg;
			}
			va_end(args);
			CheckStressSchemas(pFileNamePrefix);
		}
		break;
	case UC_CHECK_LEMMAS:
		{
			va_list args;
			va_start(args, Number);
			const char * pFileNamePrefix = "rm";
			const char * arg;
			arg = va_arg(args, const char * );
			if (arg != 0)
			{
				pFileNamePrefix = arg;
			}
			va_end(args);
			CheckLemmas(pFileNamePrefix);
		}
		break;
	}
}

////////////////////////////////////////////////////////////
// format of file with words:
// lexeme synt_class_number stress_scheme_id anytext
// for example:
//        многовато 13  S7//
////////////////////////////////////////////////////////////
void AddWordsWithoutPClass(const char * FileNamePrefix, const char * WordFileName)
{
	IRMStockManager * pStock = IRMStockManager::Create();
	pStock->LoadFromFile(FileNamePrefix, IRMStockManager::SM_FORMAT_XML);
	FILE * pInput = fopen(WordFileName, "r");
	if (!pInput)
	{
		fprintf(stderr, "Cannot open input file\n");
		return;
	}
	char buffer[1024];
	char sep[] = " /\n\r";
	while (fgets(buffer, sizeof(buffer),pInput))
	{
		char * pWord;
		pWord = strtok(buffer, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String Stem = pWord;

		pWord = strtok(NULL, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String SyntClass = pWord;

		
		pWord = strtok(NULL, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String SchemaID = pWord;

		String PClassID = "";

		pWord = strtok(NULL, sep);
		if (pWord)
		{
			PClassID = pWord;
		}

		pStock->AddLexem(SyntClass, Stem, PClassID, SchemaID);
	}
	pStock->SaveToFile("new"+String(FileNamePrefix), IRMStockManager::SM_FORMAT_XML);
}

////////////////////////////////////////////////////////////
// format of file with words:
// stem synt_class_number_code p_class_id stress_scheme_id //lemma anytext
// for example:
//	блокир 9_4 Verb4_2 SVerb4_5  //блокировать 5
////////////////////////////////////////////////////////////
void AddWords(const char * FileNamePrefix, const char * WordFileName)
{
	IRMStockManager * pStock = IRMStockManager::Create();
	pStock->LoadFromFile(FileNamePrefix, IRMStockManager::SM_FORMAT_XML);
	FILE * pInput = fopen(WordFileName, "r");
	if (!pInput)
	{
		fprintf(stderr, "Cannot open input file\n");
		return;
	}
	char buffer[1024];
	char sep[] = " /\n\r";
	while (fgets(buffer, sizeof(buffer),pInput))
	{
		char * pWord;
		pWord = strtok(buffer, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String Stem = pWord;

		pWord = strtok(NULL, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String SyntClass = pWord;

		pWord = strtok(NULL, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String PClassID = pWord;
		
		pWord = strtok(NULL, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String SchemaID = pWord;

		pWord = strtok(NULL, sep);
		if (!pWord)
		{
			fprintf(stderr, "bad string '%s' in input file\n", buffer);
			return;
		}
		String Lemma = pWord;


		CRMLexem * lex = pStock->AddLexem(SyntClass, Stem, PClassID, SchemaID);
		if (lex != NULL)
		{
			// check for lemma correctness
			int index = lex->UID.Find('_');
			String lexLemma;
			if (index < 0)
				lexLemma = lex->UID;
			else
				lexLemma = lex->UID.SubStr(0,index);
			if (lexLemma != Lemma)
			{
				fprintf(stderr, "Incompatible lemmas '%s' and '%s' in string\n '%s'\n", pWord, lexLemma.c_str(), buffer);
				return;
			}
		}
	}
	pStock->SaveToFile("new"+String(FileNamePrefix), IRMStockManager::SM_FORMAT_XML);
}

void CheckStressSchemas(const char * FileNamePrefix)
{
	IRMStockManager * pStock = IRMStockManager::Create();
	pStock->LoadFromFile(FileNamePrefix, IRMStockManager::SM_FORMAT_XML);
	if (!pStock->CheckStressSchemas())
		fprintf(stderr, "There were errors in stress schemas\n");
}

void CheckLemmas(const char * FileNamePrefix)
{
	IRMStockManager * pStock = IRMStockManager::Create();
	pStock->LoadFromFile(FileNamePrefix, IRMStockManager::SM_FORMAT_XML);
	if (!pStock->CheckLemmas())
		fprintf(stderr, "There were errors in stems or lemmas\n");
}


#endif
