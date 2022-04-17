/*____________________________________________________________________

  Project:

      Russian  Morphology   Unit
      http://axofiber.org.ru/rm/
      __________________________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru

  (C) Volkova Irina,                              1982--2003, AMC, MSU
      volkova@cs.msu.su

  Component:  Main unit
  ____________________________________________________________________

  04.XI.2003                                                     01.02
      Ported on new XML unit.
  23.IV.2003                                                     01.01
      Reorganized "ProcessQuery" method onto several private
      methods. Added "AdaptCaps", "AdaptSpell", "Decompose" methods.
  9.VI.2002                                                      01.00
      Fixed a lot of problems with nested shemas and paradigms,
      lexems without paradigm and so worth. Implemented full 
      featured stress schemas suppport.
  2.VI.2002                                                      00.10
      Fixed "CRMUnit::ProcessQuery" method to eliminate "empty flex"
      bug.
  14.II.2002                                                     00.09
      Upgraded to the "String" STL class. Fixed "ProcessQuery"
      method's legacy errors (bad element name "conju ct ion").
  05.II.2002                                                     00.08
      Invariable lexems now have no link to p-class ("Cl_0" issue
      is fixed).
  16.XI.2001                                                     00.07
      Finished "synthesis" block in "ProcessQuery" method.
  15.XI.2001                                                     00.06
      Added "ё" letter heuristic adaptation support in the
      "ProcessQuery" method. Fixed analysis part and changed the
      "answer" String format.
  05.XI.2001                                                     00.05
      Renamed "CRMAnalyzer" to "CRMUnit".
  10.IX.2001                                                     00.04
      Updated function "Analyze" upto upto CRMAdverb handling.
  03.IX.2001                                                     00.03
      Updated function "Analyze" upto CRMPronounAdjective class
      handling.
  09.VIII.2001                                                   00.02
      Added function "GetPronounParadigmsFor"
  ____________________________________________________________________
*/

#include "RMUnit.h"
#include "Lib/Xception.h"
#include "Lib/Xml.h"
#include "Lib/stdafx.h"

struct Prediction {
    String PredictedLexem;
    CSCSet<String, Strings> Hypothesises;
};

/*
const char RRMUnit::Suffixes[256][8] =
{
    // Diagnostic
    "aжн", "агн", "анин", "ател", "атор", "афи", "ба", "ащ", "вн", "граф",
    "онок", "етел", "етр", "ец", "етри", "еват", "есят", "ива", "игa", "идн",
    "изм", "ител", "истк", "иц", "йш", "ктор", "ла", "нича", "нн", "нн",
    "нт", "обн", "ова", "оват", "оги", "олог", "онок", "ост", "разы", "сн",
    "сот", "ств", "тн", "ува", "ушк", "ущ", "ци", "цк", "чат", "чив", "чик",
    "чн", "щик", "ыва", "ющ", "яни", "янин", "ятел", "ятор", "ящ",
    // Semidiagnostic
    "ан", "енк", "ени", "ист", "ик", "льн", "онк", "ск",
    // Diagnostic male
    "анин", "ател", "атор", "граф", "енок", "етел", "етр", "ец", "изм",
    "ител", "ктор", "нт", "олог", "онок", "чик", "щик", "янин", "ятел",
    "ятор", 
    //       semi
    "енк", "ик", "ист", "онк", "ск",
    // --"-- female
    "афи", "етри", "истк", "иц", "оги", "ост", "ушк", "ци",
    //       semi
    "енк", "ик", "льн", "онк", "ск",
    // --"-- neuter
    "ств", "яни",
    //       semi
    "али", "ени",
    //         semi
    "ист", "льн", "ск",
    //         semi
    "ани", "ени",
};

*/

