/*____________________________________________________________________

  Project:

      Russian  Morphology   Unit
      http://axofiber.org.ru/rm/
      __________________________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru

  (C) Volkova Irina,                              1982--2003, AMC, MSU
      volkova@cs.msu.su

  Component : Static RSU data inititalization
  Comments  :   
  ____________________________________________________________________
*/
#include "Lib/stdafx.h"

#include "RMStock.h"

// Static elements definition
const int CRMStockElement::GENERAL_DIC = 0;
const int CRMStockElement::CUSTOM_DIC  = 1;
String   CRMStockElement::LB = "<";
String   CRMStockElement::RB = ">";

Strings  CRMLexem::cCases;
Strings  CRMLexem::cCaseNames;
// 4 ..
Strings  CRMSclasses::Number::cPronounAdjective;
Strings  CRMSclasses::Number::cNumberTwo;
Strings  CRMSclasses::Number::cNumberThree;
Strings  CRMSclasses::Number::cNumberBiForm;
// 7 ..
Strings  CRMSclasses::Noun::cNounMale;
Strings  CRMSclasses::Noun::cNounFemale;
Strings  CRMSclasses::Noun::cNounMaleOrFemale;
Strings  CRMSclasses::CRMNounProperSurname::cProperSurname;
const String	 CRMSclasses::CRMNounProperSurname::cConstantParadigmID("NS_0");
// 8-1
Strings  CRMSclasses::CRMAdjective::cAdjective;
// 8-5
Strings  CRMSclasses::CRMPossesiveAdjective::cPossesiveAdjective;
// 9-1
String   CRMSclasses::Verb::cIrreflexiveIntransitiveImperfectiveSign;
Strings  CRMSclasses::Verb::cIrreflexiveIntransitiveImperfective;
// 9-2
String   CRMSclasses::Verb::cIrreflexiveIntransitivePerfectiveSign;
Strings  CRMSclasses::Verb::cIrreflexiveIntransitivePerfective;
// 9-3
String   CRMSclasses::Verb::cIrreflexiveTransitiveImperfectiveSign;
Strings  CRMSclasses::Verb::cIrreflexiveTransitiveImperfective;
// 9-4
String   CRMSclasses::Verb::cIrreflexiveTransitivePerfectiveSign;
Strings  CRMSclasses::Verb::cIrreflexiveTransitivePerfective;
// 9-5
String   CRMSclasses::Verb::cReflexiveImperfectiveSign;
Strings  CRMSclasses::Verb::cReflexiveImperfective;
// 9-6
String   CRMSclasses::Verb::cReflexivePerfectiveSign;
Strings  CRMSclasses::Verb::cReflexivePerfective;
// 9-7
String   CRMSclasses::Verb::cFrequentativeIrreflexiveIntransitiveImperfectiveSign;
Strings  CRMSclasses::Verb::cFrequentativeIrreflexiveIntransitiveImperfective;
// 9-8
String   CRMSclasses::Verb::cFrequentativeIrreflexiveTransitiveImperfectiveSign;
Strings  CRMSclasses::Verb::cFrequentativeIrreflexiveTransitiveImperfective;
// 9-9
String   CRMSclasses::Verb::cImpersonalIrreflexiveIntransitiveImperfectiveSign;
Strings  CRMSclasses::Verb::cImpersonalIrreflexiveIntransitiveImperfective;
// 9-10
String   CRMSclasses::Verb::cImpersonalIrreflexiveIntransitivePerfectiveSign;
Strings  CRMSclasses::Verb::cImpersonalIrreflexiveIntransitivePerfective;
// 9-11
String   CRMSclasses::Verb::cImpersonalIrreflexiveTransitiveImperfectiveSign;
Strings  CRMSclasses::Verb::cImpersonalIrreflexiveTransitiveImperfective;
// 9-12
String   CRMSclasses::Verb::cImpersonalIrreflexiveTransitivePerfectiveSign;
Strings  CRMSclasses::Verb::cImpersonalIrreflexiveTransitivePerfective;
// 9-13
String   CRMSclasses::Verb::cImpersonalReflexiveIntransitiveImperfectiveSign;
Strings  CRMSclasses::Verb::cImpersonalReflexiveIntransitiveImperfective;
// 9-14
String   CRMSclasses::Verb::cImpersonalReflexiveIntransitivePerfectiveSign;
Strings  CRMSclasses::Verb::cImpersonalReflexiveIntransitivePerfective;
// 9-15
String   CRMSclasses::Verb::cIrreflexiveIntransitiveBiformSign;
Strings  CRMSclasses::Verb::cIrreflexiveIntransitiveBiform;
// 9-16
String   CRMSclasses::Verb::cIrreflexiveTransitiveBiformSign;
Strings  CRMSclasses::Verb::cIrreflexiveTransitiveBiform;
// 9-17
String   CRMSclasses::Verb::cReflexiveIntransitiveBiformSign;
Strings  CRMSclasses::Verb::cReflexiveIntransitiveBiform;
// 9-18
String   CRMSclasses::Verb::cReflexiveIntransitiveBiformExtSign;
Strings  CRMSclasses::Verb::cReflexiveIntransitiveBiformExt;
// 9-19
String   CRMSclasses::Verb::cBeIrreflexiveIntransitiveSign;
Strings  CRMSclasses::Verb::cBeIrreflexiveIntransitive;

