/*____________________________________________________________________

  Project:

      Russian  Morphology   Unit
      http://axofiber.org.ru/rm/
      __________________________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru


  Component:  StockManager
  Comments:   Implements storage manipulation routines
  ____________________________________________________________________

  Log:
    06.I.2014													 01.25
		AddLexem method returns added lexeme (to check)
		New seed = 2972
	2012-2013 - ......

    24.IV.2003                                                   01.24
        Added "CRMFlexes" class, "BindFlexLinks" method.
    12.XI.2002                                                   01.23
        Updated load/save methods to satisfy new stream model.
    09.VI.2002                                                   01.22
        Fixed "Bind", "FindLexems" methods.
    05.VI.2002                                                   00.21
        Added "LoadSchemasFromXMLFile" and "SaveSchemasToXMLFile"
        methods.
    13.V.2002                                                    00.20
        Added "AddLexem" method.
    26.IV.2002                                                   00.19
        "Lexems" and "PClasses" fields are of new "CSCDictionary" 
        class, therefor loading is now twice quicker.
    12.IV.2002                                                   00.18
        Added "FindPClass", "FindPClasses" methods.
    28.III.2002                                                  00.17
        Coded up the whole "Load"/"Save" methods series. Now RMUnit 
        supports two file formats: binary and XML-based.
    24.III.2002                                                  00.16
        Modifyed "LoadFromFile" method. Added new "Load"/"Save" series
        methods.
    21.II.2002                                                   00.15
        Added first version of the "SaveToBinary" method.
    14.II.2002                                                   00.14
        Upgraded to the "String" STL class. Fixed "LoadFromFile"
        method's legacy errors (bad some element names).
    05.II.2002                                                   00.13
        All I/O methods were swithed to "CFileStream" class.
        Fixed the "Cl_0" paradigm behaviour, invariable lexems 
        with no p-class link are now acceptable.
        Updated "ProcessQuery" method according to "CAttrsArray" class
        new semanthics.
    06.I.2002                                                    00.12
        CRMLexems now are the ordered list. Thus, added method 
        optimized "FindLexem" method and "Bind" method was also fixed.
        Fixed "SaveToFile" method to enumerate unique lexem's 
        identifiers.
    25.XI.2001                                                   00.11
        Fixed "FindStem" method issue: incorrect insert position
        determination ("Index").
    23.X.2001                                                    00.10
        Removed method "GetPAttr" for translating packed attributes to
        String.
    10.IX.2001                                                   00.09
        Updated "LexemToXML" and "LoadFromFile" functions with new 11 
        classes from CRMNumberThree upto CRMAdverb.
    22.VII.2001                                                  00.08
        Add method "GetPAttr" for translating packed attributes to
        String.
    21.VII.2001                                                  00.07
        Add method "LexemToXML" for flexibility and code saving.
    08.VII.2001                                                  00.06
        Updated structure model (Load and Save routines)
    05.VII.2001                                                  00.05
        Added class "CRMLexems"
    27.VI.2001                                                   00.04
        Class "CRForm" renamed to "CRMLexem"
    23.VI.2001                                                   00.03
        Fixed "FindStem" function.
    21.VI.2001                                                   00.02
        Changed data structure module locatation.
    11.VI.2001                                                   00.00
  ____________________________________________________________________
*/

#include "Lib/stdafx.h"
#include "Lib/Xception.h"
#include "Lib/SCArray.h"
#include "Lib/Xml.h"
#include "Lib/FileStream.h"
#include "RMStockManager.h"

const static String ModuleFileName ( __FILE__ );

const int MinThreshold = 2;


/*____________________________________________________________________

                                                                    /
    CRMFlex class definition                                    \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMFlex
{
public:
    String ID;
    CSCPtrArray<CRMParadigm> PClasses;
};

/*____________________________________________________________________

                                                                    /
    CRMStockManager class definition                            \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMStockManager: public IRMStockManager
{
public:
            // Implementation
            CRMLexem*       AddLexem      ( const String& SynClass, const String& StemID, const String& PClassID, const String& StressSchemaID );
            int             GetBusyStatus () const { return BusyStatus; }
            CRMLexems *     FindCognates  ( const String& Stem ) const;
            CRMLexems *     FindLexems    ( const String& Stem, const String& Flex ) const;
		    CRMLexems *     FindComparativeLexems    ( const String& Stem, const String& Flex ) const;
            CRMParadigm *   FindPClass    ( const String& PClassName );
            CRMPClasses *   FindPClasses  ( const String& FlexID ) const;
            CRMPClasses *   FindPClasses  ( const String& FlexID, const String& SClass ) const;
            String          FindPClassesForXML ( const String& FlexID, const String& SClass ) const;
            int             FormsCount    () const { return FormsQnt; }
            int             LexemsCount   () const { return Lexems.Count (); }
            void            LoadFromFile  ( const String& BaseName, IRMStockManager::FileFormat Format );
            int             PClassesCount () const { return PClasses.Count (); }
            void            SaveToFile    ( const String& BaseName, IRMStockManager::FileFormat Format );
            int             StressSchemasCount () const { return StressSchemas.Count (); };
            void            UpdateCounters  ( bool Clear = false );
                            CRMStockManager ();
#ifdef RMU_UTIL
			bool			GetStem(int index, String& Form) const;
			bool			CheckStressSchemas() const;
			bool			CheckLemmas() const;
#endif
    virtual                ~CRMStockManager ();
private:
    // Attributes
    CSCDictionary<CRMStem>        Stems;
    CSCDictionary<CRMLexem>       Lexems;
    CSCDictionary<CRMParadigm>    PClasses;
    CSCDictionary<CRMStressSchema>StressSchemas;
    CSCDictionary<CRMFlex>        FlexLinks;
    int                           FormsQnt;
    int                           BusyStatus;
    // GIG 30.10.2012 - static member for new UID generation
    static          int           NewUIDSeed;
    // Operation
    void            Bind ();
    void            BindFlexLinks ( const String& FlexID, CRMParadigm * PClass );
    void            LoadLexemsFromBinaryFile  ( const String& BaseName );
    void            LoadLexemsFromXMLFile     ( const String& BaseName );
    void            LoadPClassesFromBinaryFile( const String& BaseName );
    void            LoadPClassesFromXMLFile   ( const String& BaseName );
    void            LoadStemsFromBinaryFile   ( const String& BaseName );
    void            LoadStemsFromXMLFile      ( const String& BaseName );
    void            LoadSchemasFromBinaryFile ( const String& BaseName );
    void            LoadSchemasFromXMLFile    ( const String& BaseName );
    bool            ParadigmContains          ( CRMLexem* Lexem, const String& Flex );
    void            SaveLexemsToBinaryFile    ( const String& BaseName );
    void            SaveLexemsToXMLFile       ( const String& BaseName, int DicType = CRMLexem::GENERAL_DIC );
    void            SavePClassesToBinaryFile  ( const String& BaseName );
    void            SavePClassesToXMLFile     ( const String& BaseName, int DicType = CRMParadigm::GENERAL_DIC );
    void            SaveStemsToBinaryFile     ( const String& BaseName );
    void            SaveStemsToXMLFile        ( const String& BaseName, int DicType = CRMStem::GENERAL_DIC );
    void            SaveSchemasToBinaryFile   ( const String& BaseName );
    void            SaveSchemasToXMLFile      ( const String& BaseName, int DicType = CRMStressSchema::GENERAL_DIC );
};

/*____________________________________________________________________
                                                                  _
                                                                _| |_
    CRMStockManager make function                              |_   _|
                                                                 |_|
  ____________________________________________________________________
*/

IRMStockManager *
IRMStockManager::Create ()
{
    return new CRMStockManager;
}

/*____________________________________________________________________
                                                                  _
                                                                _| |_
    CRMStockManager constructor                                |_   _|
                                                                 |_|
  ____________________________________________________________________
*/