/*____________________________________________________________________

                                                                    /
    CRMUnit class definition                                    \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMUnit : public IRMUnit {
public:
    // Life-cycle
    CRMUnit() : BusyStatus(0),
                Stock(IRMStockManager::Create()){};
    ~CRMUnit();
    // Interface
    int GetBusyStatus() const { return Stock->GetBusyStatus(); };
    int FormsCount() const { return Stock->FormsCount(); };
    int LexemsCount() const { return Stock->LexemsCount(); };
    void LoadFromFile(const String& BaseName, IRMStockManager::FileFormat Format = IRMStockManager::SM_FORMAT_BINARY) { Stock->LoadFromFile(BaseName, Format); }
    int PClassesCount() const { return Stock->PClassesCount(); };
    String ProcessQuery(const String& Query);
    void SaveToFile(const String& BaseName, IRMStockManager::FileFormat Format = IRMStockManager::SM_FORMAT_BINARY) { Stock->SaveToFile(BaseName, Format); }
    int StressSchemasCount() const { return Stock->StressSchemasCount(); }

private:
    void AdaptCaps(String Form, Strings& Forms) const;
    void AdaptSpell(const String& Form, Strings& Forms) const;
    // GIG - 04.03.2013 - extra parameter for comparative degree
    void EncodeLexem(CRMLexem* Lexem, String& Result, const String& From, const String& Stem, const String& Flex, int StartPos = 0) const;
    // GIG - 13.01.2013
    //bool              HasPrefix     ( const String& Form, int & PrefixIndex ) const;
    String ProcessAnalyzeQuery(XMLElement* XML) const;
    String ProcessAnalyzeNewQuery(XMLElement* XML) const;
    String ProcessFindCognates(XMLElement* XML) const;
    String ProcessSynthesizeQuery(XMLElement* XML) const;
    String ProcessShowPClass(XMLElement* XML) const;
    String ProcessShowPClasses(XMLElement* XML) const;
    // Attributes
    static const char* Version;
    static const char* Build;
    int BusyStatus;
    IRMStockManager* Stock;
    // GIG - 13.01.2013
    //static  const char        Prefixes[64][8];
    //static  const int         PrefixesQnt;
};

const char* CRMUnit::Version = "RMU v3.4";
const char* CRMUnit::Build = "55/" __DATE__;

/* GIG - 11.01.2013
const int    CRMUnit::PrefixesQnt = 31;
const char   CRMUnit::Prefixes[64][8] =
{
    "по-", "в-", "во-",
    "вз", "вы", "до", "за",   "из",   "ис",   "на",   "над", "низ", "недо",
    "по", "об", "от", "пере", "пре",  "пред", "при",  "про", "под", "раз",
    "рас",
    "навы", "напод", "повы", "пона", "вс", "пораз", "проза",
};
*/

/*____________________________________________________________________
                                                                  _
                                                                _| |_
    CRMUnit make function                                      |_   _|
                                                                 |_|
  ____________________________________________________________________
*/

