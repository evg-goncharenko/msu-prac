/*____________________________________________________________________

  Project:

      Russian  Morphology   Unit
      http://axofiber.org.ru/rm/
      __________________________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru

  Component:  RMU storage elements
  Comments:   
  ____________________________________________________________________

  Log:
    20.V.2003                                                    01.18
        Redesigned "CRMLexem" and all its descendand classes. Modified
        "CRMParadigm" to support new lexem analysis.
        Added "DicType" field into "CRMLexem" class.
    09.VI.2002                                                   01.17
        Implemented full featured stress schemas suppport. Fixed
        "CRMLexem::IdentifyStress" and "::IdentifyFlex" methods.
    4.VI.2002                                                    00.16
        Modified "CRMLexem" classes to improve load/save procedures -
        added "CreateByName" and "CreateByCode" methods.
    2.VI.2002                                                    00.15
        Fixed "CRMParadigm::Contains" and "CRMParadigm::ContainsFlex"
        methods to eliminate "empty flex" bug.
    16.V.2002                                                    00.14
        Added "CRMStressSchema" class. Added complete "noun" group
        classes.
    10.II.2002                                                   00.13
        Redesigned "CRMLexem" class interface. Added virtual "ToXML"
        method instead of inherited "LexemToXML" method.
    06.I.2002                                                    00.12
        Fixed "GetFirstFlex" method abnormal handling flex variations.
        Critical.
    29.XI.2001                                                   00.11
        Added "UsageCounter" property into "CRMLexem" class.
    25.XI.2001                                                   00.10
        Fixed mistyped non closed XML bracket in "IdentifyFlex" 
        methods.
    16.XI.2001                                                   00.09
        Added "GetAllFlex" method in class "CRMParadigm" for 
        synthesis.
    15.XI.2001                                                   00.08
        Added "GetFirstFlex" method in class "CRMParadigm" for 
        normal form construction.
    26.IX.2001                                                   00.07
        Added 13 classes from "CRMVerbReflexivePerfective" upto
        "CRMVerbBeIrreflexiveIntransitive".
    21.IX.2001                                                   00.06
        Added "TranslateToXML" public method into "CRMLexem" class
        Added 12 classes from "CRMAdverb" upto 
        "CRMVerbReflexivePerfective".
    10.IX.2001                                                   00.05
        Added 11 classes from "CRMNumberThree" upto "CRMAdverb".
    03.IX.2001                                                   00.04
        Added "CRMPronounAdjective" class.
    02.IX.2001                                                   00.03
        Fixed "Contains" function behaviour with nested pclasses.
    05.VIII.2001                                                 00.02
        Added aggregated class "FlexPositions" in the "CRMParadigm" 
        class. Changed the returning value type of the "Contains"
        function.
    07.VII.2001                                                  00.01
        Added public method "ContainsFlex" into class "CRMParadigm"
    15.VI.2001                                                   00.00
  ____________________________________________________________________
*/
#include "Lib/stdafx.h"
#include "RMStock.h"

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    CreateBy...                                                 )   (
    methods                                                     \___/
  ____________________________________________________________________
*/

CRMLexem*
CRMLexem::CreateByNumber ( int SynClass )
{
    CRMLexem* Lexem = NULL;
    
    switch ( SynClass )
    {
        case ERM_PARTICLE:                                                  Lexem = new CRMSclasses::CRMParticle;                                             break;  // (1)
        case ERM_INTERJECTION:                                              Lexem = new CRMSclasses::CRMInterjection;                                         break;  // (2)
        case ERM_PERSONAL_PRONOUN:                                          Lexem = new CRMSclasses::CRMPersonalPronoun;                                      break;  // (3-1)
        case ERM_REFLEXIVE_PRONOUN:                                         Lexem = new CRMSclasses::CRMReflexivePronoun;                                     break;  // (3-2)
        case ERM_PRONOUN:                                                   Lexem = new CRMSclasses::CRMPronoun;                                              break;  // (3-3)
        case ERM_NUMBER_ONE:                                                Lexem = new CRMSclasses::CRMNumberOne;                                            break;  // (4-1)
        case ERM_NUMBER_TWO:                                                Lexem = new CRMSclasses::CRMNumberTwo;                                            break;  // (4-2)
        case ERM_NUMBER_THREE:                                              Lexem = new CRMSclasses::CRMNumberThree;                                          break;  // (4-3)
        case ERM_NUMBER:                                                    Lexem = new CRMSclasses::CRMNumber;                                               break;  // (4-4)
        case ERM_NUMBER_BIFORM:                                             Lexem = new CRMSclasses::CRMNumberBiForm;                                         break;  // (4-5)
        case ERM_NOUN_MALE_INANIMATE:                                       Lexem = new CRMSclasses::CRMNounMaleInanimate;                                    break;  // (7-1)
        case ERM_NOUN_MALE_ANIMATE:                                         Lexem = new CRMSclasses::CRMNounMaleAnimate;                                      break;  // (7-2)
        case ERM_NOUN_FEMALE_INANIMATE:                                     Lexem = new CRMSclasses::CRMNounFemaleInanimate;                                  break;  // (7-3)
        case ERM_NOUN_FEMALE_ANIMATE:                                       Lexem = new CRMSclasses::CRMNounFemaleAnimate;                                    break;  // (7-4)
        case ERM_NOUN_NEUTER_INANIMATE:                                     Lexem = new CRMSclasses::CRMNounNeuterInanimate;                                  break;  // (7-5)
        case ERM_NOUN_NEUTER_ANIMATE:                                       Lexem = new CRMSclasses::CRMNounNeuterAnimate;                                    break;  // (7-6)
        case ERM_NOUN_MALEFEMALE_ANIMATE:                                   Lexem = new CRMSclasses::CRMNounMaleFemaleAnimate;                                break;  // (7-7)
        case ERM_NOUN_MALE:                                                 Lexem = new CRMSclasses::CRMNounMale;                                             break;  // (7-8)
        case ERM_NOUN_MALE_OR_FEMALE_INANIMATE:                             Lexem = new CRMSclasses::CRMNounMaleOrFemaleInanimate;                            break;  // (7-9) 
        case ERM_NOUN_MALE_OR_FEMALE_ANIMATE:                               Lexem = new CRMSclasses::CRMNounMaleOrFemaleAnimate;                              break;  // (7-10)
        case ERM_NOUN_FEMALE:                                               Lexem = new CRMSclasses::CRMNounFemale;                                           break;  // (7-11)
        case ERM_NOUN_NEUTER_OR_FEMALE_INANIMATE:                           Lexem = new CRMSclasses::CRMNounNeuterOrFemaleInanimate;                          break;  // (7-12) 
        case ERM_NOUN_NEUTER:                                               Lexem = new CRMSclasses::CRMNounNeuter;                                           break;  // (7-13)
        case ERM_NOUN_NEUTER_OR_MALE_ANIMATE:                               Lexem = new CRMSclasses::CRMNounNeuterOrMaleAnimate;                              break;  // (7-14)
        case ERM_NOUN_CONST_NEUTER_INANIMATE:                               Lexem = new CRMSclasses::CRMNounNeuterConstInanimate;                             break;  // (7-15) 
        case ERM_NOUN_NEUTER_OR_MALE_INANIMATE:                             Lexem = new CRMSclasses::CRMNounNeuterOrMaleInimate;                              break;  // (7-16)
		case ERM_NOUN_PROPER_FEMALE_ANIMATE:                                Lexem = new CRMSclasses::CRMNounProperFemaleAnimate;                              break;  // (7-17)
		case ERM_NOUN_PROPER_MALE_ANIMATE:                                  Lexem = new CRMSclasses::CRMNounProperMaleAnimate;                                break;  // (7-18)
		case ERM_NOUN_PROPER_MALEFEMALE_ANIMATE:                            Lexem = new CRMSclasses::CRMNounProperMaleFemaleAnimate;                          break;  // (7-19)
		case ERM_NOUN_HYPOCOR_FEMALE_ANIMATE:                               Lexem = new CRMSclasses::CRMNounHypocorFemaleAnimate;                             break;  // (7-20)
		case ERM_NOUN_HYPOCOR_MALE_ANIMATE:                                 Lexem = new CRMSclasses::CRMNounHypocorMaleAnimate;                               break;  // (7-21)
		case ERM_NOUN_HYPOCOR_MALEFEMALE_ANIMATE:                           Lexem = new CRMSclasses::CRMNounHypocorMaleFemaleAnimate;                         break;  // (7-22)
		case ERM_NOUN_PROPER_FEMALE_INANIMATE:                              Lexem = new CRMSclasses::CRMNounProperFemaleInanimate;                            break;  // (7-23)
		case ERM_NOUN_PROPER_MALE_INANIMATE:                                Lexem = new CRMSclasses::CRMNounProperMaleInanimate;                              break;  // (7-24)
		case ERM_NOUN_PROPER_SURNAME:		                                Lexem = new CRMSclasses::CRMNounProperSurname;								      break;  // (7-25)
		case ERM_ADJECTIVE:                                                 Lexem = new CRMSclasses::CRMAdjective;                                            break;  // (8-1)
        case ERM_PRONOUN_ADJECTIVE:                                         Lexem = new CRMSclasses::CRMPronounAdjective;                                     break;  // (8-2)
        case ERM_NUMBER_ORDINAL:                                            Lexem = new CRMSclasses::CRMNumberOrdinal;                                        break;  // (8-3)
        case ERM_PARTICIPLE:                                                                                                                                  break;  // (8-4)
        case ERM_POSSESIVE_ADJECTIVE:                                       Lexem = new CRMSclasses::CRMPossesiveAdjective;                                   break;  // (8-5)
        case ERM_VERB_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE:                Lexem = new CRMSclasses::CRMVerbIrreflexiveIntransitiveImperfective;              break;  // (9-1)
        case ERM_VERB_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE:                  Lexem = new CRMSclasses::CRMVerbIrreflexiveIntransitivePerfective;                break;  // (9-2)
        case ERM_VERB_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE:                  Lexem = new CRMSclasses::CRMVerbIrreflexiveTransitiveImperfective;                break;  // (9-3)
        case ERM_VERB_IRREFLEXIVE_TRANSITIVE_PERFECTIVE:                    Lexem = new CRMSclasses::CRMVerbIrreflexiveTransitivePerfective;                  break;  // (9-4)
        case ERM_VERB_REFLEXIVE_IMPERFECTIVE:                               Lexem = new CRMSclasses::CRMVerbReflexiveImperfective;                            break;  // (9-5)
        case ERM_VERB_REFLEXIVE_PERFECTIVE:                                 Lexem = new CRMSclasses::CRMVerbReflexivePerfective;                              break;  // (9-6)
        case ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE:  Lexem = new CRMSclasses::CRMVerbFrequentativeIrreflexiveIntransitiveImperfective; break;  // (9-7)
        case ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE:    Lexem = new CRMSclasses::CRMVerbFrequentativeIrreflexiveTransitiveImperfective;   break;  // (9-8)
        case ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE:     Lexem = new CRMSclasses::CRMVerbImpersonalIrreflexiveIntransitiveImperfective;    break;  // (9-9)
        case ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE:       Lexem = new CRMSclasses::CRMVerbImpersonalIrreflexiveIntransitivePerfective;      break;  // (9-10)
        case ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE:       Lexem = new CRMSclasses::CRMVerbImpersonalIrreflexiveTransitiveImperfective;      break;  // (9-11)
        case ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_PERFECTIVE:         Lexem = new CRMSclasses::CRMVerbImpersonalIrreflexiveTransitivePerfective;        break;  // (9-12)
        case ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_IMPERFECTIVE:       Lexem = new CRMSclasses::CRMVerbImpersonalReflexiveIntransitiveImperfective;      break;  // (9-13)
        case ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_PERFECTIVE:         Lexem = new CRMSclasses::CRMVerbImpersonalReflexiveIntransitivePerfective;        break;  // (9-14)
        case ERM_VERB_IRREFLEXIVE_INTRANSITIVE_BIFORM:                      Lexem = new CRMSclasses::CRMVerbIrreflexiveIntransitiveBiform;                    break;  // (9-15)
        case ERM_VERB_IRREFLEXIVE_TRANSITIVE_BIFORM:                        Lexem = new CRMSclasses::CRMVerbIrreflexiveTransitiveBiform;                      break;  // (9-16)
        case ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM:                        Lexem = new CRMSclasses::CRMVerbReflexiveIntransitiveBiform;                      break;  // (9-17)
        case ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM_EXT:                    Lexem = new CRMSclasses::CRMVerbReflexiveIntransitiveBiformExt;                   break;  // (9-18)
        case ERM_VERB_BE_IRREFLEXIVE_INTRANSITIVE:                          Lexem = new CRMSclasses::CRMVerbBeIrreflexiveIntransitive;                        break;  // (9-19)
        case ERM_PREPOSITION:                                               Lexem = new CRMSclasses::CRMPreposition;                                          break;  // (10)
        case ERM_CONJUNCTION:                                               Lexem = new CRMSclasses::CRMConjunction;                                          break;  // (11)
        case ERM_ADVERB:                                                    Lexem = new CRMSclasses::CRMAdverb;                                               break;  // (12)
        case ERM_PREDICATIVE:                                               Lexem = new CRMSclasses::CRMPredicative;                                          break;  // (13)
        case ERM_PARENTHESIS:                                               Lexem = new CRMSclasses::CRMParenthesis;                                          break;  // (14)
    };
    return Lexem;
}

CRMLexem*
CRMLexem::CreateByCode ( const String& Code )
{
    for ( int i = 0; i < ERM_LAST_CLASS_NUMBER; ++i ) {
        if ( Code == ERMClassNames[i][0] )
            return CreateByNumber ( i );
	}
    return NULL;
}

CRMLexem*
CRMLexem::CreateByName ( const String& Name )
{
    for ( int i = 0; i < ERM_LAST_CLASS_NUMBER; ++i ) {
        if ( Name == ERMClassNames[i][1] )
            return CreateByNumber ( i );
	}
    return NULL;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    ProcessFlex                                                 )   (
                                                                \___/
  ____________________________________________________________________
*/

bool CRMSclasses::CRMAdjective::AdjectiveHomonim::_HackOn = false;


Strings 
CRMLexem::ProcessFlex ( 
    CRMParadigm::FlexVar & FlexPos,
    const String & Properties,
    const Strings & FlexData,
    CRMHomonim * Homonim,
    const CRMParadigm * const Paradigm,
    CRMStressSchema * StressSchema )
{ 
    int     j, k;
    Strings Result, preResult, StressData;
    String  prop;

    if ( FlexData.Count () == 0 || ( Paradigm && Paradigm->Items.Count () == 0 ))
    {
        prop = Properties;
        prop += " static=yes";
        if ( StressSchema )
        {
            StressData = StressSchema->Identify ( FlexPos, 0 );
			// GIG: all variations of stress are written into single result since they are belong to SINGLE morphoresult
			if (StressData.Count()) {
				String stresses = " stress=" + StressData[0];
				//for ( j = 0; j < StressData.Count(); ++j )
				//	Result.Add ( prop + StressData[j] );
				for (j = 1; j < StressData.Count(); ++j)
					stresses += "," + StressData[j];
				Result.Add(prop + stresses);
			}
        }
        else
            Result.Add ( prop );
    }
    else
    {
		// VIA:GIG: common shemata - for all lexemes. Specific processing is in VerbHomonim class
        prop = Properties + FlexData [FlexPos.Pos % 512];
		Result.Add ( prop );
		Homonim->Identify ( Result, FlexPos.Pos, Properties, Paradigm );
        if ( StressSchema )
        {
			StressData = StressSchema->Identify ( FlexPos,  Homonim->StressDelta());
			// GIG: all variations of stress are written into single result since they are belong to SINGLE morphoresult
			if (0 < StressData.Count()) {
				String stresses = " stress=" + StressData[0];
				for (j = 1; j < StressData.Count(); ++j)
					stresses += "," + StressData[j];
				for (k = 0; k < Result.Count(); ++k)
					preResult.Add(Result[k] + stresses);
				//for (j = 0; j < StressData.Count(); ++j)
				//	for (k = 0; k < Result.Count(); ++k)
				//		preResult.Add(Result[k] + StressData[j]);
			}
            Result = preResult;
        }
    }
    return Result;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    ProcessFlexes                                               )   (
                                                                \___/
  ____________________________________________________________________
*/

Strings 
CRMLexem::ProcessFlexes ( 
    CRMParadigm::FlexPositions * Positions,
    const String & Properties,
    const Strings & FlexData,
    CRMHomonim * Homonim,
    const CRMParadigm * const Paradigm,
    CRMStressSchema * StressSchema )
{ 
    int i;

    if ( FlexData.Count () == 0 )
        return ProcessFlex ( Positions->Items[0], Properties, FlexData, Homonim, Paradigm, StressSchema );
    else
    {
        Strings Result;
        for ( i = 0; i < Positions->Items.Count (); ++i )
            Result += ProcessFlex ( Positions->Items[i], Properties, FlexData, Homonim, Paradigm, StressSchema );
        return Result;
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Contains                                                    )   (
                                                                \___/
  ____________________________________________________________________
*/

CRMParadigm::FlexPositions *
CRMParadigm::Contains ( const String & Flex, int StartPos ) const
{
    int             i, j, k;
    String          PFlex;
    FlexVar         FlexItem;
    FlexPositions * NestedResult, * Result = new FlexPositions;

    if ( Items.Count () == 0 )
    {
        FlexItem.Pos = 0;
        FlexItem.Var = 0;
        Result->Items.Add ( FlexItem );
    }
    for ( i = StartPos; i < Items.Count (); ++i )
    {
        if ( Items[i]->ItemType () == ERMPFlex )
        {
            // Single flex in current position
            if ( (( CRMParadigm::Flex *) Items[i] )->ID == Flex )
            {
                FlexItem.Pos = i;
                FlexItem.Var = 0;
                Result->Items.Add ( FlexItem );
            }
        }
        else if ( Items[i]->ItemType () == ERMPFlexes )
        {
            // Flex variation
            for ( j = 0; j < ( (Flexes *) Items[i] )->Items.Count (); ++j )
            {
				// GIG: we should codify nested results even for exact match ("дела-ем")
				Flexes * pFlexes =  (Flexes *) Items[i];
                PFlex = pFlexes->Items[j];
				if (Flex.Find ( PFlex ) == 0 )
				{
                    if ( pFlexes->Nested[j] )
                    {
                        NestedResult = pFlexes->Nested[j]->Contains ( Flex.SubStr ( PFlex.Length (), Flex.Length () - PFlex.Length () ) );
                        if ( NestedResult )
                        {
                            // Add codified flex numbers (basis is 512 for nested classes)
                            for ( k = 0; k < NestedResult->Items.Count (); ++k )
                            {
                                FlexItem.Pos = i + ( ( NestedResult->Items[k].Pos + 1 ) << 9 );
                                FlexItem.Var = j + ( ( NestedResult->Items[k].Var + 1 ) << 9 );
                                Result->Items.Add ( FlexItem );
                            }
                            delete NestedResult;
                        }
                    } else if (Flex.Length() == PFlex.Length()) {// perfect match without nesting
						FlexItem.Pos = i;
						FlexItem.Var = j;
                        Result->Items.Add ( FlexItem );
					} else
						;
				}
            }
        }
    }
    return Result;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    ContainsFlex                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

bool
CRMParadigm::ContainsFlex ( const String& Flex, int StartPos ) const
{
    bool      Result ( false );
    int       i, j;
    String    PFlex;

    if ( Items.Count () == 0 )
    {
        Result = Flex.IsEmpty ();
    }
    else for ( i = StartPos; i < Items.Count (); i++ )
    {
        if ( Items[i]->ItemType () == ERMPFlex )
        {
            // Single flex in current position
            if ( Flex == ( (CRMParadigm::Flex *) Items[i])->ID )
            {
                Result = true;
                break;
            }
        }
        else if ( Items[i]->ItemType () == ERMPFlexes )
        {
			// GIG: 03/03/2012 - we need to check nested flexes in ANY case
			Flexes * pFlexes = (Flexes *) Items[i];
            // Flex variation
            for ( j = 0; j < pFlexes->Items.Count (); j++ )
            {
                PFlex = pFlexes->Items[j];
                if ( Flex.Find ( PFlex ) == 0 )
                {
                    // GIG: we still need to check nested condition even in case of perfect match ("делающ")
                    // Nested case
                    if (pFlexes->Nested[j] )
                        Result = pFlexes->Nested[j]->ContainsFlex ( Flex.SubStr ( PFlex.Length (), Flex.Length () - PFlex.Length () ) );
					else
						Result = Flex.Length() == PFlex.Length(); // PFlex is already prefix!!!
                }
            }
            if ( Result ) break;
         }
    }
    return Result;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    GetAllFlexes                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

Strings*
CRMParadigm::GetAllFlexes () const
{
    int i, j, k;
    Strings *Result = new Strings, *NestedResult;
 
    for ( i = 0; i < Items.Count (); i++ )
    {
        if ( Items[i]->ItemType () == ERMPFlex )
        {
            if ( ((Flex*)Items[i])->ID != "!" )
                Result->Add ( ( (Flex*)Items[i] )->ID );
        }
        else if ( Items[i]->ItemType () == ERMPFlexes )
        {
            for ( j = 0; j < ( (Flexes*) Items[i])->Items.Count (); j++ )
            {
                if ( ( (Flexes*) Items[i])->Items[j] != "!" ) 
                {
                    if ( ((Flexes*) Items[i])->Nested[j] ) 
                    {
                        NestedResult = ( (Flexes*) Items[i])->Nested[j]->GetAllFlexes ();
                        for ( k = 0; k < NestedResult->Count (); k++ )

                            if ( (*NestedResult)[k] != "!" ) Result->Add ( ( (Flexes*)Items[i] )->Items[j] + (*NestedResult)[k] );
                        if ( NestedResult ) delete NestedResult;
                    }
                    else
                        Result->Add ( ( (Flexes*)Items[i] )->Items[j] );
                }
            }
        }
    }
    return Result;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    GetAllFlexPositions                                         )   (
                                                                \___/
  ____________________________________________________________________
*/

CRMParadigm::FlexPositions *
CRMParadigm::GetAllFlexPositions () const
{
    FlexVar       FlexItem;
    FlexPositions *NestedResult, *Result = new FlexPositions;
    int           i, j, k;
 
    if ( Items.Count () != -1 )
    for ( i = 0; i < Items.Count (); ++i )
    {
        if ( Items[i]->ItemType () == ERMPFlex )
        {
            // Single flex in current position
            if ( ((Flex *) Items[i] )->ID != "!" )
            {
                FlexItem.Pos = i;
                FlexItem.Var = 0;
                Result->Items.Add ( FlexItem );
            }
        }
        else if ( Items[i]->ItemType () == ERMPFlexes )
        {
            // Flex variation
            for ( j = 0; j < ( (Flexes *) Items[i] )->Items.Count (); ++j )
            {
                if ( ( (Flexes *) Items[i] )->Items[j] != "!" )
                {
                    if ( ((Flexes *) Items[i] )->Nested[j] )
                    {
                        NestedResult = ( (Flexes *) Items[i] )->Nested[j]->GetAllFlexPositions ();
                        if ( NestedResult )
                        {
                            // Add codified flex numbers (basis is 512 for nested classes)
                            for ( k = 0; k < NestedResult->Items.Count (); ++k )
                            {
                                FlexItem.Pos = i + ( ( NestedResult->Items[k].Pos + 1 ) << 9 );
                                FlexItem.Var = j + ( ( NestedResult->Items[k].Var + 1 ) << 9 );
                                Result->Items.Add ( FlexItem );
                            }
                            delete NestedResult;
                        }
                    }
                    else
                    {
                        FlexItem.Pos = i;
                        FlexItem.Var = j;
                        Result->Items.Add ( FlexItem );
                    }
                }
            }
        }
    }
    return Result;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    GetFirstFlex                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

String
CRMParadigm::GetFirstFlex () const
{
    int     i = 0, j;
    bool    Found = false;
    String  Result;
    
    while ( !Found && i < Items.Count () )
    {
        if ( Items[i]->ItemType () == ERMPFlex )
        {
            if ( ( (Flex*)Items[i] )->ID != "!" )
            {
                Result = ( (Flex*)Items[i] )->ID;
                Found = true;
            }
        }
        else if ( Items[i]->ItemType () == ERMPFlexes )
        {
            j = 0;
            while ( !Found && j < ( (Flexes*) Items[i])->Items.Count () )
            {
                if ( ( (Flexes*) Items[i])->Items[j] != "!" ) 
                {
                    Result = ((Flexes*) Items[i])->Items[j];
                    Found = true;
                    if ( ( (Flexes*) Items[i])->Nested[j] )
                    {
                        if ( ( (Flexes*) Items[i])->Nested[j]->GetFirstFlex () != "!" )
                        {
                            Result += ( (Flexes*) Items[i])->Nested[j]->GetFirstFlex ();
                            Found = true;
                        }
                        else
                            Found = false;
                    }
                }
                j++;
            }
        }
        i++;
    }
    return Result;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    IdentifyFlex                                                )   (
                                                                \___/
  ____________________________________________________________________
*/

Strings
CRMParadigm::IdentifyFlex ( const String& Flex ) const
{
    Strings Result;
    CRMParadigm::FlexPositions * FlexPositions = Contains ( Flex );
    CRMSclasses::CRMPossesiveAdjective::PossesiveAdjectiveHomonim possesiveAdjectiveHomonim;
    CRMSclasses::CRMPronounAdjective::PronounAdjectiveHomonim pronounAdjectiveHomonim;
    CRMSclasses::CRMAdjective::AdjectiveHomonim adjectiveHomonim;
    CRMSclasses::Verb::VerbHomonim verbHomonim;
    CRMSclasses::Noun::NounHomonim nounHomonim(0);

    if ( UID.Find ( "PrAdj" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  "<pronounadjective",
                  CRMSclasses::Number::cPronounAdjective, &pronounAdjectiveHomonim, this, NULL );
    }
    else if ( UID.Find ( "PossAdj" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  "<possesiveadjective",
                  CRMSclasses::CRMPossesiveAdjective::cPossesiveAdjective, &possesiveAdjectiveHomonim, this, NULL );
    }
    else if ( UID.Find ( "Adj" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  "<adjective",
                  CRMSclasses::CRMAdjective::cAdjective, &adjectiveHomonim, this, NULL );
    }
    else if ( UID.Find ( "NounM" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  "<noun gender=\"male\"",
                  CRMSclasses::Noun::cNounMale, &nounHomonim, this, NULL );
    }
    else if ( UID.Find ( "NounG" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  "<noun gender=female",
                  CRMSclasses::Noun::cNounFemale, &nounHomonim, this, NULL );
    }
    else if ( UID.Find ( "NounS" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  "<noun gender=neuter",
                  CRMSclasses::Noun::cNounMale, &nounHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb1" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cIrreflexiveIntransitiveImperfectiveSign,
                  CRMSclasses::Verb::cIrreflexiveIntransitiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb2" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cIrreflexiveIntransitivePerfectiveSign,
                  CRMSclasses::Verb::cIrreflexiveIntransitivePerfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb3" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cIrreflexiveTransitiveImperfectiveSign,
                  CRMSclasses::Verb::cIrreflexiveTransitiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb4" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cIrreflexiveTransitivePerfectiveSign,
                  CRMSclasses::Verb::cIrreflexiveTransitivePerfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb5" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cReflexiveImperfectiveSign,
                  CRMSclasses::Verb::cReflexiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb6" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cReflexivePerfectiveSign,
                  CRMSclasses::Verb::cReflexivePerfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb7" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cFrequentativeIrreflexiveIntransitiveImperfectiveSign,
                  CRMSclasses::Verb::cFrequentativeIrreflexiveIntransitiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb8" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cFrequentativeIrreflexiveTransitiveImperfectiveSign,
                  CRMSclasses::Verb::cFrequentativeIrreflexiveTransitiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb9" ) == 0 )
    {
        Result =  CRMLexem::ProcessFlexes ( FlexPositions, 
                  CRMSclasses::Verb::cImpersonalIrreflexiveIntransitiveImperfectiveSign,
                  CRMSclasses::Verb::cImpersonalIrreflexiveIntransitiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb10" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cImpersonalIrreflexiveIntransitivePerfectiveSign,
                CRMSclasses::Verb::cImpersonalIrreflexiveIntransitivePerfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb11" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cImpersonalIrreflexiveTransitiveImperfectiveSign,
                CRMSclasses::Verb::cImpersonalIrreflexiveTransitiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb12" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cImpersonalIrreflexiveTransitivePerfectiveSign,
                CRMSclasses::Verb::cImpersonalIrreflexiveTransitivePerfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb13" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cImpersonalReflexiveIntransitiveImperfectiveSign,
                CRMSclasses::Verb::cImpersonalReflexiveIntransitiveImperfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb14" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cImpersonalReflexiveIntransitivePerfectiveSign,
                CRMSclasses::Verb::cImpersonalReflexiveIntransitivePerfective, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb15" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cIrreflexiveIntransitiveBiformSign,
                CRMSclasses::Verb::cIrreflexiveIntransitiveBiform, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb16" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cIrreflexiveTransitiveBiformSign,
                CRMSclasses::Verb::cIrreflexiveTransitiveBiform, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb17" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cReflexiveIntransitiveBiformSign,
                CRMSclasses::Verb::cReflexiveIntransitiveBiform, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb18" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cReflexiveIntransitiveBiformExtSign,
                CRMSclasses::Verb::cReflexiveIntransitiveBiformExt, &verbHomonim, this, NULL );
    }
    else if ( UID.Find ( "Verb19" ) == 0 )
    {
        Result = CRMLexem::ProcessFlexes ( FlexPositions, 
                CRMSclasses::Verb::cBeIrreflexiveIntransitiveSign,
                CRMSclasses::Verb::cBeIrreflexiveIntransitive, &verbHomonim, this, NULL );
    }
    delete FlexPositions;
    return Result;
}

/*____________________________________________________________________
                                                                 ___
    IdentifyFlex                                                /   \
                                                                )...(
    (for all CRMLexem descendants)                              \___/
  ____________________________________________________________________
*/

// 7 Nouns
Strings
CRMSclasses::Noun::IdentifyFlex ( CRMParadigm::FlexVar & FlexPos, const String & Form ) const
{
    int           Model = 'M';
    NounHomonim   homonim((Noun*)this);
    String        Properties = GetStaticProperties () + GetDerivedProperties ();

    if ( 4 < Paradigm->UID.Length () )
        Model = Paradigm->UID[4];
    switch ( Model )
    {
        case 'G':
            return ProcessFlex ( FlexPos, "<noun" + Properties, cNounFemale, &homonim, Paradigm, StressSchema );
        default:
            return ProcessFlex ( FlexPos, "<noun" + Properties, cNounMale, &homonim, Paradigm, StressSchema );
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \

    IdentifyStress                                              )   (
                                                                \___/
  ____________________________________________________________________
*/

Strings
CRMStressSchema::Identify ( const CRMParadigm::FlexVar& FlexPos, int StressDelta ) const
{
    CRMStressSchema *NestedSchema;
    int   i, j;
    int   FPos, FlexPosVar, NestedFlexPos, NestedFlexPosVar;
    Strings Result;
    
    // Calculate general variation:
    // If P-class has flex variation, but schema does not then discard flex var value for stress
    FPos = FlexPos.Pos % 512;
    FlexPosVar = ( 1 < Items[FPos]->Vars.Count () ) ? FlexPos.Var % 512 : 0;
    for ( i = 0; i < Items[FPos]->Vars[FlexPosVar]->Stresses.Count (); ++i )
    {
        if ( (NestedSchema = Items[FPos]->Vars[FlexPosVar]->Stresses[i].Nested) )
        {
            NestedFlexPos    = ( FlexPos.Pos >> 9 ) - 1;
            NestedFlexPosVar = ( NestedSchema->Items[NestedFlexPos]->Vars.Count () > 1 ) ? ( FlexPos.Var >> 9 ) - 1 : 0;
            // Multiple nested stress var
            for ( j = 0; j < NestedSchema->Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses.Count (); ++j )
            {
                if ( NestedSchema->Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses[j].Aux )
                    //Result.Add ( " stress=" + String::FromInt ( NestedSchema->Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses[j].Pos + StressDelta ) + " auxstress=" + String::FromInt ( Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses[j].Aux + StressDelta ) );
					Result.Add ( String::FromInt ( NestedSchema->Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses[j].Pos + StressDelta ) + "|" + String::FromInt ( Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses[j].Aux + StressDelta ) );
				else
                    //Result.Add ( " stress=" + String::FromInt ( NestedSchema->Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses[j].Pos + StressDelta ) );
					Result.Add ( String::FromInt ( NestedSchema->Items[NestedFlexPos]->Vars[NestedFlexPosVar]->Stresses[j].Pos + StressDelta ) );
			}
        }
        else if ( Items[FPos]->Vars[FlexPosVar]->Stresses[i].Aux )
            //Result.Add ( " stress=" + String::FromInt ( Items[FPos]->Vars[FlexPosVar]->Stresses[i].Pos ) + " auxstress=" + String::FromInt ( Items[FPos]->Vars[FlexPosVar]->Stresses[i].Aux + StressDelta ) );
            Result.Add ( String::FromInt ( Items[FPos]->Vars[FlexPosVar]->Stresses[i].Pos ) + "|" + String::FromInt ( Items[FPos]->Vars[FlexPosVar]->Stresses[i].Aux + StressDelta ) );
        else
            //Result.Add ( " stress=" + String::FromInt ( Items[FPos]->Vars[FlexPosVar]->Stresses[i].Pos + StressDelta ) );
			Result.Add ( String::FromInt ( Items[FPos]->Vars[FlexPosVar]->Stresses[i].Pos + StressDelta ) );
	}
    return Result;
}