CRMStockManager::CRMStockManager() 
{
    BusyStatus = 0;
    FormsQnt = 0;
    FlexLinks.ForceNotToOwn ();
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    AddLexem                                                    )   (
                                                                \___/
  ____________________________________________________________________
*/

int CRMStockManager::NewUIDSeed = 2972;

CRMLexem*
CRMStockManager::AddLexem ( const String& SynClass, const String& StemID, const String& PClassID, const String& StressSchemaID)
{
    int       Position;
    CRMStem*  Stem;
    CRMLexem* Lexem;

	FILE * pOutput = fopen("addedwords.txt", "a");
    try
    {
        // It seems to be hardly improved in future since this code below ain't complete and correct
        // ...
		// GIG - yes - tried to do something in this area - 30.10.2012
		if ( (Lexem = CRMLexem::CreateByName ( SynClass ))  || ( Lexem = CRMLexem::CreateByCode( SynClass )))
        {
            if ( Stems.FindKey ( StemID, Position ) )
			{
                Stem = Stems[Position];
			}
			else
            {
                Stem = new CRMStem;
                Stem->UID = StemID;
                Stems.Add ( StemID, Stem, Position );
            }
			// GIG : 15.11.2012 - throw exception in case of unknown PClass or Schema
			if (!PClassID.IsEmpty())
			{
				Lexem->Paradigm = PClasses[PClassID];
				if (!Lexem->Paradigm)
					throw new Xception("Unknown P-class " + PClassID + " for stem " + StemID);
			}
			else
			{
				Lexem->Paradigm =  NULL;
			}
            Lexem->StressSchema = StressSchemas[StressSchemaID];
			if (!Lexem->StressSchema)
				throw new Xception("Unknown schema " + StressSchemaID + " for stem " + StemID);
			if (Lexem->Paradigm)
				Lexem->UID = StemID + Lexem->Paradigm->GetFirstFlex ();
			else
				Lexem->UID = StemID;

			int dummy = 0;
			if (Lexems.FindKey(Lexem->UID, dummy))
				Lexem->UID += "_" + String::FromInt(++NewUIDSeed);

            Lexems.Add ( Lexem->UID, Lexem );

            Stem->Lexems.Add ( Lexem );
			fprintf(pOutput, "%s UID=\"%s\"\n", StemID.c_str(), Lexem->UID.c_str());
			fclose(pOutput);
        }
		else
		{
			fprintf(pOutput, "Error 1 in %s\n", StemID.c_str());
			fclose(pOutput);
		}
    }
    catch ( Xception * Error )
    {
        delete Lexem;
        Error->Notify ();
		fprintf(pOutput, "Error: %s\n", Error->GetNotification().c_str());
		fclose(pOutput);
        delete Error;
		return NULL;
    }
	return Lexem;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    Bind                                                      | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

void
CRMStockManager::Bind ()
{
    int           Index, SubIndex, i, j;
    int           FinishedJob;
    double        JobLength;
    CRMLexem    * Lexem;
    CRMParadigm * Paradigm;
    CRMParadigm::Flexes * Flexes;
    CRMParadigm::FlexPositions* FPositions;
    CRMStressSchema*      Schema;
    String                str;

    // Calculate job length
    FinishedJob = 0;
    JobLength = ( double ) 80.0 / ( Stems.Count () + 2 * Lexems.Count () + 2 * PClasses.Count () + StressSchemas.Count () );

    // Bind Stems with Lexems
    for ( Index = 0; Index < Stems.Count (); ++Index )
    {
        SubIndex = 0;
        while ( SubIndex < Stems[Index]->LexemIDs.Count() )
        {
            if ( (Lexem = Lexems[ Stems[Index]->LexemIDs[SubIndex] ])!=0 )
            {
                ++SubIndex;
                Stems[Index]->Lexems.Add ( Lexem );
            }
            else
                Stems[Index]->LexemIDs.Remove ( SubIndex );
        }
        // Free unused memory of IDs lists, since all links are done
        Stems[Index]->LexemIDs.RemoveAll ();
        BusyStatus = 80 - int ( Index * JobLength );
    }
    FinishedJob += Index - 1;
    // Bind Lexems with Paradigms (and free Paradigm identifiers' Strings) and StressSchemas 
    Index = 0;
    while ( Index < Lexems.Count () )
    {
        if ( !Lexems[Index]->ParadigmID.IsEmpty () )
        {
            if ( (Paradigm = PClasses [ Lexems[Index]->ParadigmID ]) )
            {
                Lexems[Index]->Paradigm = Paradigm;
                Lexems[Index]->ParadigmID.Clear ();
                if ( !Lexems[Index]->StressSchemaID.IsEmpty () && ( Schema = StressSchemas [ Lexems[Index]->StressSchemaID ] ) )
                {
                    // Bind with stress schema, if it exists
                    Lexems[Index]->StressSchema = Schema;
                    Lexems[Index]->StressSchemaID.Clear ();
                }
                ++Index;
            }
            else
            {
                try
                {   throw Xception ( "Can't bind lexem\"" + Lexems[Index]->UID + "\" with P-class \"" + Lexems[Index]->ParadigmID + "\"." ); }
                catch ( Xception & Error )
                {   Error.Notify (); }
                // Clean links
                for ( i = 0; i < Stems.Count (); ++i )
                    for ( j = 0; j < Stems[i]->Lexems.Count (); ++j )
                        if ( Stems[i]->Lexems[j]->UID == Lexems[Index]->UID )
                             Stems[i]->Lexems.Remove ( j );
                Lexems.Remove ( Index );
			      }
        }
        else
        {
            // Const (no paradigm)
            if ( !Lexems[Index]->StressSchemaID.IsEmpty () && ( Schema = StressSchemas [ Lexems[Index]->StressSchemaID ] ) )
            {
                // Bind with stress schema, if it exists
                Lexems[Index]->StressSchema = Schema;
                Lexems[Index]->StressSchemaID.Clear ();
            }
            ++Index;            
        }
        BusyStatus = 80 - int ( ( FinishedJob + Index ) * JobLength );
    }
    FinishedJob += Index - 1;
    // Bind nested Paradigms (and free nested Paradigm identifiers' Strings)
    for ( Index = 0; Index < PClasses.Count (); ++Index )
    {
        for ( SubIndex = 0; SubIndex < PClasses[Index]->Items.Count (); ++SubIndex )
        {
            if ( PClasses[Index]->Items[SubIndex]->ItemType () == ERMPVariation )
            {
                Flexes = new CRMParadigm::Flexes;
                for ( i = 0; i < ( (CRMParadigm::Variation*) PClasses[Index]->Items[SubIndex])->NestedIDs.Count (); ++i )
                {
                    Flexes->Items.Add ( ( (CRMParadigm::Variation*) PClasses[Index]->Items[SubIndex])->Flexes[i] );
                    if ( ! ( (CRMParadigm::Variation*) PClasses[Index]->Items[SubIndex])->NestedIDs[i].IsEmpty () )
                    {
                        if ( (Paradigm = PClasses[ ( (CRMParadigm::Variation*) PClasses[Index]->Items[SubIndex])->NestedIDs[i] ]) )
                            Flexes->Nested.Add ( Paradigm );
                        else
                        {
                            try
                            {   throw Xception ( "\nCan't bind P-Class <" + PClasses[Index]->UID + "> with <" + ( (CRMParadigm::Variation*) PClasses[Index]->Items[SubIndex])->NestedIDs[i] + ">" ); }
                            catch ( Xception & Error )
                            {   Error.Notify (); }
                            Flexes->Nested.Add ( NULL );
                        }
                    }
                    else
                        Flexes->Nested.Add ( NULL );
                }
                PClasses[Index]->Items.Replace ( Flexes, SubIndex );
            }
        }
        BusyStatus = 80 - int ( ( FinishedJob + Index ) * JobLength );
    }
    FinishedJob += Index - 1;
    // Bind reverse flex links to paradigms
    for ( Index = 0; Index < PClasses.Count (); ++Index )
    {
        for ( SubIndex = 0; SubIndex < PClasses[Index]->Items.Count (); ++SubIndex )
        {
            switch ( PClasses[Index]->Items[SubIndex]->ItemType () )
            {
                case ERMPFlex:
                    BindFlexLinks ( ( (CRMParadigm::Flex*) PClasses[Index]->Items[SubIndex] )->ID, PClasses[Index] );
                    break;
                case ERMPFlexes:
                    for ( i = 0; i < ( (CRMParadigm::Flexes*) PClasses[Index]->Items[SubIndex] )->Items.Count (); ++i )
                        BindFlexLinks ( ( (CRMParadigm::Flexes*) PClasses[Index]->Items[SubIndex] )->Items[i], PClasses[Index] );
                    break;
            }
        }
        BusyStatus = 80 - int ( ( FinishedJob + Index ) * JobLength );
    }
    FinishedJob += Index - 1;
    // Build suffixes
    for ( Index = 0; Index < Stems.Count (); ++Index )
    {
        if ( 2 < Stems[Index]->UID.Length () )
        {
            str = Stems[Index]->UID.SubStr ( Stems[Index]->UID.Length () - 2, 2 );
            for ( SubIndex = 0; SubIndex < Stems[Index]->Lexems.Count (); ++SubIndex )
                if ( (Paradigm = Stems[Index]->Lexems[SubIndex]->Paradigm) )
                    Paradigm->Suffixes.Add ( str );
        }
        if ( 3 < Stems[Index]->UID.Length () )
        {
            str = Stems[Index]->UID.SubStr ( Stems[Index]->UID.Length () - 2, 3 );
            for ( SubIndex = 0; SubIndex < Stems[Index]->Lexems.Count (); ++SubIndex )
                if ( (Paradigm = Stems[Index]->Lexems[SubIndex]->Paradigm) )
                    Paradigm->SuffixesExt.Add ( str );
        }
    }
    // Bind nested stress schemas (and free nested Stress identifiers' Strings)
    for ( Index = 0; Index < StressSchemas.Count (); ++Index )
    {
        for ( SubIndex = 0; SubIndex < StressSchemas[Index]->Items.Count (); ++SubIndex )
        {
            for ( i = 0; i < StressSchemas[Index]->Items[SubIndex]->Vars.Count (); ++i )
            {
                for ( j = 0; j < StressSchemas[Index]->Items[SubIndex]->Vars[i]->Stresses.Count (); ++j )
                {
                    if ( !StressSchemas[Index]->Items[SubIndex]->Vars[i]->Stresses[j].NestedID.IsEmpty () )
                    {
                        if ( (Schema = StressSchemas [ StressSchemas[Index]->Items[SubIndex]->Vars[i]->Stresses[j].NestedID ]) )
                        {
                            // Nested stress schema
                            StressSchemas[Index]->Items[SubIndex]->Vars[i]->Stresses[j].Nested = Schema;
                            StressSchemas[Index]->Items[SubIndex]->Vars[i]->Stresses[j].NestedID.Clear ();
                        }
                        else
                        {
                            try
                            {   throw Xception ( "\nCan't bind S-Schema <" + StressSchemas[Index]->UID + "> with <" + StressSchemas[Index]->Items[SubIndex]->Vars[i]->Stresses[j].NestedID + ">" ); }
                            catch ( Xception & Error )
                            {   Error.Notify (); }
                            StressSchemas[Index]->Items[SubIndex]->Vars[i]->Stresses[j].Nested = NULL;
                        }
                    }
                }
            }
        }
        BusyStatus = 80 - int ( ( FinishedJob + Index ) * JobLength );
    }
    FinishedJob += Index - 1;
    // Count forms
    FormsQnt = 0;
    for ( Index = 0; Index < Lexems.Count (); ++Index )
    {
        if ( Lexems[Index]->Paradigm )
        {
            FPositions = Lexems[Index]->Paradigm->GetAllFlexPositions ();
            if ( FPositions )
            {
                FormsQnt += FPositions->Items.Count ();
//AfxMessageBox ( "PClass" + Lexems[Index]->Paradigm->UID + " forms:" + String::FromInt ( FPositions->Items.Count () ) );
                delete FPositions;
            }
        }
        else
        {
            // Const (no paradigm)
            ++FormsQnt;
        }
        BusyStatus = 80 - int ( ( FinishedJob + Index ) * JobLength );
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    BindFlexLink                                              | (o)  |
                                                              |______|
  ____________________________________________________________________
*/

void
CRMStockManager::BindFlexLinks ( const String& FlexID, CRMParadigm * PClass )
{
    CRMFlex * Flex;
    bool      YetNotLinked;
    int       i;

    if ( FlexID != "!" )
    {
        Flex = FlexLinks [ FlexID ];
        if ( Flex )
        {
            YetNotLinked = true;
            // Add aditional link, if it's needed
            for ( i = 0; i < Flex->PClasses.Count (); ++i )
                if ( PClass == Flex->PClasses[i] )
                    YetNotLinked = false;
            if ( YetNotLinked )
                Flex->PClasses.Add ( PClass );
        }
        else
        {
            // Add new entry
            Flex = new CRMFlex;
            Flex->ID = FlexID;
            Flex->PClasses.Add ( PClass );
            FlexLinks.Add ( FlexID, Flex );
        }
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    FindCognates                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

CRMLexems*
CRMStockManager::FindCognates ( const String& Stem ) const
{
    int         i;
    CRMStem   * stem;
    CRMLexems * Lexems = new CRMLexems;
                Lexems->ForceNotToOwn ();

    try
    {
        if ( (stem = Stems[Stem]) )
            for ( i = 0; i < stem->Lexems.Count (); ++i )
                Lexems->Add ( stem->Lexems[i] );
    }
    catch ( Xception* Error )
    {
        Error->Notify ();
        delete Error;
    }
    return Lexems;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    FindLexems                                                  )   (
                                                                \___/
  ____________________________________________________________________
*/

CRMLexems*
CRMStockManager::FindLexems ( const String& Stem, const String& Flex ) const
{
    int           i, SClass;
    CRMStem*      stem;
    CRMLexems*    Lexems = new CRMLexems;
                  Lexems->ForceNotToOwn ();

    try
    {
        if ( (stem = Stems[Stem]) )
        {
            for ( i = 0; i < stem->Lexems.Count (); ++i )
            {
                if ( stem->Lexems[i]->Paradigm )
                {
                    if ( stem->Lexems[i]->Paradigm->ContainsFlex ( Flex ) )
                        Lexems->Add ( stem->Lexems[i] );
                }
                else if ( Flex.IsEmpty () )
                {
                    SClass = stem->Lexems[i]->GetSynClass ();
                    if ( SClass == ERM_PARTICLE     || SClass == ERM_INTERJECTION || SClass == ERM_PREPOSITION ||
                         SClass == ERM_CONJUNCTION  || SClass == ERM_ADVERB       || SClass == ERM_PREDICATIVE ||
                         SClass == ERM_PARENTHESIS )
                    {
                        // Only const classes are allowed not to have paradigm
                        Lexems->Add ( stem->Lexems[i] );
                    }
                }
            }
        }
    }
    catch ( Xception* Error )
    {
        Error->Notify ();
        delete Error;
    }
    return Lexems;
}

// GIG - 04.03.2013 - redesign the comparative degree adjective processing
//
CRMLexems *       CRMStockManager::FindComparativeLexems    ( const String& Stem, const String& Flex ) const
{
    int           i;
    CRMStem*      stem;
    CRMLexems*    Lexems = new CRMLexems;
                  Lexems->ForceNotToOwn ();

    try
    {
        if ( (stem = Stems[Stem]) )
        {
			for ( i = 0; i < stem->Lexems.Count (); ++i )
            {
				if ( stem->Lexems[i]->Paradigm && stem->Lexems[i]->GetSynClass() == ERM_ADJECTIVE)
                {
                    if ( stem->Lexems[i]->Paradigm->ContainsFlex ( Flex, CRMParadigm::COMPARATIVE_ADJ_FLEX_POSITION ) )
                        Lexems->Add ( stem->Lexems[i] );
                }
            }
        }
    }
    catch ( Xception* Error )
    {
        Error->Notify ();
        delete Error;
    }
    return Lexems;

}


/*____________________________________________________________________
                                                                 ___
                                                                /   \
    FindPClass                                                  )   (
                                                                \___/
  ____________________________________________________________________
*/

CRMParadigm*
CRMStockManager::FindPClass ( const String& PClassName )
{
    return PClasses[PClassName];
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    FindPClasses                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

CRMPClasses *
CRMStockManager::FindPClasses ( const String& FlexID ) const
{
    int           Index;
    String        Result;
    CRMFlex     * Flex;
    CRMPClasses * PClassesSubSet = new CRMPClasses;

    PClassesSubSet->ForceNotToOwn ();
    if ( (Flex = FlexLinks[FlexID]) )
    {
        for ( Index = 0; Index < Flex->PClasses.Count (); ++Index )
            if ( MinThreshold < Flex->PClasses[Index]->Suffixes.Count () || MinThreshold < Flex->PClasses[Index]->SuffixesExt.Count () )
                PClassesSubSet->Add ( Flex->PClasses[Index] );
    }
    return PClassesSubSet;
}

CRMPClasses *
CRMStockManager::FindPClasses ( const String& FlexID, const String& SClass ) const
{
    int           Index;
    bool          Checked;
    String        Result, ID;
    CRMFlex     * Flex;
    CRMPClasses * PClassesSubSet = new CRMPClasses;

    PClassesSubSet->ForceNotToOwn ();
    if ( (Flex = FlexLinks[FlexID]) )
    {
        for ( Index = 0; Index < Flex->PClasses.Count (); ++Index )
        {
            ID = PClasses[Index]->UID;
            if      ( SClass == "personalpronoun" )  Checked = ( ID.Find ( "PersPr" )  == 0 );
            else if ( SClass == "reflexivepronoun" ) Checked = ( ID.Find ( "ReflPr" )  == 0 );
            else if ( SClass == "pronoun" )          Checked = ( ID.Find ( "Pron" )    == 0 );
            else if ( SClass == "pronounadjective" ) Checked = ( ID.Find ( "PrAdj" )   == 0 );
            else if ( SClass == "adjective" )        Checked = ( ID.Find ( "Adj" )     == 0 );
            else if ( SClass == "noun" )             Checked = ( ID.Find ( "Noun" )    == 0 );
            else if ( SClass == "verb" )             Checked = ( ID.Find ( "Verb" )    == 0 );
            if ( Checked ) PClassesSubSet->Add ( Flex->PClasses[Index] );
        }
    }
    return PClassesSubSet;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    FindPClassesForXML                                          )   (
                                                                \___/
  ____________________________________________________________________
*/

String
CRMStockManager::FindPClassesForXML ( const String& FlexID, const String& SClass ) const
{
    String Result;
    CRMPClasses * PClassesSubSet = FindPClasses ( FlexID, SClass );

    for ( int i = 0; PClassesSubSet->Count (); ++i )
    {
        Result += "<name>";
        Result += (*PClassesSubSet)[i]->UID;
    }
    delete PClassesSubSet;
    return Result;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    LoadFromFile                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

void
CRMStockManager::LoadFromFile ( const String& BaseName, IRMStockManager::FileFormat Format )
{
    try 
    {
        // Start job (busy)
        BusyStatus = 99;
        if ( Format == SM_FORMAT_BINARY )
        {
            LoadStemsFromBinaryFile ( BaseName + ".stem.bin.data" );
            BusyStatus = 95;
            LoadLexemsFromBinaryFile ( BaseName + ".lexem.bin.data" );
            BusyStatus = 90;
            LoadPClassesFromBinaryFile ( BaseName + ".pclass.bin.data" );
            BusyStatus = 85;
            LoadSchemasFromBinaryFile ( BaseName + ".schema.bin.data" );
        }
        else if ( Format == SM_FORMAT_XML )
        {
            LoadStemsFromXMLFile ( BaseName + ".stem.data" );
            BusyStatus = 95;
            LoadLexemsFromXMLFile ( BaseName + ".lexem.data" );
            BusyStatus = 90;
            LoadPClassesFromXMLFile ( BaseName + ".pclass.data" );
            BusyStatus = 85;
            LoadSchemasFromXMLFile ( BaseName + ".schema.data" );
        }
        BusyStatus = 80;
        Bind ();
        // Job is done
        BusyStatus = 0;
    }
    catch ( Xception* Error )
    {
        // Translate known exception
        Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while reading from base: \"" + BaseName + "\"", "LoadFromFile", "CRMStockManager", ModuleFileName );
        newError->Message += "\n" + Error->GetNotification ();
        delete Error;
        throw newError;
    }
    catch ( ... )
    {
        throw new Xception ( Xception::X_FILE_IO, "Error while reading from base: \"" + BaseName + "\"", "LoadFromFile", "CRMStockManager", ModuleFileName );
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadLexemsFromBinaryFile                                  |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadLexemsFromBinaryFile ( const String& BaseName )
{
    //int         AttrIndex  ( 0 );
    int         LineNumber ( 0 );
    String      Line;
    String      eofmsg ( "Unexpected end of file" );
    FileStream  File;
    StreamXORFilter   Xor;
    StreamZipFilter   Stream;
    CRMLexem        * Lexem = NULL;
    CRMSclasses::CRMPersonalPronoun   * PersonalPronoun;
    CRMSclasses::CRMReflexivePronoun  * ReflexivePronoun;
    CRMSclasses::CRMPronoun           * Pronoun;

    try
    {
        if ( File.Open ( BaseName, FileStream::modeRead | FileStream::modeBinary ) )
        {
            Xor.SetInput ( &File );
            Stream.SetEOLsymbol ( '\r' );
            Stream.SetInput ( &Xor );
            Lexems.ForceNotToSort ();
            while ( Stream.ReadLine ( Line ) )
            {
                if ( (Lexem = CRMLexem::CreateByCode ( Line )) )
                {
                    ++LineNumber;
                    if ( Stream.ReadLine ( Line ) )
                        Lexem->UID = Line;
                    else
                        throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                    ++LineNumber;
                    if ( Stream.ReadLine ( Line ) )
                        Lexem->ParadigmID = Line;
                    else
                        throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                    ++LineNumber;
                    if ( Stream.ReadLine ( Line ) )
                        Lexem->StressSchemaID = Line;
                    else
                        throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                    ++LineNumber;
                    if ( Stream.ReadLine ( Line ) )
                        Lexem->UsageCounter = Line.ToInt ();
                    else
                        throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                    if ( Lexem->GetSynClass () == ERM_PERSONAL_PRONOUN )
                    {
                        PersonalPronoun = ( CRMSclasses::CRMPersonalPronoun * ) Lexem;
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            PersonalPronoun->Animate = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            PersonalPronoun->Gender = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            PersonalPronoun->Number = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )                
                            PersonalPronoun->Person = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                    }
                    else if ( Lexem->GetSynClass () == ERM_REFLEXIVE_PRONOUN )
                    {
                        ReflexivePronoun = ( CRMSclasses::CRMReflexivePronoun * ) Lexem;
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            ReflexivePronoun->Animate = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            ReflexivePronoun->Gender = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            ReflexivePronoun->Number = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                    }
                    else if ( Lexem->GetSynClass () == ERM_PRONOUN )
                    {
                        Pronoun = ( CRMSclasses::CRMPronoun * ) Lexem;
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            Pronoun->Animate = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            Pronoun->Gender = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
                            Pronoun->Number = Line.ToInt ();
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );  
                    }
                    Lexems.Add ( Lexem->UID, Lexem );
                }
            }
            Lexems.Sort ();
        }
    }
    catch ( Xception * Error )
    {
        // Translate known exception
        Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while reading \"" +  Line.SubStr (0, 20) + "...\" in line #" + String::FromInt ( LineNumber ) + " from base: \"" + BaseName + "\"", "LoadLexemsFromBinaryFile", "CRMStockManager", ModuleFileName );
        newError->Message += "\n" + Error->GetNotification ();
        delete Error;
        throw newError;
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadLexemsFromXMLFile                                     |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadLexemsFromXMLFile ( const String& BaseName )
{
    int               AttrIndex ( 0 );
    String            Line;
    XMLElement      * XML = NULL;
    String            eofmsg ( "Unexpected end of file" );
    // Lexem elements
    FileStream        LexemFile;
    StreamXMLFilter   XMLFilter;
    CRMLexem        * Lexem;
    CRMSclasses::CRMPersonalPronoun   * PersonalPronoun;
    CRMSclasses::CRMReflexivePronoun  * ReflexivePronoun;
    CRMSclasses::CRMPronoun           * Pronoun;

    try
    {
        if ( LexemFile.Open ( BaseName, FileStream::modeRead ) )
        {
            XMLFilter.SetInput ( &LexemFile );
            try
            {
                Lexems.ForceNotToSort ();
                while ( (XML = XMLFilter.ReadElement ()) )
                {
                    if ( XML->IsElem () && 1 < XML->Attrs ()->Count () )
                    {
                        Lexem = NULL;
                        // 3-1
                        if ( XML->Name () == ERMClassNames[ERM_PERSONAL_PRONOUN][1] )
                        {
                            PersonalPronoun = new CRMSclasses::CRMPersonalPronoun;
                            for ( AttrIndex = 0; AttrIndex < XML->Attrs ()->Count (); ++AttrIndex )
                            {
                                if ( XML->AttrName(AttrIndex) == "uid" )
                                    PersonalPronoun->UID = ( XML->Attr(AttrIndex) );
                                // Animation
                                if ( XML->AttrName(AttrIndex)  == "animate" )
                                {
                                    if ( XML->Attr(AttrIndex) == "yes" ) PersonalPronoun->Animate = 0;
                                    else if ( XML->Attr(AttrIndex) == "no" ) PersonalPronoun->Animate = 1;
                                    else PersonalPronoun->Animate = 2;
                                }
                                // Gender
                                if ( XML->AttrName(AttrIndex) == "gender" ) 
                                {
                                    if ( XML->Attr(AttrIndex) == "male" ) PersonalPronoun->Gender = 0;
                                    else if ( XML->Attr(AttrIndex) == "female" ) PersonalPronoun->Gender = 1;
                                    else if ( XML->Attr(AttrIndex) == "neuter" ) PersonalPronoun->Gender = 2;
                                    else PersonalPronoun->Gender = 3;
                                }
                                // Number
                                if ( XML->AttrName(AttrIndex) == "number" )
                                {
                                    if ( XML->Attr(AttrIndex) == "single" ) PersonalPronoun->Number = 0;
                                    else if ( XML->Attr(AttrIndex) == "plural" ) PersonalPronoun->Number = 1;
                                    else PersonalPronoun->Number = 2;
                                }
                                // Person
                                if ( XML->AttrName(AttrIndex) == "person" )
                                {
                                    if ( XML->Attr(AttrIndex) == "1" ) PersonalPronoun->Person = 0;
                                    else if ( XML->Attr(AttrIndex) == "2" ) PersonalPronoun->Person = 1;
                                    else PersonalPronoun->Person = 2;
                                }
                                // Paradigm class
                                if ( XML->AttrName(AttrIndex) == "paradigmid" )
                                    PersonalPronoun->ParadigmID = XML->Attr(AttrIndex);
                                if ( XML->AttrName(AttrIndex) == "schemaid" )
                                    PersonalPronoun->StressSchemaID = XML->Attr(AttrIndex);
                                if ( XML->AttrName(AttrIndex) == "uc" )
                                    PersonalPronoun->UsageCounter = XML->Attr(AttrIndex).ToInt ();
                            }
                            Lexem = PersonalPronoun;
                        }
                        // 3-2
                        else if ( XML->Name () == ERMClassNames[ERM_REFLEXIVE_PRONOUN][1] )
                        {
                            ReflexivePronoun = new CRMSclasses::CRMReflexivePronoun;
                            for ( AttrIndex = 0; AttrIndex < XML->Attrs()->Count (); ++AttrIndex )
                            {
                                if ( XML->AttrName(AttrIndex) == "uid" )
                                    ReflexivePronoun->UID = ( XML->Attr(AttrIndex) );
                                // Animation
                                if ( XML->AttrName(AttrIndex) == "animate" )
                                {
                                    if ( XML->Attr(AttrIndex) == "yes" ) ReflexivePronoun->Animate = 0;
                                    else if ( XML->Attr(AttrIndex) == "no" ) ReflexivePronoun->Animate = 1;
                                    else ReflexivePronoun->Animate = 2;
                                }
                                // Gender
                                if ( XML->AttrName(AttrIndex) == "gender" ) 
                                {
                                    if ( XML->Attr(AttrIndex) == "male" ) ReflexivePronoun->Gender = 0;
                                    else if ( XML->Attr(AttrIndex) == "female" ) ReflexivePronoun->Gender = 1;
                                    else if ( XML->Attr(AttrIndex) == "neuter" ) ReflexivePronoun->Gender = 2;
                                    else ReflexivePronoun->Gender = 3;
                                }
                                // Number
                                if ( XML->AttrName(AttrIndex) == "number" )
                                {
                                    if ( XML->Attr(AttrIndex) == "single" ) ReflexivePronoun->Number = 0;
                                    else if ( XML->Attr(AttrIndex) == "plural" ) ReflexivePronoun->Number = 1;
                                    else ReflexivePronoun->Number = 2;
                                }
                                // Paradigm class
                                if ( XML->AttrName(AttrIndex) == "paradigmid" )
                                    ReflexivePronoun->ParadigmID = XML->Attr(AttrIndex);
                                if ( XML->AttrName(AttrIndex) == "schemaid" )
                                    ReflexivePronoun->StressSchemaID = XML->Attr(AttrIndex);
                                // Usage counter
                                if ( XML->AttrName(AttrIndex) == "uc" )
                                    ReflexivePronoun->UsageCounter = XML->Attr(AttrIndex).ToInt ();
                            }
                            Lexem = ReflexivePronoun;
                        }
                        // 3-3
                        else if ( XML->Name () == ERMClassNames[ERM_PRONOUN][1] )
                        {
                            Pronoun = new CRMSclasses::CRMPronoun;
                            for ( AttrIndex = 0; AttrIndex < XML->Attrs()->Count (); ++AttrIndex )
                            {
                                if ( XML->AttrName(AttrIndex) == "uid" )
                                    Pronoun->UID = ( XML->Attr(AttrIndex) );
                                // Animation
                                if ( XML->AttrName(AttrIndex) == "animate" )
                                {
                                    if ( XML->Attr(AttrIndex) == "yes" ) Pronoun->Animate = 0;
                                    else if ( XML->Attr(AttrIndex) == "no" ) Pronoun->Animate = 1;
                                    else Pronoun->Animate = 2;
                                }
                                // Gender
                                if ( XML->AttrName(AttrIndex) == "gender" ) 
                                {
                                    if ( XML->Attr(AttrIndex) == "male" ) Pronoun->Gender = 0;
                                    else if ( XML->Attr(AttrIndex) == "female" ) Pronoun->Gender = 1;
                                    else if ( XML->Attr(AttrIndex) == "neuter" ) Pronoun->Gender = 2;
                                    else Pronoun->Gender = 3;
                                }
                                // Number
                                if ( XML->AttrName(AttrIndex) == "number" )
                                {
                                    if ( XML->Attr(AttrIndex) == "single" ) Pronoun->Number = 0;
                                    else if ( XML->Attr(AttrIndex) == "plural" ) Pronoun->Number = 1;
                                    else Pronoun->Number = 2;
                                }
                                // Paradigm class
                                if ( XML->AttrName(AttrIndex) == "paradigmid" )
                                    Pronoun->ParadigmID = XML->Attr(AttrIndex);
                                if ( XML->AttrName(AttrIndex) == "schemaid" )
                                    Pronoun->StressSchemaID = XML->Attr(AttrIndex);
                                // Usage counter
                                if ( XML->AttrName(AttrIndex) == "uc" )
                                    Pronoun->UsageCounter = XML->Attr(AttrIndex).ToInt ();
                            }
                            Lexem = Pronoun;
                        }
                        else if ( ! XML->Name ().IsEmpty () )
                        {
                            if ( (Lexem = CRMLexem::CreateByName ( XML->Name () )) )
                            {
                                for ( AttrIndex = 0; AttrIndex < XML->Attrs()->Count (); ++AttrIndex )
                                {
                                    // UID
                                    if ( XML->AttrName(AttrIndex) == "uid" )
                                        Lexem->UID = XML->Attr(AttrIndex);
                                    // Paradigm class
                                    if ( XML->AttrName(AttrIndex) == "paradigmid" )
                                        Lexem->ParadigmID = XML->Attr(AttrIndex);
                                    if ( XML->AttrName(AttrIndex) == "schemaid" )
                                        Lexem->StressSchemaID = XML->Attr(AttrIndex);
                                    // Usage counter
                                    if ( XML->AttrName(AttrIndex) == "uc" )
                                        Lexem->UsageCounter = XML->Attr(AttrIndex).ToInt ();
                                }
                            }
                            else
                                throw new Xception ( Xception::X_FILE_IO, "Unknown S-class name in basefile \"" +  BaseName + "\".", "LoadLexemsFromXMLFile", "CRMStockManager", ModuleFileName );
                        }
                        if ( Lexem ) Lexems.Add ( Lexem->UID, Lexem );
                    }
                    delete XML;
                    XML = NULL;
                }
                Lexems.Sort ();
            }
            finally (
                if ( XML ) delete XML;
            )
        }
    }
    catch ( Xception * Error )
    {
        // Translate known exception
        Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while reading from base: \"" + BaseName + "\"", "LoadLexemsFromXMLFile", "CRMStockManager", ModuleFileName );
        newError->Message += "\n" + Error->GetNotification ();
        delete Error;
        throw newError;
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadPClassFromBinaryFile                                  |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadPClassesFromBinaryFile ( const String& BaseName )
{
    unsigned long AttrIndex, SubIndex, AttrsQty, SubAttrsQty;
    int           LineNumber ( 0 );
    String        Line;
    String        eofmsg ( "Unexpected end of file" );
    FileStream           File;
    StreamXORFilter      Xor;
    StreamZipFilter      Stream;
    // Paradigm elements
    CRMParadigm           * PClass = NULL;
    CRMParadigm::Flex     * PFlex = NULL;
    CRMParadigm::Variation* PVariation = NULL;
    CSCTypeArray<String>    FlexIDs;
    CSCTypeArray<String>    NestedIDs;

    try
    {        
        if ( File.Open ( BaseName, FileStream::modeRead  | FileStream::modeBinary ) )
        {
            Xor.SetInput ( &File );
            Stream.SetEOLsymbol ( '\r' );
            Stream.SetInput ( &Xor );
            PClasses.ForceNotToSort ();
            while ( Stream.ReadLine ( Line ) )
            {
                ++LineNumber;
                PClass = new CRMParadigm;
                PClass->UID = Line;
                ++LineNumber;
                if ( Stream.ReadLine ( Line ) )
                {
                    AttrsQty = Line.ToInt ();
                    for ( AttrIndex = 0; AttrIndex < AttrsQty; AttrIndex++ )
                    {
                        // Read the number of flex groups
                        ++LineNumber;
                        if ( Stream.ReadLine ( Line ) )
                        {
                            if ( Line.IsEmpty () )
                            {
                                // No flex variation
                                ++LineNumber;
                                if ( Stream.ReadLine ( Line ) )
                                {
                                    PFlex = new CRMParadigm::Flex;
                                    PFlex->ID = Line;
                                    PClass->Items.Add ( PFlex );
                                }
                            }
                            else
                            {
                                FlexIDs.RemoveAll ();
                                NestedIDs.RemoveAll ();
                                // Flex variation
                                SubAttrsQty = Line.ToInt ();
                                for ( SubIndex = 0; SubIndex < SubAttrsQty; SubIndex++ )
                                {
                                    ++LineNumber;
                                    if ( Stream.ReadLine ( Line ) )
                                        FlexIDs.Add ( Line );
                                    else 
                                        throw new Xception ( Xception::X_FILE_IO, eofmsg );
                                    ++LineNumber;
                                    if ( Stream.ReadLine ( Line ) )
                                        NestedIDs.Add ( Line );
                                    else 
                                        throw new Xception ( Xception::X_FILE_IO, eofmsg );
                                }
                                PVariation = new CRMParadigm::Variation;
                                PVariation->Flexes = FlexIDs;
                                PVariation->NestedIDs = NestedIDs;
                                PClass->Items.Add ( PVariation );
                            }
                        }
                        else
                            throw new Xception ( Xception::X_FILE_IO, eofmsg );
                    }
                }
                PClasses.Add ( PClass->UID, PClass );
            }
            PClasses.Sort ();
        }
    }
    catch ( Xception* Error )
    {
        // Translate known exception
        Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while reading \"" +  Line.SubStr (0, 20) + "...\" in line #" + String::FromInt ( LineNumber ) + " from base: \"" + BaseName + "\"", "LoadPClassFromBinaryFile", "CRMStockManager", ModuleFileName );
        newError->Message += "\n" + Error->GetNotification ();
        delete Error;
        throw newError;
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadPClassFromXMLFile                                     |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadPClassesFromXMLFile ( const String& BaseName )
{
    int                     i, j, AttrIndex;
    XMLElement            * XML = NULL;
    XMLElement            * XMLflex = NULL;
    FileStream              File;
    StreamXMLFilter         XMLFilter;
    // Paradigm elements
    CRMParadigm           * PClass = NULL;
    CRMParadigm::Flex     * PFlex = NULL;
    CRMParadigm::Variation* PVariation = NULL;
    CSCTypeArray<String>    FlexIDs;
    CSCTypeArray<String>    NestedIDs;

    try
    {
        if ( File.Open ( BaseName, FileStream::modeRead ) )
        {
            XMLFilter.SetInput ( &File );
            try
            {
                PClasses.ForceNotToSort ();
                while ( (XML = XMLFilter.ReadElement ()) )
                {
                    if ( XML->IsElem () && XML->Name () == "paradigm" )
                    {
                        PClass = new CRMParadigm;
                        if ( XML->Attrs()->Exists ( "uid", AttrIndex ) )
                            PClass->UID = XML->Attr(AttrIndex);
                        // Read internal tags
                        for ( i = 0; i < XML->Children ()->Count (); ++i )
                        {
                            if ( XML->Child(i)->Name () == "f" )
                            {
                                // Tag "f"
                                FlexIDs.RemoveAll ();
                                NestedIDs.RemoveAll ();
                                for ( j = 0; j < XML->Child(i)->Children()->Count (); ++j )
                                {
                                    XMLflex = XML->Child(i)->Child(j);
                                    if ( XMLflex->Name () == "flex" )
                                    {
                                        // Tag "flex"
                                        if ( XMLflex->Attrs()->Exists ( "id", AttrIndex ) )
                                            FlexIDs.Add ( XMLflex->Attr(AttrIndex) );
                                        else
                                            FlexIDs.Add ( "" );
                                        if ( XMLflex->Attrs()->Exists ( "nestedid", AttrIndex ) )
                                            NestedIDs.Add ( XMLflex->Attr(AttrIndex) );
                                        else
                                            NestedIDs.Add ( "" );
                                    }
                                }
                                if ( NestedIDs.Count () == 1 && NestedIDs[0] == "" )
                                {
                                    // This flex element is simple (without nesting or variation)
                                    PFlex = new CRMParadigm::Flex;
                                    PFlex->ID = FlexIDs[0];
                                    PClass->Items.Add ( PFlex );
                                }
                                else
                                {
                                    // This flex element is complex (with nesting or variation)
                                    PVariation = new CRMParadigm::Variation;
                                    PVariation->Flexes    = FlexIDs;
                                    PVariation->NestedIDs = NestedIDs;
                                    PClass->Items.Add ( PVariation );
                                }
                            }
                        }
                        PClasses.Add ( PClass->UID, PClass );
                    }
                    delete XML;
                    XML = NULL;
                }
                PClasses.Sort ();
            }
            finally (
                if ( XML ) delete XML;
            )
        }
    }
    catch ( Xception* Error )
    {
        // Translate known exception
        Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while reading from base: \"" + BaseName + "\"", "LoadPClassFromXMLFile", "CRMStockManager", ModuleFileName );
        newError->Message += "\n" + Error->GetNotification ();
        delete Error;
        throw newError;
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadStemsFromBinaryFile                                   |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadStemsFromBinaryFile ( const String& BaseName )
{
    int             AttrsCount, AttrIndex, Position;
    int             LineNumber ( 0 );
    FileStream      File;
    StreamXORFilter Xor;
    StreamZipFilter Stream;
    String          Line;
    CRMStem       * Stem;

    
	if ( File.Open ( BaseName, FileStream::modeRead | FileStream::modeBinary ) )
	{
		// GIG : 09.11.2011 - File.Open does not throw exceptions so try-catch (with translation) is moved one level down
		try
		{
			Xor.SetInput ( &File );
			Stream.SetEOLsymbol ( '\r' );
			Stream.SetInput ( &Xor );
			Stems.ForceNotToSort ();
			while ( Stream.ReadLine ( Line ) )
			{
				// Find existing stem in dictionary
				++LineNumber;
				if ( !Stems.FindKey ( Line, Position ) )
				{
					Stem = new CRMStem;
					Stem->UID = Line;
					Stems.Add ( Line, Stem, Position );
				}
				else
					Stem = Stems[Position];
				++LineNumber;
				if ( Stream.ReadLine ( Line ) && !Line.IsEmpty () )
				{
					AttrsCount = Line.ToInt ();
					for ( AttrIndex = 0; AttrIndex < AttrsCount; ++AttrIndex )
					{
						++LineNumber;
						if ( Stream.ReadLine ( Line ) )
							Stem->LexemIDs.Add ( Line );
						else
							throw new Xception ( Xception::X_FILE_IO, "Unexpected end of file" );
					}
				}
			}
			Stems.Sort ();
		}
		catch ( Xception * Error )
		{
			// Translate known exception
			Xception * newError = new Xception ( Xception::X_FILE_IO, "Error while reading \"" +  Line.SubStr (0, 20) + "...\" in line #" + String::FromInt ( LineNumber ) + " from base: \"" + BaseName + "\"", "LoadStemsFromBinaryFile", "CRMStockManager", ModuleFileName );
			newError->Message += "\n" + Error->GetNotification ();
			delete Error;
			throw newError;
		}
	} 
	else // GIG: 0911.2011 - added exception raised when file is not found
	{
		throw new Xception("Error while opening file" + BaseName);
	}
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadStemsFromXMLFile                                      |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadStemsFromXMLFile ( const String& BaseName )
{
    int               AttrIndex, Position, TagIndex ( 0 );
    String            Value;
    CRMStem         * Stem;
    FileStream        File;
    StreamXMLFilter   XMLFilter;
    XMLElement      * XML = NULL;

    
	if ( File.Open ( BaseName, FileStream::modeRead ) )
	{
		try // GIG : 09.11.2011 - move one level down
		{
			XMLFilter.SetInput ( &File );
			try
			{
				Stems.ForceNotToSort ();
				while ( (XML = XMLFilter.ReadElement ()) )
				{
					if ( XML->IsElem () && XML->Name () == "stem" )
					{
						if ( XML->Attrs()->Exists ( "uid", AttrIndex ) )
						{
							Value = XML->Attr(AttrIndex);
							Position = 0;                  
							if ( !Stems.FindKey ( Value, Position ) )
							{
								Stem = new CRMStem;
								Stem->UID = Value;
								Stems.Add ( Value, Stem, Position );
							}
							else
								Stem = Stems[Position];                 
							for ( AttrIndex = 0; AttrIndex < XML->Attrs()->Count (); ++AttrIndex )
							{
								if ( XML->AttrName(AttrIndex) == "lexemid" )
								{
									// Add lexem ID into the stem entry
									Stem->LexemIDs.Add ( XML->Attr(AttrIndex) );
								}
							}
						}
					}
					++TagIndex;
					delete XML;
					XML = NULL;
				}
				Stems.Sort ();
			}
			finally (
				if ( XML ) delete XML;
			)
		}
		catch ( Xception * Error )
		{
			// Translate known exception
			Xception * newError = new Xception ( Xception::X_FILE_IO, "Error while reading " + String::FromInt ( TagIndex ) + " element from base: \"" + BaseName + "\"", "LoadStemsFromXMLFile", "CRMStockManager", ModuleFileName );
			newError->Message += "\n" + Error->GetNotification ();
			delete Error;
			throw newError;
		}
	}
	else // GIG : 09.11.2011
	{
		throw new Xception("Error while opening file" + BaseName);
	}
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadSchemasFromBinaryFile                                 |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadSchemasFromBinaryFile ( const String& BaseName )
{
    int             FlexesCount, VarsCount, StressCount;
    int             LineNumber ( 0 );
    FileStream      File;
    StreamXORFilter Xor;
    StreamZipFilter Stream;
    String          Line;
    CRMStressSchema             * Schema;
    CRMStressSchema::Stress       Stress;
    CRMStressSchema::StressVar  * StressVar;
	CRMStressSchema::FlexStress * FlexStress;

	if ( File.Open ( BaseName, FileStream::modeRead | FileStream::modeBinary ) )
	{
		try // GIG : 09.11.2011 - move one level down
		{
			Xor.SetInput ( &File );
			Stream.SetEOLsymbol ( '\r' );
			Stream.SetInput ( &Xor );
			StressSchemas.ForceNotToSort ();
			++LineNumber;
			while ( Stream.ReadLine ( Line ) )
			{
				if ( !Line.IsEmpty () )
				{
					Schema = new CRMStressSchema;
					Schema->UID = Line;
					++LineNumber;
					if ( Stream.ReadLine ( Line ) )
					{
						if ( Line.IsEmpty () ) 
							FlexesCount = 0;
						else
							FlexesCount = Line.ToInt ();
						while ( 0 < FlexesCount && Stream.GetState () == IStreamBase::STREAM_OK )
						{
							FlexStress = new CRMStressSchema::FlexStress;
							++LineNumber;
							if ( Stream.ReadLine ( Line ) )
							{
								if ( Line.IsEmpty () ) 
									VarsCount = 0;
								else
									VarsCount = Line.ToInt ();
								while ( 0 < VarsCount && Stream.GetState () == IStreamBase::STREAM_OK )
								{
									StressVar = new CRMStressSchema::StressVar;
									++LineNumber;
									if ( Stream.ReadLine ( Line ) )
									{
										if ( Line.IsEmpty () ) 
											StressCount = 0;
										else
											StressCount = Line.ToInt ();
										while ( 0 < StressCount && Stream.GetState () == IStreamBase::STREAM_OK )
										{
											++LineNumber;
											Stream.ReadLine ( Line );
											if ( Line.IsEmpty () ) Stress.Pos = 0;
											else Stress.Pos = Line.ToInt ();
											++LineNumber;
											Stream.ReadLine ( Line );
											if ( Line.IsEmpty () ) Stress.Aux = 0;
											else Stress.Aux = Line.ToInt ();
											++LineNumber;
											Stream.ReadLine ( Stress.NestedID );
											StressVar->Stresses.Add ( Stress ); 
											--StressCount;
										}
									}
									FlexStress->Vars.Add ( StressVar );
									--VarsCount;
								}
							}
							Schema->Items.Add ( FlexStress );
							--FlexesCount;
						}
					}
					StressSchemas.Add ( Schema->UID, Schema );
				}
			}
			StressSchemas.Sort ();
		}
		catch ( Xception* Error )
		{
			// Translate known exception
			Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while reading \"" +  Line.SubStr (0, 20) + "...\" in line #" + String::FromInt ( LineNumber ) + " from base: \"" + BaseName + "\"", "LoadSchemasFromBinaryFile", "CRMStockManager", "RMStockManager.cpp" );
			newError->Message += "\n" + Error->GetNotification ();
			delete Error;
			throw newError;
		}
	}
	else // GIG : 09.11.2011
	{
		throw new Xception("Error while opening file" + BaseName);
	}
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    LoadSchemasFromXMLFile                                    |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::LoadSchemasFromXMLFile ( const String& BaseName )
{
	int     i, j, k;
	int     AttrIndex, AttrPos, TagIndex ( 0 );
	String              Value;
	FileStream          File;
	XMLElement        * XML = NULL, * nestedXML;
	StreamXMLFilter               XMLFilter;
	CRMStressSchema::Stress       Stress;
	CRMStressSchema::StressVar  * StressVar;
	CRMStressSchema::FlexStress * FlexStress;
	CRMStressSchema             * Schema;

	if ( File.Open ( BaseName, FileStream::modeRead ) )
	{
		try // GIG : 09.11.2011 - move one level down
		{
			XMLFilter.SetInput ( &File );
			try
			{
				StressSchemas.ForceNotToSort ();
				while ( (XML = XMLFilter.ReadElement ()) )
				{
					if ( XML->IsElem () && XML->Name () == "schema" )
					{
						if ( XML->Attrs()->Exists ( "uid", AttrPos ) )
						{
							Schema = new CRMStressSchema;
							Schema->UID = XML->Attr(AttrPos);
							for ( i = 0; i < XML->Children()->Count (); ++i )
							{
								if ( XML->Child(i)->Name () == "f" )
								{
									FlexStress = new CRMStressSchema::FlexStress;
									for ( j = 0; j < XML->Child(i)->Children()->Count (); ++j )
									{
										nestedXML = XML->Child(i)->Child(j);
										if ( nestedXML->Name () == "flex" )
										{
											StressVar = new CRMStressSchema::StressVar;
											for ( k = 0; k < nestedXML->Children()->Count (); ++k )
											{
												Stress.Pos = Stress.Aux = 0; Stress.NestedID.Clear ();
												if ( nestedXML->Child(k)->Name () == "pos" )
												{
													if ( nestedXML->Child(k)->Attrs()->Exists ( "id", AttrIndex ) )
														Stress.Pos = nestedXML->Child(k)->Attr(AttrIndex).ToInt ();
													if ( nestedXML->Child(k)->Attrs()->Exists ( "aux", AttrIndex ) )
														Stress.Aux = nestedXML->Child(k)->Attr(AttrIndex).ToInt ();
													StressVar->Stresses.Add ( Stress );
												}
												else if ( nestedXML->Child(k)->Name () == "nested" )
												{
													if ( nestedXML->Child(k)->Attrs()->Exists ( "id", AttrIndex ) )
														Stress.NestedID = nestedXML->Child(k)->Attr(AttrIndex);
													StressVar->Stresses.Add ( Stress );
												}
											}
											if ( StressVar->Stresses.Count () == 0 )
											{
												try
												{   throw Xception ( "Error while loading stress schemas\nSchema \"" + Schema->UID + "\" has empty flex elements" ); }
												catch ( Xception & Error )
												{   Error.Notify (); }
											}
											FlexStress->Vars.Add ( StressVar );
										}
									}
									Schema->Items.Add ( FlexStress ); 
								}
							}
							StressSchemas.Add ( Schema->UID, Schema );
						}
					}
					++TagIndex;
					delete XML;
					XML = NULL;
				}
				StressSchemas.Sort ();
			}
			finally (
				if ( XML ) delete XML;
			)
		}
		catch ( Xception* Error )
		{
			// Translate known exception
			Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while reading from base: \"" + BaseName + "\"", "LoadSchemasFromXMLFile", "CRMStockManager", ModuleFileName );
			newError->Message += "\n" + Error->GetNotification ();
			delete Error;
			throw newError;
		}
	}
	else // GIG : 09.11.2011
	{
		throw new Xception("Error while opening file" + BaseName);
	}
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    SaveToFile                                                  )   (
                                                                \___/
  ____________________________________________________________________
*/

void
CRMStockManager::SaveToFile ( const String& BaseName, enum IRMStockManager::FileFormat Format )
{
    try
    {
        if ( Format == SM_FORMAT_BINARY )
        {
            remove ( ( "~" + BaseName + ".stem.bin.data" ).c_str() );
            rename ( ( BaseName + ".stem.bin.data" ).c_str(),( "~" + BaseName + ".stem.bin.data" ).c_str() );
            SaveStemsToBinaryFile     ( BaseName + ".stem.bin.data" );
            remove ( ( "~" + BaseName + ".lexem.bin.data" ).c_str() );
            rename ( ( BaseName + ".lexem.bin.data" ).c_str(),( "~" + BaseName + ".lexem.bin.data" ).c_str() );
            SaveLexemsToBinaryFile    ( BaseName + ".lexem.bin.data" );
            remove ( ( "~" + BaseName + ".pclass.bin.data" ).c_str() );
            rename ( ( BaseName + ".pclass.bin.data" ).c_str(),( "~" + BaseName + ".pclass.bin.data" ).c_str() );
            SavePClassesToBinaryFile  ( BaseName + ".pclass.bin.data" );
            remove ( ( "~" + BaseName + ".schema.bin.data" ).c_str() );
            rename ( ( BaseName + ".schema.bin.data" ).c_str(),( "~" + BaseName + ".schema.bin.data" ).c_str() );
            SaveSchemasToBinaryFile   ( BaseName + ".schema.bin.data" );
        }
        else if ( Format == SM_FORMAT_XML )
        {
            remove ( ( "~" + BaseName + ".stem.data" ).c_str() );
            rename ( ( BaseName + ".stem.data" ).c_str(),  ( "~" + BaseName + ".stem.data" ).c_str() );
            SaveStemsToXMLFile        ( BaseName + ".stem.data" );
            remove ( ( "~" + BaseName + ".lexem.data" ).c_str() );
            rename ( ( BaseName + ".lexem.data" ).c_str(), ( "~" + BaseName + ".lexem.data" ).c_str() );
            SaveLexemsToXMLFile       ( BaseName + ".lexem.data" );
            remove ( ( "~" + BaseName + ".pclass.data" ).c_str() );
            rename ( ( BaseName + ".pclass.data" ).c_str(),( "~" + BaseName + ".pclass.data" ).c_str() );
            SavePClassesToXMLFile     ( BaseName + ".pclass.data"  );
            remove ( ( "~" + BaseName + ".schema.data" ).c_str() );
            rename ( ( BaseName + ".schema.data" ).c_str(),( "~" + BaseName + ".schema.data" ).c_str() );
            SaveSchemasToXMLFile   ( BaseName + ".schema.data" );
        }
//        remove ( ( "~" + BaseName + ".user.stem.data" ).c_str() );
//        rename ( ( BaseName + ".user.stem.data" ).c_str(),  ( "~" + BaseName + ".user.stem.data" ).c_str() );
/*
        remove ( ( "~" + BaseName + ".user.lexem.data" ).c_str() );
        rename ( ( BaseName + ".user.lexem.data" ).c_str(), ( "~" + BaseName + ".user.lexem.data" ).c_str() );
        SaveLexemsToXMLFile       ( BaseName + ".user.lexem.data", CRMLexem::CUSTOM_DIC );

        remove ( ( "~" + BaseName + ".user.pclass.data" ).c_str() );
        rename ( ( BaseName + ".user.pclass.data" ).c_str(),( "~" + BaseName + ".user.pclass.data" ).c_str() );
        SavePClassesToXMLFile     ( BaseName + ".user.pclass.data"  );
        remove ( ( "~" + BaseName + ".user.schema.data" ).c_str() );
        rename ( ( BaseName + ".user.schema.data" ).c_str(),( "~" + BaseName + ".user.schema.data" ).c_str() );
        SaveSchemasToXMLFile   ( BaseName + ".user.schema.data" );
*/
    }
    catch ( Xception * )
    {
        // Translate known exception
        throw;
    }
    catch ( ... )
    {
        throw new Xception ( Xception::X_FILE_IO, "Can't save to base: \"" + BaseName + "\"", "SaveToFile", "CRMStockManager", ModuleFileName );
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SaveLexemsToBinaryFile                                    |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::SaveLexemsToBinaryFile ( const String& BaseName )
{
    int         ItemIndex;
    String      Line;
    // Lexem elements
    FileStream  File;
    StreamXORFilter  Xor;
    StreamZipFilter  Stream;

    CRMLexem  * Lexem;
    CRMSclasses::CRMPersonalPronoun * PersonalPronoun;
    CRMSclasses::CRMReflexivePronoun* ReflexivePronoun;
    CRMSclasses::CRMPronoun         * Pronoun;

    if ( Lexems.Count () )
    {
        try
        {
            File.Open ( BaseName, FileStream::modeCreate   | FileStream::modeWrite | 
                                  FileStream::modeTruncate | FileStream::modeBinary );
            Xor.SetOutput ( &File );
            Stream.SetOutput ( &Xor );
            Stream.SetEOLsymbol ( '\r' );
            for ( ItemIndex = 0; ItemIndex < Lexems.Count (); ++ItemIndex )
            {
                Lexem = Lexems[ItemIndex];
                Stream.WriteLine ( ERMClassNames[Lexem->GetSynClass ()][0] );
                Stream.WriteLine ( Lexem->UID );
                if ( Lexem->Paradigm ) Stream.WriteLine ( Lexem->Paradigm->UID );
                else Stream.WriteLine ( "" );
                if ( Lexem->StressSchema ) Stream.WriteLine ( Lexem->StressSchema->UID );
                else Stream.WriteLine ( "" );
                Stream.WriteLine ( String::FromInt ( Lexem->UsageCounter ) );
                if ( Lexem->GetSynClass () == ERM_PERSONAL_PRONOUN )
                {
                    PersonalPronoun = ( CRMSclasses::CRMPersonalPronoun * ) Lexem;
                    Stream.WriteLine ( String::FromInt ( PersonalPronoun->Animate ) );
                    Stream.WriteLine ( String::FromInt ( PersonalPronoun->Gender ) );
                    Stream.WriteLine ( String::FromInt ( PersonalPronoun->Number ) );
                    Stream.WriteLine ( String::FromInt ( PersonalPronoun->Person ) );
                }
                if ( Lexem->GetSynClass () == ERM_REFLEXIVE_PRONOUN )
                {
                    ReflexivePronoun = ( CRMSclasses::CRMReflexivePronoun * ) Lexem;
                    Stream.WriteLine ( String::FromInt ( ReflexivePronoun->Animate ) );
                    Stream.WriteLine ( String::FromInt ( ReflexivePronoun->Gender ) );
                    Stream.WriteLine ( String::FromInt ( ReflexivePronoun->Number ) );
                }
                else if ( Lexem->GetSynClass () == ERM_PRONOUN )
                {
                    Pronoun = ( CRMSclasses::CRMPronoun * ) Lexem;
                    Stream.WriteLine ( String::FromInt ( Pronoun->Animate ) );
                    Stream.WriteLine ( String::FromInt ( Pronoun->Gender ) );
                    Stream.WriteLine ( String::FromInt ( Pronoun->Number ) );
                }
            }
        }
        catch ( Xception* Error )
        {
            // Translate known exception
            Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while storing line \"" +  Line.SubStr (0, 20) + "...\" to basefile: \"" + BaseName + "\"", "SaveLexemsToBinaryFile", "CRMStockManager", ModuleFileName );
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SaveLexemsToXMLFile                                       |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::SaveLexemsToXMLFile ( const String& BaseName, int DicType )
{
    int         ItemIndex;
    FileStream  File;
    String      Line;

    if ( Lexems.Count () )
    {
        try
        {
            File.Open  ( BaseName, FileStream::modeCreate | FileStream::modeWrite | FileStream::modeTruncate );
            for ( ItemIndex = 0; ItemIndex < Lexems.Count (); ItemIndex++ )
                if ( Lexems[ItemIndex]->DicType == DicType )
                {
                    Line = Lexems[ItemIndex]->ToXML () + "\n";
                    // It is important to add here the reaction on the 
                    // null String result of the LexemToXML function
                    File.Write ( Line );
                }
            if ( File.GetFileSize () == 0 )
            {
                File.Close ();
                _unlink ( BaseName.c_str () );
            }
        }
        catch ( Xception * Error )
        {
            // Translate known exception
            Xception* newError;
            if ( Line.IsEmpty () ) 
                newError = new Xception ( Xception::X_FILE_IO, "Error while storing to basefile: \"" + BaseName + "\"", "SaveLexemsToXMLFile", "CRMStockManager", ModuleFileName );
            else
                newError = new Xception ( Xception::X_FILE_IO, "Error while storing line \"" +  Line.SubStr (0, 20) + "...\" to basefile: \"" + BaseName + "\"", "SaveLexemsToXMLFile", "CRMStockManager", ModuleFileName );
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SavePClassesToBinaryFile                                  |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::SavePClassesToBinaryFile ( const String& BaseName )
{
    int             ItemIndex, AttrIndex, SubIndex;
    FileStream      File;
    StreamXORFilter Xor;
    StreamZipFilter Stream;
    String          Number, Line;

    if ( PClasses.Count () )
    {
        try
        {
            File.Open ( BaseName, FileStream::modeCreate   | FileStream::modeWrite | 
                                  FileStream::modeTruncate | FileStream::modeBinary );
            Xor.SetOutput ( &File );
            Stream.SetOutput ( &Xor );
            Stream.SetEOLsymbol ( '\r' );
            for ( ItemIndex = 0; ItemIndex < PClasses.Count (); ++ItemIndex )
            {
                Stream.WriteLine ( PClasses[ItemIndex]->UID );
                Stream.WriteLine ( String::FromInt ( PClasses[ItemIndex]->Items.Count() ) );
                for ( AttrIndex = 0; AttrIndex < PClasses[ItemIndex]->Items.Count(); ++AttrIndex )
                {
                    if ( PClasses[ItemIndex]->Items[AttrIndex]->ItemType () == ERMPFlex )
                    {
                        Stream.WriteLine ( "" );
                        Stream.WriteLine ( ( (CRMParadigm::Flex*) PClasses[ItemIndex]->Items[AttrIndex])->ID );
                    }
                    else
                    {
                        Stream.WriteLine ( String::FromInt ( ( (CRMParadigm::Flexes*) PClasses[ItemIndex]->Items[AttrIndex])->Items.Count () ) );
                        for ( SubIndex = 0; SubIndex < ( (CRMParadigm::Flexes*) PClasses[ItemIndex]->Items[AttrIndex])->Items.Count (); ++SubIndex )
                        {
                            Stream.WriteLine ( ( (CRMParadigm::Flexes*) PClasses[ItemIndex]->Items[AttrIndex])->Items[SubIndex] );
                            if ( ( (CRMParadigm::Flexes*) PClasses[ItemIndex]->Items[AttrIndex])->Nested[SubIndex] )
                                Stream.WriteLine ( ( (CRMParadigm::Flexes*) PClasses[ItemIndex]->Items[AttrIndex])->Nested[SubIndex]->UID );
                            else
                                Stream.WriteLine ( "" );
                        }
                    }
                }
            }
        }
        catch ( Xception* Error )
        {
            // Translate known exception
            Xception* newError;
            if ( Line.IsEmpty () ) 
                newError = new Xception ( Xception::X_FILE_IO, "Error while storing to basefile: \"" + BaseName + "\"", "SavePClassesToBinaryFile", "CRMStockManager", ModuleFileName );
            else
                newError = new Xception ( Xception::X_FILE_IO, "Error while storing line \"" +  Line.SubStr (0, 20) + "...\" to basefile: \"" + BaseName + "\"", "SavePClassesToBinaryFile", "CRMStockManager", ModuleFileName );
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SavePClassesToXMLFile                                     |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
CRMStockManager::SavePClassesToXMLFile ( const String& BaseName, int DicType )
{
    int         ItemIndex, AttrIndex, SubIndex;
    FileStream  PClassFile;
    String      Number, Line;

    if ( PClasses.Count () )
    {
        try
        {
            PClassFile.Open ( BaseName, FileStream::modeCreate | FileStream::modeWrite | FileStream::modeTruncate );
            for ( ItemIndex = 0; ItemIndex < PClasses.Count () ; ItemIndex++ )
                if ( PClasses[ItemIndex]->DicType == DicType )
                {
                    Line = "<paradigm UID=\"" + PClasses[ItemIndex]->UID + "\">\n";
                    for ( SubIndex = 0; SubIndex < PClasses[ItemIndex]->Items.Count (); SubIndex++ )
                    {
                        // Flexes
                        Line += " <f>\n";
                        if ( PClasses[ItemIndex]->Items[SubIndex]->ItemType () == ERMPFlex )
                        {
                            Line += "  <flex ID=\"" + ( (CRMParadigm::Flex*)(PClasses[ItemIndex]->Items[SubIndex]))->ID + "\" />\n";
                        }
                        else if ( PClasses[ItemIndex]->Items[SubIndex]->ItemType () == ERMPFlexes )
                        {
                            for ( AttrIndex = 0; AttrIndex < ( (CRMParadigm::Flexes*)(PClasses[ItemIndex]->Items[SubIndex]))->Items.Count (); AttrIndex++ )
                            {
                                Line += "  <flex ID=\"" + ( (CRMParadigm::Flexes*)(PClasses[ItemIndex]->Items[SubIndex]))->Items[AttrIndex] + "\"";
                                if ( ( (CRMParadigm::Flexes*)(PClasses[ItemIndex]->Items[SubIndex]))->Nested[AttrIndex] )
                                    Line += " nestedID=\"" + ( (CRMParadigm::Flexes*)(PClasses[ItemIndex]->Items[SubIndex]))->Nested[AttrIndex]->UID + "\" />\n";
                                else
                                    Line += " />\n";
                            }
                        }
                        Line += " </f>\n";
                    }
                    Line += "</paradigm>\n";
                    PClassFile.Write ( Line );
                }
        }
        catch ( Xception* Error )
        {
            // Translate known exception
            Xception* newError;
            newError = new Xception ( Xception::X_FILE_IO, "Error while storing line \"" +  Line.SubStr (0, 20) + "...\" to basefile: \"" + BaseName + "\"", "SavePClassesToXMLFile", "CRMStockManager", ModuleFileName );  
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SaveSchemasToBinaryFile                                   |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::SaveSchemasToBinaryFile ( const String& BaseName )
{
    int             Index, SubIndex, VarIndex, StressIndex;
    int             Pos, Aux;
    FileStream      File;
    StreamXORFilter Xor;
    StreamZipFilter Stream;
    String          Line;

    if ( StressSchemas.Count () )
    {
        try
        {
            File.Open ( BaseName, FileStream::modeCreate   | FileStream::modeWrite | 
                                  FileStream::modeTruncate | FileStream::modeBinary );
            Xor.SetOutput ( &File );
            Stream.SetOutput ( &Xor );
            Stream.SetEOLsymbol ( '\r' );
            for ( Index = 0; Index < StressSchemas.Count (); ++Index )
            {
                Stream.WriteLine ( StressSchemas[Index]->UID );
                if ( StressSchemas[Index]->Items.Count () )
                    Stream.WriteLine ( String::FromInt ( StressSchemas[Index]->Items.Count () ) );
                else
                    Stream.WriteLine ( "" );
                for ( SubIndex = 0; SubIndex < StressSchemas[Index]->Items.Count (); ++SubIndex )
                {
                    if ( StressSchemas[Index]->Items[SubIndex]->Vars.Count () )
                        Stream.WriteLine ( String::FromInt ( StressSchemas[Index]->Items[SubIndex]->Vars.Count ()) );
                    else
                        Stream.WriteLine ( "" );
                    for ( VarIndex = 0; VarIndex < StressSchemas[Index]->Items[SubIndex]->Vars.Count (); ++VarIndex )
                    {
                        if ( StressSchemas[Index]->Items[SubIndex]->Vars[VarIndex]->Stresses.Count () )
                            Stream.WriteLine ( String::FromInt ( StressSchemas[Index]->Items[SubIndex]->Vars[VarIndex]->Stresses.Count () ) );
                        else
                            Stream.WriteLine ( "" );
                        for ( StressIndex = 0; StressIndex < StressSchemas[Index]->Items[SubIndex]->Vars[VarIndex]->Stresses.Count (); ++StressIndex )
                        {
                            Pos = StressSchemas[Index]->Items[SubIndex]->Vars[VarIndex]->Stresses[StressIndex].Pos;
                            Aux = StressSchemas[Index]->Items[SubIndex]->Vars[VarIndex]->Stresses[StressIndex].Aux;
                            if ( Pos ) 
                                Stream.WriteLine ( String::FromInt ( Pos ) );
                            else
                                Stream.WriteLine ( "" );
                            if ( Aux ) 
                                Stream.WriteLine ( String::FromInt ( Aux ) );
                            else
                                Stream.WriteLine ( "" );
                            if ( StressSchemas[Index]->Items[SubIndex]->Vars[VarIndex]->Stresses[StressIndex].Nested )
                                Stream.WriteLine ( StressSchemas[Index]->Items[SubIndex]->Vars[VarIndex]->Stresses[StressIndex].Nested->UID );
                            else
                                Stream.WriteLine ( "" );
                        }
                    }
                }
            }
        }
        catch ( Xception* Error )
        {
            // Translate known exception
            Xception* newError = new Xception ( Xception::X_FILE_IO, "Error while writing line \"" +  Line.SubStr (0, 20) + "...\" from base: \"" + BaseName + "\"", "SaveSchemasFromBinaryFile", "CRMStockManager", ModuleFileName );
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SaveSchemasToXMLFile                                      |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::SaveSchemasToXMLFile ( const String& BaseName, int DicType )
{
    int           ItemIndex, AttrIndex, SubIndex, PosIndex;
    FileStream    SchemaFile;
    String        Line, str;
    CRMStressSchema::Stress StressPos;

    if ( StressSchemas.Count () )
    {
        try
        {
            SchemaFile.Open ( BaseName, FileStream::modeCreate | FileStream::modeWrite | FileStream::modeTruncate );

            for ( ItemIndex = 0; ItemIndex < StressSchemas.Count (); ++ItemIndex )
            {
                if ( StressSchemas[ItemIndex]->DicType == DicType )
                {
                    SchemaFile.Write ( "<schema UID=\"" + StressSchemas[ItemIndex]->UID + "\">\n");
                    for ( AttrIndex = 0; AttrIndex < StressSchemas[ItemIndex]->Items.Count (); ++AttrIndex )
                    {
                        SchemaFile.Write ( " <f>\n");
                        for ( SubIndex = 0; SubIndex < StressSchemas[ItemIndex]->Items[AttrIndex]->Vars.Count (); ++SubIndex )
                        {
                            SchemaFile.Write ( "  <flex>\n");
                            str = "   ";
                            for ( PosIndex = 0; PosIndex < StressSchemas[ItemIndex]->Items[AttrIndex]->Vars[SubIndex]->Stresses.Count (); ++PosIndex )
                            {
                                StressPos = StressSchemas[ItemIndex]->Items[AttrIndex]->Vars[SubIndex]->Stresses[PosIndex];
                                if ( StressPos.Nested )
                                    str += "<nested ID=\"" + StressPos.Nested->UID + "\" />";
                                else
                                {
                                    if ( StressPos.Aux )
                                        str += "<pos ID=\"" + String::FromInt ( StressPos.Pos ) + "\" aux=\"" + String::FromInt ( StressPos.Aux ) + "\" />";
                                    else
                                        str += "<pos ID=\"" + String::FromInt ( StressPos.Pos ) + "\" />";
                                }
                            }
                            SchemaFile.Write ( str + "\n  </flex>\n");
                        }
                        SchemaFile.Write ( " </f>\n");
                    }
                    SchemaFile.Write ( "</schema>\n");
                }
            }
        }
        catch ( Xception* Error )
        {
            // Translate known exception
            Xception* newError;
            newError = new Xception ( Xception::X_FILE_IO, "Error while storing line \"" +  Line.SubStr (0, 20) + "...\" to basefile: \"" + BaseName + "\"", "SaveSchemasToXMLFile", "CRMStockManager", ModuleFileName );
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SaveStemsToBinaryFile                                     |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::SaveStemsToBinaryFile ( const String& BaseName )
{
    int             ItemIndex, AttrIndex;
    FileStream      File;
    StreamXORFilter Xor;
    StreamZipFilter Stream;
    String          Line;

    if ( Stems.Count () )
    {
        try
        {
            File.Open ( BaseName, FileStream::modeCreate   | FileStream::modeWrite | 
                                  FileStream::modeTruncate | FileStream::modeBinary );
            Xor.SetOutput ( &File );
            Stream.SetOutput ( &Xor );
            Stream.SetEOLsymbol ( '\r' );
            for ( ItemIndex = 0; ItemIndex < Stems.Count (); ItemIndex++ )
            {
                Stream.WriteLine ( Stems[ItemIndex]->UID );
                Stream.WriteLine ( String::FromInt ( Stems[ItemIndex]->Lexems.Count () ) );
                for ( AttrIndex = 0; AttrIndex < Stems[ItemIndex]->Lexems.Count (); AttrIndex++ )
                    Stream.WriteLine ( Stems[ItemIndex]->Lexems[AttrIndex]->UID );
            }
        }
        catch ( Xception * Error )
        {
            // Translate known exception
            Xception* newError;
            newError = new Xception ( Xception::X_FILE_IO, "Error while storing line \"" +  Line.SubStr (0, 20) + "...\" to basefile: \"" + BaseName + "\"", "SaveStemsToBinaryFile", "CRMStockManager", ModuleFileName );  
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    SaveStemsToXMLFile                                        |      |
                                                              |___O__|
  ____________________________________________________________________
*/

void
CRMStockManager::SaveStemsToXMLFile ( const String& BaseName, int DicType)
{
    int         ItemIndex, AttrIndex;
    FileStream  StemFile;
    String      Line;

    if ( Stems.Count () )
    {
        try
        {
            StemFile.Open ( BaseName, FileStream::modeCreate | FileStream::modeWrite | FileStream::modeTruncate );
            for ( ItemIndex = 0; ItemIndex < Stems.Count () ; ItemIndex++ )
                if ( Stems[ItemIndex]->DicType == DicType )
                {
                    Line = "<stem UID=\"" + Stems[ItemIndex]->UID + "\"";
                    for ( AttrIndex = 0; AttrIndex < Stems[ItemIndex]->Lexems.Count (); ++AttrIndex )
                        Line += " lexemID=\"" + Stems[ItemIndex]->Lexems[AttrIndex]->UID + "\"";
                    Line += " />\n";
                    StemFile.Write ( Line );
                }
        }
        catch ( Xception* Error )
        {
            // Translate known exception
            Xception* newError;
            newError = new Xception ( Xception::X_FILE_IO, "Error while storing line \"" +  Line.SubStr (0, 20) + "...\" to basefile: \"" + BaseName + "\"", "SaveStemsToXMLFile", "CRMStockManager", ModuleFileName );  
            newError->Message += "\n" + Error->GetNotification ();
            delete Error;
            throw newError;
        }
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    UpdateCounters                                              )   (
                                                                \___/
  ____________________________________________________________________
*/

void
CRMStockManager::UpdateCounters ( bool Clear )
{
    unsigned long i;

    if ( Clear ) 
        for ( i = 0; Lexems.Count (); i++ ) Lexems[i]->UsageCounter = 0;
    else
        for ( i = 0; Lexems.Count (); i++ )
            if ( Lexems[i]->UsageCounter > 2 )
                Lexems[i]->UsageCounter /= 2;
}

/*____________________________________________________________________
                                                                 _  _
                                                                \ \/ /
    CRMStockManager destructor                                   )  (
                                                                /_/\_\
  ____________________________________________________________________
*/

CRMStockManager::~CRMStockManager()
{

}

#ifdef RMU_UTIL
bool			  CRMStockManager::GetStem(int index, String& Form) const
{
	if (index < 0 || index >= Stems.Count()) return false;
	Form = Stems.KeyOf(index);
	return true;
}

bool CRMStockManager::CheckStressSchemas() const
{
	bool retval = true;
	FILE * fOut = fopen("badschemata.txt","w");
	if (!fOut)
	{
		fprintf(stderr, "Cannot open badschemata.txt for write\n");
		return false;
	}
	for (int i = 0; i < Lexems.Count(); ++i)
	{
		CRMLexem * lex = Lexems[i];
		if (lex->StressSchema == NULL)
		{
			fprintf(fOut, "Stress schemata is absent for lexeme '%s'\n", lex->UID.c_str());
			retval = false;
		}
	}
	fclose(fOut);
	return retval;
}

static bool CompareLemma(String lemma, String UID)
{
	int index = UID.Find('_');
	if (index >= 0)
		UID = UID.SubStr(0,index);
	lemma.ReplaceAll(String("¸"),String("å"));
	UID.ReplaceAll(String("¸"),String("å"));
	if (lemma != UID)
		return false;
	return true;
}

bool CRMStockManager::CheckLemmas() const
{
	bool retval = true;
	FILE * fOut = fopen("badlemmas.txt","w");
	if (!fOut)
	{
		fprintf(stderr, "Cannot open badlemmas.txt for write\n");
		return false;
	}
	for (int i = 0; i < Stems.Count(); ++i)
	{
		CRMStem * stem = Stems[i];
		for (int j = 0; j < stem->Lexems.Count(); ++j)
		{
			String lemma;
			CRMParadigm * paradigm = stem->Lexems[j]->Paradigm;
			if (paradigm == NULL || paradigm->Items.Count() == 0)
				lemma = stem->UID;
			else
				lemma = stem->UID + paradigm->GetFirstFlex();
			if (!CompareLemma(lemma, stem->Lexems[j]->UID))
			{
				fprintf(fOut, "Bad lemma '%s' for lexem '%s' with stem '%s'\n", lemma.c_str(),stem->Lexems[j]->UID.c_str(), stem->UID.c_str());
				retval = false;
			}
		}
	}
	fclose(fOut);
	return retval;
}

#endif