IRMUnit*
IRMUnit::Create() {
    return new CRMUnit;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    AdaptCaps                                                 | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

void CRMUnit::AdaptCaps(String Form, Strings& Forms) const {
    if (192 <= unsigned(Form[0]) && unsigned(Form[0]) <= 223) {
        // Store first variant - first capital, rest - lowers
        MakeLower(Form, 1);
        Forms.Add(Form);
    }
    // Store second variant - all lowers
    MakeLower(Form);
    Forms.Add(Form);
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    AdaptSpell                                                | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

void CRMUnit::AdaptSpell(const String& Form, Strings& Forms) const {
    String SubForm;
    int ReplPos;

    // Check first capital letter
    AdaptCaps(Form, Forms);
    // Heurisitic adaptation analysis
    ReplPos = Form.Find("е");
    if (Form.Find("ё") == -1 && 0 <= ReplPos)
        do {
            // Make several form patterns with "e" letter replaced by "ё"
            SubForm = Form;
            SubForm[ReplPos] = 'ё';
            // Check first capital letter
            AdaptCaps(SubForm, Forms);
            // Prepare next pattern
            ReplPos = Form.Find("е", ReplPos + 1);
        } while (ReplPos != -1);
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    Decompose                                                 | (o)  |
                                                              |______|
  ____________________________________________________________________
*/
/*
CResolutions * 
CRMUnit::Decompose ( const String& Form ) const
{
    CResolutions * Result = new CResolutions;
    CResolution Resolution;
    int   Pos, CurrentPrefix, CurrentSuffix;
    String Prefix, Stem, Suffix, Affix;
    int PrefixQty = 31; 
    int SuffixQty = 146;

    if ( !Form.IsEmpty () )
    {
        for ( CurrentPrefix = 0; CurrentPrefix < PrefixQty; ++CurrentPrefix )
        {
            Prefix = Prefixes [CurrentPrefix];
            if ( FastFind ( Form, Prefix, 0 ) == 0 )
            {
                for ( CurrentSuffix = 0; CurrentSuffix < SuffixQty; ++CurrentSuffix )
                {
                    Resolution.Stem.Clear ();
                    Resolution.Affix.Clear ();
                    Suffix = Suffixes [CurrentSuffix];
                    if ( 0 < ( Pos = FastFind ( Form, Suffix, 1 ) ) )
                    {
                        if ( Prefix.Length () <= Pos )
                        {
                            // Found pre-suf combination
                            Resolution.Prefix = Prefix;
                            Resolution.Stem   = Form.SubStr ( Prefix.Length (), Pos - Prefix.Length () );
                            Resolution.Suffix = Suffix;
                            Resolution.Affix  = Form.SubStr ( Pos + Suffix.Length (), Form.Length () - Pos - Suffix.Length () );
                        }
                        else
                        {
                            // Found only suffix
                            Resolution.Prefix.Clear ();
                            Resolution.Stem   = Form.SubStr ( 0, Pos );
                            Resolution.Suffix = Suffix;
                            Resolution.Affix  = Form.SubStr ( Pos + Suffix.Length (), Form.Length () - Pos - Suffix.Length () );
                        }
                    }
                    else
                    {
                        // Found only prefix
                        Resolution.Prefix = Prefix;
                        Resolution.Stem   = Form.SubStr ( Prefix.Length (), Form.Length () - Prefix.Length () );
                        Resolution.Suffix.Clear ();
                        Resolution.Affix.Clear ();
                    }
                    Result->Add ( Resolution );
                }
            }
        }

    }
    return Result;
}
*/
/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    EncodeLexem                                               | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

void CRMUnit::EncodeLexem(CRMLexem* Lexem, String& Result, const String& Form, const String& Stem, const String& Flex, int StartPos) const {
    int k;
    CRMParadigm::FlexPositions* FlexPositions = NULL;

    try {
        if (Lexem->Paradigm && 0 < Lexem->Paradigm->Items.Count()) {
            Result += "<analyzed form=\"" + Form + "\" lexem=\"" + Stem + Lexem->Paradigm->GetFirstFlex() + "\" />";
            FlexPositions = Lexem->Paradigm->Contains(Flex, StartPos);
        } else {
            CRMParadigm::FlexVar FV;
            FV.Pos = 0;
            FV.Var = 0;
            Result += "<analyzed form=\"" + Form + "\" lexem=\"" + Stem + "\" />";
            FlexPositions = new CRMParadigm::FlexPositions;
            FlexPositions->Items.Add(FV);
        }
        if (FlexPositions) {
            for (k = 0; k < FlexPositions->Items.Count(); ++k)
                Result += CompleteMultiStringResult(Lexem->IdentifyFlex(FlexPositions->Items[k], Form));
            ++Lexem->UsageCounter;
            if (1999999999 < Lexem->UsageCounter) Stock->UpdateCounters();
        }
    }
    finally (
        if ( FlexPositions ) delete FlexPositions;
    )
}
    /*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ProcessAnalyzeQuery                                       |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

    String
    CRMUnit::ProcessAnalyzeQuery(XMLElement* XML) const {
        String Result;
        String Form;
        String ThisStem;
        String ThisFlex;
        Strings Forms;
        CRMLexems* Lexems;
        int Pos, Length;
        bool IsPossiblyComparativeMode;
        bool Found;
        int k;

        struct CompDegreeHackHelper {
            CompDegreeHackHelper() {
                CRMSclasses::CRMAdjective::AdjectiveHomonim::WeakDegreeHack(true);
            }
            ~CompDegreeHackHelper() {
                CRMSclasses::CRMAdjective::AdjectiveHomonim::WeakDegreeHack(false);
            }
        };

        if (!XML->Attr("form").IsEmpty()) {
            int i;
            if (XML->Attr("match") == "spell")
                Forms.Add(XML->Attr("form"));
            else
                AdaptSpell(XML->Attr("form"), Forms);
            for (k = 0; k < Forms.Count(); k++) {
                Form = Forms[k];
                Pos = Length = Form.Length();
                IsPossiblyComparativeMode = false;
                Found = false;
                if (4 < Length)
                    IsPossiblyComparativeMode = (Form.SubStr(0, 2) == "по");
                while (0 <= Pos) {
                    ThisStem = Form.SubStr(0, Pos);
                    ThisFlex = Form.SubStr(Pos, Length - Pos);
                    Lexems = Stock->FindLexems(ThisStem, ThisFlex);
                    if (Lexems->Count()) Found = true;
                    for (i = 0; i < Lexems->Count(); ++i)
                        EncodeLexem((*Lexems)[i], Result, Form, ThisStem, ThisFlex);
                    delete Lexems;
                    // GIG - 04.03.2013 - search for comparative adjectives completely redesigned
                    if (IsPossiblyComparativeMode && 2 <= ThisStem.Length()) {
                        CompDegreeHackHelper hack;
                        ThisStem = ThisStem.SubStr(2, ThisStem.Length() - 2);
                        Lexems = Stock->FindComparativeLexems(ThisStem, ThisFlex);
                        if (Lexems->Count()) Found = true;
                        for (i = 0; i < Lexems->Count(); ++i)
                            EncodeLexem((*Lexems)[i], Result, Form, ThisStem, ThisFlex);
                        delete Lexems;
                    }
                    --Pos;
                }
                if (!Found && 0 < Form.Find("-")) {
                    // Analyze compound adjectives
                }
            }
        }
        return Result;
    }

    /*____________________________________________________________________
                                                                 __
                                                               _|__|_
    HasPrefix                                                 | (o)  |
                                                              |______|
  ____________________________________________________________________
*/
    /* GIG - 13.01.2013
bool
CRMUnit::HasPrefix ( const String& Form, int & PrefixIndex ) const
{
    bool Result = false;
    
    while ( !Result && PrefixIndex < PrefixesQnt )
    {
        if ( Form.Find  ( Prefixes[PrefixIndex], 0 ) == 0 )
            Result = true;
        else
            ++PrefixIndex;
    }
    return Result;
}
*/

    /*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ProcessAnalyzeNewQuery                                    |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

    String
    CRMUnit::ProcessAnalyzeNewQuery(XMLElement* XML) const {
        String Result;
        Strings preResult;
        CSCPtrArray<Prediction> Answers;
        Prediction* pAnswer;
        String Form;
        String ThisStem;
        String StemTail, StemTailShort;
        String ThisFlex;
        String line;
        Strings Forms;
        CRMPClasses* PClasses;
        int Pos, Length;
        int i, j, k, h, AttrPos, p;

        if (XML->Attrs()->Exists("form", AttrPos) && !(*XML->Attrs())[AttrPos].Value.IsEmpty()) {
            if (XML->Attrs()->Exists("match", AttrPos) && (*XML->Attrs())[AttrPos].Value == "spell")
                Forms.Add(XML->Attr(AttrPos));
            else
                AdaptSpell(XML->Attr(AttrPos), Forms);
            for (k = 0; k < Forms.Count(); k++) {
                Form = Forms[k];
                if (3 < (Length = Form.Length())) {
                    Pos = 3;
                    while (Pos <= Length) {
                        StemTail = Form.SubStr(Pos - 3, 3);
                        StemTailShort = Form.SubStr(Pos - 2, 2);
                        if (Pos == Length) {
                            ThisStem = Form;
                            ThisFlex.Clear();
                        } else {
                            ThisStem = Form.SubStr(0, Pos);
                            ThisFlex = Form.SubStr(Pos, Length - Pos);
                        }
                        PClasses = Stock->FindPClasses(ThisFlex);
                        if (PClasses->Count()) {
                            for (i = 0; i < PClasses->Count(); ++i) {
                                if ((*PClasses)[i]->SuffixesExt.Find(StemTail, p) || (*PClasses)[i]->Suffixes.Find(StemTailShort, p)) {
                                    line = "<predicted form=\"" + Form + "\" lexem=\"" + ThisStem + (*PClasses)[i]->GetFirstFlex() + "\" />";
                                    h = 0;
                                    while (h < Answers.Count())
                                        if (Answers[h]->PredictedLexem == line)
                                            break;
                                        else
                                            ++h;
                                    if (h == Answers.Count() || !Answers.Count()) {
                                        // New answer
                                        pAnswer = new Prediction;
                                        pAnswer->PredictedLexem = line;
                                        Answers.Add(pAnswer);
                                    }
                                    preResult = (*PClasses)[i]->IdentifyFlex(ThisFlex);
                                    for (j = 0; j < preResult.Count(); ++j)
                                        // preResult should be cleaned before append
                                        Answers[h]->Hypothesises.Add(preResult[j]);
                                }
                            }
                        }
                        delete PClasses;
                        ++Pos;
                    }
                }
            }
        }
        for (i = 0; i < Answers.Count(); ++i) {
            Result += Answers[i]->PredictedLexem;
            for (j = 0; j < Answers[i]->Hypothesises.Count(); ++j)
                Result += Answers[i]->Hypothesises[j] + " />";
        }
        return Result;
    }

    /*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ProcessFindCognates                                       | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

    String
    CRMUnit::ProcessFindCognates(XMLElement* XML) const {
        String Result;
        /*
    String          Stem;
    String          Form;
    Strings         Forms;
    CRMLexems*      Lexems;
    CRMLexems*      tempLexems;
    CRMParadigm::FlexPositions* FlexPositions;
    unsigned long   i, j, k, AttrPos, FormLength, PrefixIndex;

    if ( Attrs.Exists( "form", AttrPos ) && !Attrs[AttrPos].Value.IsEmpty () )
    {
        if ( Attrs.Exists ( "match", AttrPos ) && Attrs[AttrPos].Value == "spell" )
            Forms.Add ( Attrs[AttrPos].Value );
        else
            AdaptSpell ( Attrs[AttrPos].Value, Forms );
        for ( i = 0; i < Forms.Count (); ++i )
        {
            FormLength = Forms[i].Length ();


            if ( 4 < FormLength )
            {
                for ( j = FormLength - 1; FormLength - 3 < j; --j )
                {
                    Stem = Forms[i].SubStr ( 0, j );
                    Lexems = Stock->FindCognates ( Stem );
                    PrefixIndex = 0;
                    while ( HasPrefix ( Stem, PrefixIndex ) )
                    {
                        tempLexems = Stock->FindCognates ( Stem.SubStr ( strlen ( Prefixes[PrefixIndex] ), j - strlen ( Prefixes[PrefixIndex] ) ) );
                        if ( tempLexems )
                        {
                            Lexems->Append ( *tempLexems );
                            delete tempLexems;
                        }
                        ++PrefixIndex;
                    }
                    for ( k = 0; k < Lexems->Count (); ++k )
                    {
                        if ( (*Lexems)[k]->Paradigm )
                        {
                            Form = Stem + (*Lexems)[k]->Paradigm->GetFirstFlex ();
                            Result += "<found lexem=\"" + Form + "\" />";
                            FlexPositions = (*Lexems)[k]->Paradigm->Contains ( (*Lexems)[k]->Paradigm->GetFirstFlex () );
                        }
                        else
                        {
                            CRMParadigm::FlexVar FV;
                            FV.Pos = 0; FV.Var = 0;
                            Form = Stem;
                            Result += "<found lexem=\"" + Stem + "\" />";
                            FlexPositions = new CRMParadigm::FlexPositions;
                            FlexPositions->Items.Add ( FV );
                        }
                        if ( FlexPositions )
                        {
                            for ( j = 0; j < FlexPositions->Items.Count (); ++j )
                            {
                                Result += (*Lexems)[k]->IdentifyFlex ( FlexPositions->Items[j], Form );
                                (*Lexems)[k]->UsageCounter++;
                                if ( (*Lexems)[k]->UsageCounter > 1999999999 ) Stock->UpdateCounters ();
                            }
                            delete FlexPositions;
                        }
                    }
                    delete Lexems;
                }
            }

        }
    }
*/
        return Result;
    }

    /*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ProcessSynthesizeQuery                                    | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

    String
    CRMUnit::ProcessSynthesizeQuery(XMLElement* XML) const {
        String Result;
        String Form;
        String ThisStem;
        String ThisFlex;
        Strings Forms;
        Strings* Flexes;
        CRMLexems* Lexems;
        int Pos, Length;
        int i, j, k;

        if (!XML->Attr("form").IsEmpty()) {
            if (XML->Attr("match") == "spell")
                Forms.Add(XML->Attr("form"));
            else
                AdaptSpell(XML->Attr("form"), Forms);
            for (j = 0; j < Forms.Count(); ++j) {
                Form = Forms[j];
                Pos = Length = Form.Length();
                while (0 <= Pos) {
                    ThisStem = Form.SubStr(0, Pos);
                    ThisFlex = Form.SubStr(Pos, Length - Pos);
                    Lexems = Stock->FindLexems(ThisStem, ThisFlex);
                    if (Lexems->Count()) {
                        CRMParadigm::FlexPositions* FlexPositions;
                        for (i = 0; i < Lexems->Count(); ++i) {
                            if ((*Lexems)[i]->Paradigm && 0 < (*Lexems)[i]->Paradigm->Items.Count()) {
                                Result += "<synthesized lexem=\"" + ThisStem + (*Lexems)[i]->Paradigm->GetFirstFlex() + "\" />";
                                Flexes = (*Lexems)[i]->Paradigm->GetAllFlexes();
                                FlexPositions = (*Lexems)[i]->Paradigm->GetAllFlexPositions();
                            } else {
                                Flexes = new Strings;
                                Flexes->Add("");
                                CRMParadigm::FlexVar FV;
                                FV.Pos = 0;
                                FV.Var = 0;
                                Result += "<synthesized lexem=\"" + ThisStem + "\" />";
                                FlexPositions = new CRMParadigm::FlexPositions;
                                FlexPositions->Items.Add(FV);
                            }
                            if (FlexPositions) {
                                for (k = 0; k < Flexes->Count(); ++k) {
                                    Result += "<form value=\"" + ThisStem + (*Flexes)[k] + "\" />";
                                    Result += CompleteMultiStringResult((*Lexems)[i]->IdentifyFlex(FlexPositions->Items[k], Form));
                                }
                                ++(*Lexems)[i]->UsageCounter;
                                if ((*Lexems)[i]->UsageCounter > 1999999999) Stock->UpdateCounters();
                            }
                            delete Flexes;
                            delete FlexPositions;
                        }
                    }
                    delete Lexems;
                    --Pos;
                }
            }
        }
        return Result;
    }

    /*____________________________________________________________________
                                                                 ___
                                                                /   \
    ProcessQuery                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

    String
    CRMUnit::ProcessQuery(const String& Query) {
        String ThisStem;
        String ThisFlex;
        String Form, SubForm, Result;
        XMLElement* XML = NULL;

        XML = XMLElement::ReadFrom(Query);
        try {
            if (XML && XML->IsElem()) {
                if (XML->Name() == "analyze")
                    Result = ProcessAnalyzeQuery(XML);
                else if (XML->Name() == "analyzenew")
                    Result = ProcessAnalyzeNewQuery(XML);
                else if (XML->Name() == "findcognates")
                    Result = ProcessFindCognates(XML);
                else if (XML->Name() == "synthesize")
                    Result = ProcessSynthesizeQuery(XML);
                else if (XML->Name() == "showpclass")
                    Result = ProcessShowPClass(XML);
                else if (XML->Name() == "showpclasses")
                    Result = ProcessShowPClasses(XML);
                /*
            else if ( XML->Name () == "add" )
            {
                if (  Attrs.Exists ( "synclass", AttrPos ) && Attrs.Exists ( "stem", AttrPos ) && 
                      Attrs.Exists ( "pclass", AttrPos ) && Attrs.Exists ( "stressschema", AttrPos ) )
                {
                    Stock->AddLexem ( Attrs.Get("synclass"), Attrs.Get("stem"), Attrs.Get("pclass"), Attrs.Get("stressschema") );
                }

            }
    */
                else if (XML->Name() == "tellinfo") {
                    Result = "<info version=\"";
                    Result += Version;
                    Result += "\" build=\"";
                    Result += Build;
                    Result += "\" />";
                }
            }
        } finally(
            if (XML) delete XML;) if (Result.IsEmpty()) Result = "<unrecognized />";
        return "<answer>" + Result + "</answer>";
    }

    /*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ProcessShowPClass                                         | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

    String
    CRMUnit::ProcessShowPClass(XMLElement* XML) const {
        String Result;
        String ThisFlex;
        int i, j;

        if (!XML->Attr("name").IsEmpty()) {
            CRMParadigm* PClass = Stock->FindPClass(XML->Attr("name"));
            if (PClass) {
                Result = "<pclass name=\"" + PClass->UID + "\">\n\r";
                for (i = 0; i < PClass->Items.Count(); ++i) {
                    Result += " <f>\n\r";
                    if (PClass->Items[i]->ItemType() == ERMPFlex) {
                        // Single flex in current position
                        Result += "  <flex ID=\"" + ((CRMParadigm::Flex*)PClass->Items[i])->ID + "\">\n\r";
                    } else if (PClass->Items[i]->ItemType() == ERMPFlexes) {
                        for (j = 0; j < ((CRMParadigm::Flexes*)(PClass->Items[i]))->Items.Count(); ++j) {
                            Result += "  <flex ID=\"" + ((CRMParadigm::Flexes*)(PClass->Items[i]))->Items[j] + "\"";
                            if (((CRMParadigm::Flexes*)(PClass->Items[i]))->Nested[j])
                                Result += " nestedID=\"" + ((CRMParadigm::Flexes*)(PClass->Items[i]))->Nested[j]->UID + "\">\n\r";
                            else
                                Result += ">\n\r";
                        }
                    }
                    Result += " </f>\n\r";
                }
                Result += "</pclass>\n\r";
            }
        }
        return Result;
    }

    /*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ProcessShowPClasses                                       | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

    String
    CRMUnit::ProcessShowPClasses(XMLElement* XML) const {
        String Result;
        String Flex;
        String SClass;

        Flex = XML->Attr("flex");
        SClass = XML->Attr("sclass");
        if (!Flex.IsEmpty() && !SClass.IsEmpty()) {
            // Obey constraints: show all P-Classes, which belong specified S-Class and contain specified Flex
            Result = Stock->FindPClassesForXML(Flex, SClass);
            if (Result.IsEmpty())
                Result = "<notfound />";
            else
                Result = "<pclasses flex=\"" + Flex + "\" sclass=\"" + SClass + "\">\n\r" + Result + "\n\r</pclasses>\n\r";
        }
        return Result;
    }

    /*____________________________________________________________________
                                                                 _  _
                                                                \ \/ /
    CRMUnit destructor                                           )  (
                                                                /_/\_\
  ____________________________________________________________________
*/

    CRMUnit::~CRMUnit() {
        delete Stock;
    }