// CRMParadigm
Strings   CRMParadigm::NounSuffixes;
Strings   CRMParadigm::AdjectiveSuffixes;
Strings   CRMParadigm::VerbSuffixes;


CRMLexem::MorphDataInit::MorphDataInit ()
{
    CRMLexem::cCases.
    Add ( " case=nominative" ) .
    Add ( " case=genitive" ) .
    Add ( " case=dative" ) .
    Add ( " case=accusative" ) .
    Add ( " case=instrumental" ) .
    Add ( " case=prepositional" );

    CRMLexem::cCaseNames.
    Add ( "nominative" ) .
    Add ( "genitive" ) .
    Add ( "dative" ) .
    Add ( "accusative" ) .
    Add ( "instrumental" ) .
    Add ( "prepositional" );
    
    // 4-1
    CRMSclasses::Number::cPronounAdjective.
    Add ( " animate=any gender=male number=single case=nominative" ) .
    Add ( " animate=any gender=male number=single case=genitive" ) .
    Add ( " animate=any gender=male number=single case=dative" ) .
    Add ( " animate=any gender=male number=single case=instrumental" ) .
    Add ( " animate=any gender=male number=single case=prepositional" ) .
    Add ( " animate=any gender=neuter number=single case=nominative" ) .
    Add ( " animate=any gender=neuter number=single case=genitive" ) .
    Add ( " animate=any gender=neuter number=single case=dative" ) .
    Add ( " animate=any gender=neuter number=single case=instrumental" ) .
    Add ( " animate=any gender=neuter number=single case=prepositional" ) .
    Add ( " animate=any gender=female number=single case=nominative" ) .
    Add ( " animate=any gender=female number=single case=genitive" ) .
    Add ( " animate=any gender=female number=single case=dative" ) .
//	GIG: 30.08.2014: в случае женского рода вычисляется не винительный, а предложный падеж
    Add ( " animate=any gender=female number=single case=accusative" ) .
    Add ( " animate=any gender=female number=single case=instrumental" ) .
//    Add ( " animate=any gender=female number=single case=prepositional" ) .
    Add ( " animate=any gender=any number=plural case=nominative" ) .
    Add ( " animate=any gender=any number=plural case=genitive" ) .
    Add ( " animate=any gender=any number=plural case=dative" ) .
    Add ( " animate=any gender=any number=plural case=instrumental" ) .
    Add ( " animate=any gender=any number=plural case=prepositional" );

    // 4-2
    CRMSclasses::Number::cNumberTwo.
    Add ( " animate=any gender=male case=nominative" ) .
    Add ( " animate=any gender=male case=genitive" ) .
    Add ( " animate=any gender=male case=dative" ) .
    Add ( " animate=no gender=male case=accusative" ) .
    Add ( " animate=yes gender=male case=accusative" ) .
    Add ( " animate=any gender=male case=instrumental" ) .
    Add ( " animate=any gender=male case=prepositional" ) .
    Add ( " animate=any gender=neuter case=nominative" ) .
    Add ( " animate=any gender=neuter case=genitive" ) .
    Add ( " animate=any gender=neuter case=dative" ) .
    Add ( " animate=no gender=neuter case=accusative" ) .
    Add ( " animate=yes gender=neuter case=accusative" ) .
    Add ( " animate=any gender=neuter case=instrumental" ) .
    Add ( " animate=any gender=neuter case=prepositional" ) .
    Add ( " animate=any gender=female case=nominative" ) .
    Add ( " animate=any gender=female case=genitive" ) .
    Add ( " animate=any gender=female case=dative" ) .
    Add ( " animate=no gender=female case=accusative" ) .
    Add ( " animate=yes gender=female case=accusative" ) .
    Add ( " animate=any gender=female case=instrumental" ) .
    Add ( " animate=any gender=female case=prepositional" );

    // 4-3
    CRMSclasses::Number::cNumberThree.
    Add ( " animate=any case=nominative" ) .
    Add ( " animate=any case=genitive" ) .
    Add ( " animate=any case=dative" ) .
    Add ( " animate=no case=accusative" ) .
    Add ( " animate=yes case=accusative" ) .
    Add ( " animate=any case=instrumental" ) .
    Add ( " animate=any case=prepositional" );

	// 4-5
	CRMSclasses::Number::cNumberBiForm.
    Add ( " case=nominative" ) .
    Add ( " case=accusative" );


    // 7-1 7-2 7-8 and Neuter 7-5 7-6 7-12
    CRMSclasses::Noun::cNounMale.
    Add ( " case=nominative number=single" ) .
    Add ( " case=genitive number=single" ) .
    Add ( " case=dative number=single" ) .
    Add ( " case=instrumental number=single" ) .
    Add ( " case=prepositional number=single" ) .
    Add ( " case=nominative number=plural" ) .
    Add ( " case=genitive number=plural" ) .
    Add ( " case=dative number=plural" ) .
    Add ( " case=instrumental number=plural" ) .
    Add ( " case=prepositional number=plural" );

    // 7-3 7-4 7-7 7-11
    CRMSclasses::Noun::cNounFemale.
    Add ( " case=nominative number=single" ) .
    Add ( " case=genitive number=single" ) .
    Add ( " case=dative number=single" ) .
    Add ( " case=accusative number=single" ) .
    Add ( " case=instrumental number=single" ) .  
    Add ( " case=nominative number=plural" ) .
    Add ( " case=genitive number=plural" ) .
    Add ( " case=dative number=plural" ) .
    Add ( " case=instrumental number=plural" ) .
    Add ( " case=prepositional number=plural" );
    // 7-9 7-10
    CRMSclasses::Noun::cNounMaleOrFemale.
    Add ( " case=nominative number=single" ) .
    Add ( " case=genitive number=single" ) .
    Add ( " case=dative number=single" ) .
    Add ( " case=accusative number=single" ) .
    Add ( " case=instrumental number=single" ) .
    Add ( " case=prepositional number=single" ) .
    Add ( " case=nominative number=plural" ) .
    Add ( " case=genitive number=plural" ) .
    Add ( " case=dative number=plural" ) .
    Add ( " case=accusative number=plural" ) .
    Add ( " case=instrumental number=plural" ) .
    Add ( " case=prepositional number=plural" );

	//7-25
	CRMSclasses::CRMNounProperSurname::cProperSurname.
	Add(" gender=male case=nominative number=single") .
	Add(" gender=male case=genitive number=single") .
	Add(" gender=male case=dative number=single") .
	Add(" gender=male case=accusative number=single") .
	Add(" gender=male case=instrumental number=single") .
	Add(" gender=male case=prepositional number=single") .
	Add(" gender=female case=nominative number=single") .
	Add(" gender=female case=genitive number=single") .
	Add(" gender=female case=dative number=single") .
	Add(" gender=female case=accusative number=single") .
	Add(" gender=female case=instrumental number=single") .
	Add(" gender=female case=prepositional number=single") .
	Add(" gender=malefemale case=nominative number=plural") .
	Add(" gender=malefemale case=genitive number=plural") .
	Add(" gender=malefemale case=dative number=plural") .
	Add(" gender=malefemale case=accusative number=plural") .
	Add(" gender=malefemale case=instrumental number=plural") .
	Add(" gender=malefemale case=prepositional number=plural");

    // 8-1
    CRMSclasses::CRMAdjective::cAdjective.
    Add ( " animate=any gender=male case=nominative number=single" ) .
    Add ( " animate=any gender=male case=genitive number=single" ) .
    Add ( " animate=any gender=male case=dative number=single" ) .
    Add ( " animate=any gender=male case=instrumental number=single" ) .
    Add ( " animate=any gender=male case=prepositional number=single" ) .
    Add ( " animate=any gender=neuter case=nominative number=single" ) .
    Add ( " animate=any gender=neuter case=genitive number=single" ) .
    Add ( " animate=any gender=neuter case=dative number=single" ) .
    Add ( " animate=any gender=neuter case=instrumental number=single" ) .
    Add ( " animate=any gender=neuter case=prepositional number=single" ) .
    Add ( " animate=any gender=female case=nominative number=single" ) .
    Add ( " animate=any gender=female case=genitive number=single" ) .
    Add ( " animate=any gender=female case=dative number=single" ) .
    Add ( " animate=any gender=female case=accusative number=single" ) .
    Add ( " animate=any gender=female case=instrumental number=single" ) .
    Add ( " animate=any gender=any case=nominative number=plural" ) .
    Add ( " animate=any gender=any case=genitive number=plural" ) .
    Add ( " animate=any gender=any case=dative number=plural" ) .
    Add ( " animate=any gender=any case=instrumental number=plural" ) .
    Add ( " animate=any gender=any case=prepositional number=plural" ) .
    Add ( " form=short gender=male number=single" ) .
    Add ( " form=short gender=female number=single" ) .
    Add ( " form=short gender=neuter number=single" ) .
    Add ( " form=short gender=any number=plural" ) .
    Add ( " form=comparative degree=strong" ); // see AdjectiveHomonim hack...

    // 8-5
    CRMSclasses::CRMPossesiveAdjective::cPossesiveAdjective.
    Add ( " animate=any gender=male number=single case=nominative" ) .
    Add ( " animate=any gender=male number=single case=genitive" ) .
    Add ( " animate=any gender=male number=single case=dative" ) .
    Add ( " animate=any gender=male number=single case=instrumental" ) .
    Add ( " animate=any gender=male number=single case=prepositional" ) .
    Add ( " animate=any gender=neuter number=single case=nominative" ) .
    Add ( " animate=any gender=neuter number=single case=genitive" ) .
    Add ( " animate=any gender=neuter number=single case=dative" ) .
    Add ( " animate=any gender=neuter number=single case=instrumental" ) .
    Add ( " animate=any gender=neuter number=single case=prepositional" ) .
    Add ( " animate=any gender=female number=single case=nominative" ) .
    Add ( " animate=any gender=female number=single case=genitive" ) .
    Add ( " animate=any gender=female number=single case=dative" ) .
// GIG: 30.08.2014: в случае женского рода вычисляется не винительный, а предложный падеж
    Add ( " animate=any gender=female number=single case=accusative" ) .
	Add ( " animate=any gender=female number=single case=instrumental" ) .
//    Add ( " animate=any gender=female number=single case=prepositional" ) .
    Add ( " animate=any gender=any number=plural case=nominative" ) .
    Add ( " animate=any gender=any number=plural case=genitive" ) .
    Add ( " animate=any gender=any number=plural case=dative" ) .
    Add ( " animate=any gender=any number=plural case=instrumental" ) .
    Add ( " animate=any gender=any number=plural case=prepositional" );

    // 9-1
    CRMSclasses::Verb::cIrreflexiveIntransitiveImperfectiveSign = "<verb reflexive=no transitive=no perfective=no";
    CRMSclasses::Verb::cIrreflexiveIntransitiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=present person=1 gender=any number=single" ) .
    Add ( " tense=present person=2 gender=any number=single" ) .
    Add ( " tense=present person=3 gender=any number=single" ) .
    Add ( " tense=present person=1 gender=any number=plural" ) .
    Add ( " tense=present person=2 gender=any number=plural" ) .
    Add ( " tense=present person=3 gender=any number=plural" ) .
    Add ( " mood=imperative person=2 gender=any number=single" ) .
    Add ( " mood=imperative person=2 gender=any number=plural" ) .
    Add ( " tense=past person=any gender=male number=single" ) .
    Add ( " tense=past person=any gender=female number=single" ) .
    Add ( " tense=past person=any gender=neuter number=single" ) .
    Add ( " tense=past person=any gender=any number=plural" ) .
    Add ( " mode=gerund tense=present" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=present" ) .
    Add ( " mode=participle voice=active tense=past" );

    // 9-2
    CRMSclasses::Verb::cIrreflexiveIntransitivePerfectiveSign = "<verb reflexive=no transitive=no perfective=yes";
    CRMSclasses::Verb::cIrreflexiveIntransitivePerfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=future person=1 gender=any number=single" ) .
    Add ( " tense=future person=2 gender=any number=single" ) .
    Add ( " tense=future person=3 gender=any number=single" ) .
    Add ( " tense=future person=1 gender=any number=plural" ) .

    Add ( " tense=future person=2 gender=any number=plural" ) .
    Add ( " tense=future person=3 gender=any number=plural" ) .
    Add ( " mood=imperative person=2 gender=any number=single" ) .
    Add ( " mood=imperative person=2 gender=any number=plural" ) .
    Add ( " tense=past person=any gender=male number=single" ) .
    Add ( " tense=past person=any gender=female number=single" ) .
    Add ( " tense=past person=any gender=neuter number=single" ) .
    Add ( " tense=past person=any gender=any number=plural" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=past" );

    // 9-3
    CRMSclasses::Verb::cIrreflexiveTransitiveImperfectiveSign = "<verb reflexive=no transitive=yes perfective=no";
    CRMSclasses::Verb::cIrreflexiveTransitiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=present person=1 gender=any number=single" ) .
    Add ( " tense=present person=2 gender=any number=single" ) .
    Add ( " tense=present person=3 gender=any number=single" ) .
    Add ( " tense=present person=1 gender=any number=plural" ) .
    Add ( " tense=present person=2 gender=any number=plural" ) .
    Add ( " tense=present person=3 gender=any number=plural" ) .
    Add ( " mood=imperative person=2 gender=any number=single" ) .
    Add ( " mood=imperative person=2 gender=any number=plural" ) .
    Add ( " tense=past person=any gender=male number=single" ) .
    Add ( " tense=past person=any gender=female number=single" ) .
    Add ( " tense=past person=any gender=neuter number=single" ) .
    Add ( " tense=past person=any gender=any number=plural" ) .
    Add ( " mode=gerund tense=present" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=present" ) .
    Add ( " mode=participle voice=passive tense=present" ) .
    Add ( " mode=participle voice=active tense=past" ) .
    Add ( " mode=participle voice=passive tense=past" ) .
    Add ( " mode=infinitive vform=passive" ) .
    Add ( " vform=passive tense=present person=3 gender=any number=single" ) .
    Add ( " vform=passive tense=present person=3 gender=any number=plural" ) .
    Add ( " vform=passive tense=past person=any gender=male number=single" ) .
    Add ( " vform=passive tense=past person=any gender=female number=single" ) .
    Add ( " vform=passive tense=past person=any gender=neuter number=single" ) .
    Add ( " vform=passive tense=past person=any gender=any number=plural" ) .
    Add ( " vform=passive mode=participle tense=present voice=active" ) .
    Add ( " vform=passive mode=participle tense=past voice=active" );

    // 9-4
    CRMSclasses::Verb::cIrreflexiveTransitivePerfectiveSign = "<verb reflexive=no transitive=yes perfective=yes";
    CRMSclasses::Verb::cIrreflexiveTransitivePerfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=future person=1 gender=any number=single" ) .
    Add ( " tense=future person=2 gender=any number=single" ) .
    Add ( " tense=future person=3 gender=any number=single" ) .
    Add ( " tense=future person=1 gender=any number=plural" ) .
    Add ( " tense=future person=2 gender=any number=plural" ) .
    Add ( " tense=future person=3 gender=any number=plural" ) .
    Add ( " mood=imperative person=2 gender=any number=single" ) .
    Add ( " mood=imperative person=2 gender=any number=plural" ) .
    Add ( " tense=past person=any gender=male number=single" ) .
    Add ( " tense=past person=any gender=female number=single" ) .
    Add ( " tense=past person=any gender=neuter number=single" ) .
    Add ( " tense=past person=any gender=any number=plural" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=past" ) .
    Add ( " mode=participle voice=passive tense=past" );

    // 9-5
    CRMSclasses::Verb::cReflexiveImperfectiveSign = "<verb reflexive=yes transitive=no perfective=no";
    CRMSclasses::Verb::cReflexiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=present person=1 gender=any number=single" ) .
    Add ( " tense=present person=2 gender=any number=single" ) .
    Add ( " tense=present person=3 gender=any number=single" ) .
    Add ( " tense=present person=1 gender=any number=plural" ) .
    Add ( " tense=present person=2 gender=any number=plural" ) .
    Add ( " tense=present person=3 gender=any number=plural" ) .
    Add ( " mood=imperative person=2 gender=any number=single" ) .
    Add ( " mood=imperative person=2 gender=any number=plural" ) .
    Add ( " tense=past person=any gender=male number=single" ) .
    Add ( " tense=past person=any gender=female number=single" ) .
    Add ( " tense=past person=any gender=neuter number=single" ) .
    Add ( " tense=past person=any gender=any number=plural" ) .
    Add ( " mode=gerund tense=present" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=present" ) .
    Add ( " mode=participle voice=active tense=past" );

    // 9-6
    CRMSclasses::Verb::cReflexivePerfectiveSign = "<verb reflexive=yes transitive=no perfective=yes";
    CRMSclasses::Verb::cReflexivePerfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=future person=1 gender=any number=single" ) .
    Add ( " tense=future person=2 gender=any number=single" ) .
    Add ( " tense=future person=3 gender=any number=single" ) .
    Add ( " tense=future person=1 gender=any number=plural" ) .
    Add ( " tense=future person=2 gender=any number=plural" ) .
    Add ( " tense=future person=3 gender=any number=plural" ) .
    Add ( " mood=imperative person=2 gender=any number=single" ) .
    Add ( " mood=imperative person=2 gender=any number=plural" ) .
    Add ( " tense=past person=any gender=male number=single" ) .
    Add ( " tense=past person=any gender=female number=single" ) .
    Add ( " tense=past person=any gender=neuter number=single" ) .
    Add ( " tense=past person=any gender=any number=plural" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=past" );

    // 9-7
    CRMSclasses::Verb::cFrequentativeIrreflexiveIntransitiveImperfectiveSign = "<verb frequentative=yes reflexive=no transitive=no perfective=no tense=past";
    CRMSclasses::Verb::cFrequentativeIrreflexiveIntransitiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " gender=male number=single" ) .
    Add ( " gender=female number=single" ) .
    Add ( " gender=neuter number=single" ) .
    Add ( " gender=any number=plural" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=past" );

    // 9-8
    CRMSclasses::Verb::cFrequentativeIrreflexiveTransitiveImperfectiveSign = "<verb frequentative=yes reflexive=no transitive=yes perfective=no tense=past";
    CRMSclasses::Verb::cFrequentativeIrreflexiveTransitiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " gender=male number=single" ) .
    Add ( " gender=female number=single" ) .
    Add ( " gender=neuter number=single" ) .
    Add ( " gender=any number=plural" ) .
    Add ( " mode=gerund tense=past" ) .
    Add ( " mode=participle voice=active tense=past" ) .
	Add ( " vform=passive mode=infinitive" ) .
    Add ( " vform=passive gender=male number=single" ) .
    Add ( " vform=passive gender=female number=single" ) .
    Add ( " vform=passive gender=neuter number=single" ) .
    Add ( " vform=passive gender=any number=plural" ) .
    Add ( " vform=passive mode=participle voice=active tense=past" );

    // 9-9
    CRMSclasses::Verb::cImpersonalIrreflexiveIntransitiveImperfectiveSign = "<verb personal=no reflexive=no transitive=no perfective=no person=3 number=single gender=neuter";
    CRMSclasses::Verb::cImpersonalIrreflexiveIntransitiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=present" ) .
    Add ( " tense=past" );

    // 9-10
    CRMSclasses::Verb::cImpersonalIrreflexiveIntransitivePerfectiveSign = "<verb personal=no reflexive=no transitive=no perfective=yes person=3 number=single gender=neuter";
    CRMSclasses::Verb::cImpersonalIrreflexiveIntransitivePerfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=future" ) .
    Add ( " tense=past" );

    // 9-11
    CRMSclasses::Verb::cImpersonalIrreflexiveTransitiveImperfectiveSign = "<verb personal=no reflexive=no transitive=yes perfective=no person=3 number=single gender=neuter";
    CRMSclasses::Verb::cImpersonalIrreflexiveTransitiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=present" ) .
    Add ( " tense=past" );

    // 9-12
    CRMSclasses::Verb::cImpersonalIrreflexiveTransitivePerfectiveSign = "<verb personal=no reflexive=no transitive=yes perfective=yes person=3 number=single gender=neuter";
    CRMSclasses::Verb::cImpersonalIrreflexiveTransitivePerfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=future" ) .
    Add ( " tense=past" );

    // 9-13
    CRMSclasses::Verb::cImpersonalReflexiveIntransitiveImperfectiveSign = "<verb personal=no reflexive=yes transitive=no perfective=no person=3 number=single gender=neuter";
    CRMSclasses::Verb::cImpersonalReflexiveIntransitiveImperfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=present" ) .
    Add ( " tense=past" );

    // 9-14
    CRMSclasses::Verb::cImpersonalReflexiveIntransitivePerfectiveSign = "<verb personal=no reflexive=yes transitive=no perfective=yes person=3 number=single gender=neuter";
    CRMSclasses::Verb::cImpersonalReflexiveIntransitivePerfective.
    Add ( " mode=infinitive" ) .
    Add ( " tense=future" ) .
    Add ( " tense=past" );

    // 9-15
    CRMSclasses::Verb::cIrreflexiveIntransitiveBiformSign = "<verb reflexive=no transitive=no biform=yes";
    CRMSclasses::Verb::cIrreflexiveIntransitiveBiform.
    Add ( " mode=infinitive perfective=any" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=plural" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=single" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=plural" ) .
    Add ( " perfective=any tense=past person=any gender=male number=single" ) .
    Add ( " perfective=any tense=past person=any gender=female number=single" ) .
    Add ( " perfective=any tense=past person=any gender=neuter number=single" ) .
    Add ( " perfective=any tense=past person=any gender=any number=plural" ) .
    Add ( " perfective=no mode=gerund tense=present" ) .
    Add ( " perfective=any mode=gerund tense=past" ) .
    Add ( " perfective=no mode=participle tense=present voice=active" ) .
    Add ( " perfective=any mode=participle tense=past voice=active" );

    // 9-16
    CRMSclasses::Verb::cIrreflexiveTransitiveBiformSign = "<verb reflexive=no transitive=yes biform=yes";
    CRMSclasses::Verb::cIrreflexiveTransitiveBiform.
    Add ( " mode=infinitive perfective=any" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=plural" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=single" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=plural" ) .
    Add ( " perfective=any tense=past person=any gender=male number=single" ) .
    Add ( " perfective=any tense=past person=any gender=female number=single" ) .
    Add ( " perfective=any tense=past person=any gender=neuter number=single" ) .
    Add ( " perfective=any tense=past person=any gender=any number=plural" ) .
    Add ( " perfective=no mode=gerund tense=present" ) .
    Add ( " perfective=any mode=gerund tense=past" ) .
    Add ( " perfective=no mode=participle  tense=present voice=active" ) .
    Add ( " perfective=no mode=participle  tense=present voice=passive" ) .
    Add ( " perfective=any mode=participle tense=past voice=active" ) .
    Add ( " perfective=any mode=participle tense=past voice=active" ) .
    Add ( " mode=infinitive perfective=no vform=passive" ) .
    Add ( " perfective=no tense=present vform=passive person=3 gender=any number=single" ) .
    Add ( " perfective=no tense=present vform=passive person=3 gender=any number=plural" ) .
    Add ( " perfective=no tense=past vform=passive person=any gender=male number=single" ) .

    Add ( " perfective=no tense=past vform=passive person=any gender=female number=single" ) .
    Add ( " perfective=no tense=past vform=passive person=any gender=neuter number=single" ) .
    Add ( " perfective=no tense=past vform=passive person=any gender=any number=plural" ) .
    Add ( " perfective=no mode=participle vform=passive tense=present voice=active" ) .
    Add ( " perfective=no mode=participle vform=passive tense=past voice=active" );

    // 9-17
    CRMSclasses::Verb::cReflexiveIntransitiveBiformSign = "<verb reflexive=yes transitive=no biform=yes";
    CRMSclasses::Verb::cReflexiveIntransitiveBiform.
    Add ( " mode=infinitive perfective=any" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=plural" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=single" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=plural" ) .
    Add ( " perfective=any tense=past person=any gender=male number=single" ) .
    Add ( " perfective=any tense=past person=any gender=female number=single" ) .
    Add ( " perfective=any tense=past person=any gender=neuter number=single" ) .
    Add ( " perfective=any tense=past person=any gender=any number=plural" ) .
    Add ( " perfective=no mode=gerund tense=present" ) .
    Add ( " perfective=any mode=gerund tense=past" ) .
    Add ( " perfective=no mode=participle tense=present voice=active" ) .
    Add ( " perfective=no mode=participle tense=past voice=active" );

    // 9-18
    CRMSclasses::Verb::cReflexiveIntransitiveBiformExtSign = "<verb reflexive=yes transitive=no biform=yes";
    CRMSclasses::Verb::cReflexiveIntransitiveBiformExt.
    Add ( " mode=infinitive perfective=yes" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=single" ) .
    Add ( " perfective=any tense=present-future person=1 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=2 gender=any number=plural" ) .
    Add ( " perfective=any tense=present-future person=3 gender=any number=plural" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=single" ) .
    Add ( " perfective=any mood=imperative person=2 gender=any number=plural" ) .
    Add ( " perfective=yes tense=past person=any gender=male number=single" ) .
    Add ( " perfective=yes tense=past person=any gender=female number=single" ) .
    Add ( " perfective=yes tense=past person=any gender=neuter number=single" ) .
    Add ( " perfective=yes tense=past person=any gender=any number=plural" ) .
    Add ( " perfective=yes mode=gerund tense=past" ) .
    Add ( " perfective=yes mode=participle tense=past voice=active" );

    // 9-19
    CRMSclasses::Verb::cBeIrreflexiveIntransitiveSign = "<verb reflexive=no transitive=no";
    CRMSclasses::Verb::cBeIrreflexiveIntransitive.
    Add ( " mode=infinitive perfective=no" ) .
    Add ( " perfective=no tense=present person=1 gender=any number=single" ) .
    Add ( " perfective=no tense=present person=2 gender=any number=single" ) .
    Add ( " perfective=no tense=present person=3 gender=any number=single" ) .
    Add ( " perfective=no tense=present person=1 gender=any number=plural" ) .
    Add ( " perfective=no tense=present person=2 gender=any number=plural" ) .
    Add ( " perfective=no tense=present person=3 gender=any number=plural" ) .
    Add ( " perfective=yes tense=future person=1 gender=any number=single" ) .
    Add ( " perfective=yes tense=future person=2 gender=any number=single" ) .
    Add ( " perfective=yes tense=future person=3 gender=any number=single" ) .
    Add ( " perfective=yes tense=future person=1 gender=any number=plural" ) .
    Add ( " perfective=yes tense=future person=2 gender=any number=plural" ) .
    Add ( " perfective=yes tense=future person=3 gender=any number=plural" ) .
    Add ( " perfective=no mood=imperative person=2 gender=any number=single" ) .
    Add ( " perfective=no mood=imperative person=2 gender=any number=plural" ) .
    Add ( " perfective=no tense=past person=any gender=male number=single" ) .
    Add ( " perfective=no tense=past person=any gender=female number=single" ) .
    Add ( " perfective=no tense=past person=any gender=neuter number=single" ) .
    Add ( " perfective=no tense=past person=any gender=any number=plural" ) .
    Add ( " perfective=no mode=gerund tense=present" ) .
    Add ( " perfective=no mode=gerund tense=past" ) .
    Add ( " perfective=no mode=participle tense=past voice=active" );

    // CRMParadigm init
    CRMParadigm::NounSuffixes.
    Add ( "анин" ) . Add ( "ател" ) . Add ( "атор" ) .  Add ( "афи" ) . Add ( "граф" ) .
    Add ( "онок" ) . Add ( "етел" ) . Add ( "етр" )  . Add ( "ец" )   . Add ( "етри" ) .
    Add ( "изм" )  . Add ( "ител" ) . Add ( "истк" ) . Add ( "иц" )   . Add ( "ктор" ) .
    Add ( "нт" )   . Add ( "оги" )  . Add ( "олог" ) . Add ( "онок" ) . Add ( "ост" ) .
    Add ( "ств" )  . Add ( "ушк" )  . Add ( "ци" )   . Add ( "чик" )  . Add ( "щик" ) .
    Add ( "яни" )  . Add ( "янин" ) . Add ( "ятел" ) . Add ( "ятор");

    CRMParadigm::AdjectiveSuffixes.
    Add ( "aжн" ) . Add ( "ащ" ) . Add ( "вн" ) . Add ( "граф" ) . Add ( "еват" ) .
    Add ( "есят" ) . Add ( "идн" )  . Add ( "йш" )  . Add ( "нн" )  . Add ( "обн" ) .
    Add ( "оват" ) . Add ( "разн" ) . Add ( "сн" )  . Add ( "сот" ) . Add ( "ущ" ) .
    Add ( "цк" )   . Add ( "чат" )  . Add ( "чив" ) . Add ( "чн" )  . Add ( "ющ" ) .
    Add ( "ящ" );

    CRMParadigm::VerbSuffixes.Add ( "ава" ) . Add ( "ба" ) . Add ( "ова" ) . Add ( "ува" ) . Add ( "ыва" );
}

CRMLexem::MorphDataInit CRMLexem::Init;
