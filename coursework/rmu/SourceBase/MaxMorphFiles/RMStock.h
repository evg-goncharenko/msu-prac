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
*/

#ifndef RMSTOCK_H_2001
#define RMSTOCK_H_2001

#include "Lib/SCArray.h"
#include "Lib/Xception.h"
#include "Lib/Xml.h"
#include "RMStockData.h"

class CRMStem;
class CRMLexem;

/*____________________________________________________________________

                                                                    /
    CRMStockElement class definition                            \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMStockElement {
public:
    int DicType;
    // Static attributes
    static const int GENERAL_DIC;
    static const int CUSTOM_DIC;
    static String LB;
    static String RB;
    CRMStockElement() : DicType(GENERAL_DIC) {}
    CRMStockElement(const CRMStockElement& Element) : DicType(Element.DicType) {}
};

/*____________________________________________________________________

                                                                    /
    CRMStem class definition                                    \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMStem : public CRMStockElement {
public:
    // Attributes
    String UID;
    CSCTypeArray<String> LexemIDs;
    CSCAbstractPtrArray<CRMLexem> Lexems;

    CRMStem() { Lexems.ForceNotToOwn(); }
    CRMStem(const CRMStem& Stem) : ::CRMStockElement(Stem),
                                   UID(Stem.UID),
                                   LexemIDs(Stem.LexemIDs),
                                   Lexems(Stem.Lexems) {}
};

/*____________________________________________________________________

                                                                    /
    CRMParadigm class definition                                \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMParadigm : public CRMStockElement {
public:
    // Aggregation classes
    struct ParadigmItem {
        virtual int ItemType() const = 0;
        bool Is(String Type);
        virtual ~ParadigmItem(){};
    };

    struct Flex : public ParadigmItem {
        String ID;
        int ItemType() const { return ERMPFlex; };
    };

    struct Variation : public ParadigmItem {
        CSCTypeArray<String> Flexes;
        CSCTypeArray<String> NestedIDs;
        int ItemType() const { return ERMPVariation; };
    };

    struct Flexes : public ParadigmItem {
        CSCTypeArray<String> Items;
        CSCAbstractPtrArray<CRMParadigm> Nested;
        int ItemType() const { return ERMPFlexes; };
        Flexes() { Nested.ForceNotToOwn(); };
    };

    struct FlexVar {
        int Pos;  // Number of the flex
        int Var;  // Number of the flex variant
    };

    struct FlexPositions {
        CSCTypeArray<FlexVar> Items;
    };

    // Attributes
    String UID;
    CSCAbstractPtrArray<ParadigmItem> Items;
    CSCSet<String, Strings> Suffixes;
    CSCSet<String, Strings> SuffixesExt;

    static Strings NounSuffixes;
    static Strings AdjectiveSuffixes;
    static Strings VerbSuffixes;

    static const int COMPARATIVE_ADJ_FLEX_POSITION = 24;  // position of flex for short comparative form of adjective

    // Operations
    FlexPositions* Contains(const String& Flex, int StartPos = 0) const;
    bool ContainsFlex(const String& Flex, int StartPos = 0) const;
    Strings* GetAllFlexes() const;
    FlexPositions* GetAllFlexPositions() const;
    String GetFirstFlex() const;
    Strings IdentifyFlex(const String& Flex) const;

    CRMParadigm(){};
    CRMParadigm(const CRMParadigm& Paradigm) : ::CRMStockElement(Paradigm),
                                               UID(Paradigm.UID),
                                               Items(Paradigm.Items) {}
};

/*____________________________________________________________________

                                                                    /
    CRMStressSchema class definition                            \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMStressSchema : public CRMStockElement {
public:
    struct Stress {
        // tag "pos" or "nested"
        unsigned long Pos;  // position of the stress letter
        unsigned long Aux;  // position of the secondary stress letter
        String NestedID;
        CRMStressSchema* Nested;
        Stress() {
            Pos = 0;
            Aux = 0;
            Nested = NULL;
        };
        Stress(const Stress& AStress) : Pos(AStress.Pos),
                                        Aux(AStress.Aux),
                                        NestedID(AStress.NestedID),
                                        Nested(AStress.Nested) {}
    };

    struct StressVar {
        // tag "var"
        CSCTypeArray<Stress> Stresses;
    };

    struct FlexStress {
        // tag "f"
        CSCPtrArray<StressVar> Vars;
    };

    String UID;
    CSCPtrArray<FlexStress> Items;

    // Operations
    NumberList GetStress(const CRMParadigm::FlexVar& FlexPos, unsigned long Variation = 0, unsigned long PosLevel = 0) const;
    Strings Identify(const CRMParadigm::FlexVar& FlexPos, int StressDelta) const;

    CRMStressSchema(){};
};

/*____________________________________________________________________

                                                                    /
    CRMHomonim class definition                                 \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMHomonim {
public:
    virtual void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm){};
    virtual int StressDelta() { return 0; }
    CRMHomonim(){};
    virtual ~CRMHomonim(){};
};

/*____________________________________________________________________

                                                                    /
    CRMLexem class definition                                   \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMLexem : public CRMStockElement {
    // Operations
    static CRMLexem* CreateByNumber(int SynClass);

public:
    struct MorphDataInit {
        MorphDataInit();
    };
    static Strings cCases;
    static Strings cCaseNames;
    static MorphDataInit Init;

    // Attributes
    String UID;
    CRMParadigm* Paradigm;
    String ParadigmID;
    String StressSchemaID;
    CRMStressSchema* StressSchema;
    unsigned long UsageCounter;

    // Operations
    static CRMLexem* CreateByCode(const String& Code);
    static CRMLexem* CreateByName(const String& Name);
    String GetDerivedProperties() const {
        String Result = " UID=\"";
        Result += UID;
        Result += "\"";
        if (Paradigm) {
            Result += " paradigmID=\"";
            Result += Paradigm->UID;
            Result += "\"";
        }
        if (StressSchema) {
            Result += " schemaID=\"";
            Result += StressSchema->UID;
            Result += "\"";
        }
        // GIG: 29.03.2013 - old dream comes true
        //Result += " uc=";
        //return Result += String::FromInt ( UsageCounter );
        return Result;
    }
    virtual String GetProperties() const {
        String Result = LB;
        //Result += GetSynClassName ();
        // GIG: 13.05.2018 - output ERMClass
        Result += GetSynClassName() + " ms_class=" + String::FromInt(GetSynClass());
        return Result += GetDerivedProperties();
    };
    virtual ERMMorphoClass GetSynClass() const = 0;
    virtual String GetSynClassName() const = 0;
    virtual Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const = 0;
    static Strings ProcessFlex(CRMParadigm::FlexVar& FlexPos,
                               const String& Properties,
                               const Strings& FlexData,
                               CRMHomonim* Homonim,
                               const CRMParadigm* const Paradigm,
                               CRMStressSchema* StressSchema);
    static Strings ProcessFlexes(CRMParadigm::FlexPositions* Positions,
                                 const String& Properties,
                                 const Strings& FlexData,
                                 CRMHomonim* Homonim,
                                 const CRMParadigm* const Paradigm,
                                 CRMStressSchema* StressSchema);
    String ToXML() const { return GetProperties() + " />"; }
    CRMLexem() : Paradigm(NULL),
                 StressSchema(NULL),
                 UsageCounter(0) {}
    CRMLexem(const CRMLexem& Lexem) : ::CRMStockElement(Lexem),
                                      UID(Lexem.UID),
                                      ParadigmID(Lexem.ParadigmID),
                                      StressSchema(Lexem.StressSchema) {}
    virtual ~CRMLexem() { ; };
};

/*____________________________________________________________________

                                                                    /
    CRMUnifiedLexemData class definition                        \  /
                                                                 \/
  ____________________________________________________________________
*/

class CRMUnifiedLexemData : public CRMLexem {
public:
    // Attributes
    char Animate;
    char Gender;
    char Number;
    char Person;
};

class CRMSclasses {
public:
    /*____________________________________________________________________

                                                                    /
    Static class definition                                     \  /
                                                                 \/
  ____________________________________________________________________
*/

    class Static : public CRMLexem {
    public:
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            Strings Empty;
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), Empty, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMParticle class definition                                \  /
    1                                                            \/
  ____________________________________________________________________
*/

    class CRMParticle : public Static {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_PARTICLE; }
        String GetSynClassName() const { return ERMClassNames[ERM_PARTICLE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMInterjection class definition                            \  /
    2                                                            \/
  ____________________________________________________________________
*/

    class CRMInterjection : public Static {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_INTERJECTION; }
        String GetSynClassName() const { return ERMClassNames[ERM_INTERJECTION][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMPersonalPronoun class definition                         \  /
    3-1                                                          \/
  ____________________________________________________________________
*/

    class CRMPersonalPronoun : public CRMLexem {
    public:
        // Attributes
        char Animate;
        char Gender;
        char Number;
        char Person;
        // Operations
        String GetProperties() const {
            String Result = LB;
            Result += GetSynClassName();
            Result += GetDerivedProperties();
            Result += " animate=";
            Result += ERMAnimates[(int)Animate];
            Result += " gender=";
            Result += ERMGenders[(int)Gender];
            Result += " number=";
            Result += ERMNumbers[(int)Number];
            Result += " person=";
            return Result += ERMPersons[(int)Person];
        };
        ERMMorphoClass GetSynClass() const { return ERM_PERSONAL_PRONOUN; }
        String GetSynClassName() const { return ERMClassNames[ERM_PERSONAL_PRONOUN][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form = "") const {
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cCases, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMReflexivePronoun class definition                        \  /
    3-2                                                          \/
  ____________________________________________________________________
*/

    class CRMReflexivePronoun : public CRMLexem {
    public:
        // Attributes
        char Animate;
        char Gender;
        char Number;
        // Operations
        String GetProperties() const {
            String Result = LB;
            Result += GetSynClassName();
            Result += GetDerivedProperties();
            Result += " animate=";
            Result += ERMAnimates[(int)Animate];
            Result += " gender=";
            Result += ERMGenders[(int)Gender];
            Result += " number=";
            return Result += ERMNumbers[(int)Number];
        };
        ERMMorphoClass GetSynClass() const { return ERM_REFLEXIVE_PRONOUN; }
        String GetSynClassName() const { return ERMClassNames[ERM_REFLEXIVE_PRONOUN][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form = "") const {
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cCases, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMPronoun class definition                                 \  /
    3-3                                                          \/
  ____________________________________________________________________
*/

    class CRMPronoun : public CRMLexem {
    public:
        // Attributes
        char Animate;
        char Gender;
        char Number;
        // Operations
        String GetProperties() const {
            String Result = LB;
            Result += GetSynClassName();
            Result += GetDerivedProperties();
            Result += " animate=";
            Result += ERMAnimates[(int)Animate];
            Result += " gender=";
            Result += ERMGenders[(int)Gender];
            Result += " number=";
            return Result += ERMNumbers[(int)Number];
        };
        ERMMorphoClass GetSynClass() const { return ERM_PRONOUN; }
        String GetSynClassName() const { return ERMClassNames[ERM_PRONOUN][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form = "") const {
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cCases, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNumber                                                   \  /
                                                                 \/
  ____________________________________________________________________
*/

    class Number : public CRMLexem {
    public:
        static Strings cPronounAdjective;
        static Strings cNumberTwo;
        static Strings cNumberThree;
        static Strings cNumberBiForm;
    };

    /*____________________________________________________________________

                                                                    /
    CRMNumberOne class definition                               \  /
    4-1                                                          \/
  ____________________________________________________________________
*/

    class CRMNumberOne : public Number {
    public:
        class NumberOneHomonim : public CRMHomonim {
        public:
            void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm) {
                String props;
                switch (FlexPosition) {
                    case 0:
                        props = " animate=no gender=male number=single case=accusative";
                        break;
                    case 1:
                        props = " animate=yes gender=male number=single case=accusative";
                        break;
                    case 5:
                        props = " animate=any gender=neuter number=single case=accusative";
                        break;
                    case 12:
                        props = " animate=any gender=female number=single case=prepositional";
                        break;
                    case 15:
                        props = " animate=no gender=any number=plural case=accusative";
                        break;
                    case 16:
                        props = " animate=yes gender=any number=plural case=accusative";
                }
                if (!props.IsEmpty()) {
                    Data.Add(Properties + props);
                }
            }
        };

        ERMMorphoClass GetSynClass() const { return ERM_NUMBER_ONE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NUMBER_ONE][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            NumberOneHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cPronounAdjective, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNumberTwo class definition                               \  /
    4-2                                                          \/
  ____________________________________________________________________
*/

    class CRMNumberTwo : public Number {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NUMBER_TWO; }
        String GetSynClassName() const { return ERMClassNames[ERM_NUMBER_TWO][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cNumberTwo, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNumberThree class definition                             \  /
    4-3                                                          \/
  ____________________________________________________________________
*/

    class CRMNumberThree : public Number {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NUMBER_THREE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NUMBER_THREE][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cNumberThree, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNumber class definition                                  \  /
    4-4                                                          \/
  ____________________________________________________________________
*/

    class CRMNumber : public Number {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NUMBER; }
        String GetSynClassName() const { return ERMClassNames[ERM_NUMBER][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cCases, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNumberBiForm class definition                            \  /
    4-5                                                          \/
  ____________________________________________________________________
*/

    class CRMNumberBiForm : public Number {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NUMBER_BIFORM; }
        String GetSynClassName() const { return ERMClassNames[ERM_NUMBER_BIFORM][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            CRMHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cNumberBiForm, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNoun                                                     \  /
                                                                 \/
  ____________________________________________________________________
*/

    class Noun : public CRMLexem {
    public:
        enum {
            ESPropNone = 0,
            ESPropFirst = 1,
            ESPropAnimateYes = ESPropFirst << 0,
            ESPropAnimateNo = ESPropFirst << 1,
            ESPropAnimateAny = ESPropAnimateYes | ESPropAnimateNo,
            ESPropGenderMale = ESPropFirst << 2,
            ESPropGenderFemale = ESPropFirst << 3,
            ESPropGenderMaleFemale = ESPropFirst << 4,  //ESPropGenderMale | ESPropGenderFemale,
            ESPropGenderNeutral = ESPropFirst << 5,
            ESPropGenderMaleOrFemale = ESPropFirst << 6,
            ESPropGenderNeutralOrMale = ESPropFirst << 7,
            ESPropGenderNeutralOrFemale = ESPropFirst << 8,
        };
        class NounHomonim : public CRMHomonim {
        private:
            Noun* _pRefNoun;

        public:
            // GIG: constructor added
            NounHomonim(Noun* pNoun) : _pRefNoun(pNoun) {
            }
            Noun* GetReferenceNoun() const { return _pRefNoun; }
            void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm) {
                int Model = 'M';
                if (4 < Paradigm->UID.Length())
                    Model = Paradigm->UID[4];
                switch (Model) {
                    case 'M':
                    case 'S':
                        // VIA: male and neutral: add accusative in case of case=nominative and  animate=no and
                        //	add accusative in case of case=genitive and  animate=yes
                        // BUT ONLY for nouns which P-class is NOT:
                        // NounM_2.3 NounM_2.4 NounG_1.100 NounPl_1 NounMGD_1 NounMGD_2 NounMGD_3 NounMGD_4
                        if (NonComputableAccusativeParadigm(Paradigm))
                            break;
                        if (_pRefNoun->GetStaticPropData() & ESPropAnimateNo) {
                            switch (FlexPosition) {
                                case 0:
                                    Data.Add(Properties + " case=accusative number=single");
                                    break;
                                case 5:
                                    Data.Add(Properties + " case=accusative number=plural");
                                    break;
                            }
                        }
                        if (_pRefNoun->GetStaticPropData() & ESPropAnimateYes) {
                            switch (FlexPosition) {
                                case 1:
                                    Data.Add(Properties + " case=accusative number=single");
                                    break;
                                case 6:
                                    Data.Add(Properties + " case=accusative number=plural");
                                    break;
                            }
                        }
                        break;
                    case 'G':
                        switch (FlexPosition) {
                            case 2:
                                Data.Add(Properties + " case=prepositional number=single");
                                break;
                            case 5:
                                if (_pRefNoun->GetStaticPropData() & ESPropAnimateNo)
                                    Data.Add(Properties + " case=accusative number=plural");
                                break;
                            case 6:
                                if (_pRefNoun->GetStaticPropData() & ESPropAnimateYes)
                                    Data.Add(Properties + " case=accusative number=plural");
                                break;
                        }
                        break;
                }
            }

        private:
            // GIG: check for non-computable accusative paradigms
            static bool NonComputableAccusativeParadigm(const CRMParadigm* const pParadigm) {
                const char* UID = pParadigm->UID.c_str();
                return strcmp(UID, "NounM_2.3") == 0 ||
                       strcmp(UID, "NounM_2.4") == 0 ||
                       strcmp(UID, "NounG_1.100") == 0 ||
                       strcmp(UID, "NounPl_1") == 0 ||
                       strcmp(UID, "NounMGD_1") == 0 ||
                       strcmp(UID, "NounMGD_2") == 0 ||
                       strcmp(UID, "NounMGD_3") == 0 ||
                       strcmp(UID, "NounMGD_4") == 0;
            }
        };  // class NounHomonim
        virtual String GetStaticProperties() const = 0;
        // VIA:GIG: Add for dynamic request for animity
        virtual int GetStaticPropData() const = 0;
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const;
        static Strings cNounMale;
        static Strings cNounFemale;
        static Strings cNounMaleOrFemale;
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounMaleInanimate                                        \  /
    7-1                                                          \/
  ____________________________________________________________________
*/

    class CRMNounMaleInanimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_MALE_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_MALE_INANIMATE][1]; }
        String GetStaticProperties() const { return " animate=no gender=male"; }
        int GetStaticPropData() const { return ESPropAnimateNo | ESPropGenderMale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounMaleAnimate                                          \  /
    7-2                                                          \/
  ____________________________________________________________________
*/

    class CRMNounMaleAnimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_MALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_MALE_ANIMATE][1]; }
        String GetStaticProperties() const { return " animate=yes gender=male"; }
        int GetStaticPropData() const { return ESPropAnimateYes | ESPropGenderMale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounFemaleInanimate                                      \  /
    7-3                                                          \/
  ____________________________________________________________________
*/

    class CRMNounFemaleInanimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_FEMALE_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_FEMALE_INANIMATE][1]; }

        String GetStaticProperties() const { return " animate=no gender=female"; }
        int GetStaticPropData() const { return ESPropAnimateNo | ESPropGenderFemale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounFemaleAnimate                                        \  /
    7-4                                                          \/
  ____________________________________________________________________
*/

    class CRMNounFemaleAnimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_FEMALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_FEMALE_ANIMATE][1]; }
        String GetStaticProperties() const { return " animate=yes gender=female"; }
        int GetStaticPropData() const { return ESPropAnimateYes | ESPropGenderFemale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounNeuterInanimate                                      \  /
    7-5                                                          \/
  ____________________________________________________________________
*/

    class CRMNounNeuterInanimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_NEUTER_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_NEUTER_INANIMATE][1]; }
        String GetStaticProperties() const { return " animate=no gender=neuter"; }
        int GetStaticPropData() const { return ESPropAnimateNo | ESPropGenderNeutral; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounNeuterAnimate                                        \  /
    7-6                                                          \/
  ____________________________________________________________________
*/

    class CRMNounNeuterAnimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_NEUTER_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_NEUTER_ANIMATE][1]; }
        String GetStaticProperties() const { return " animate=yes gender=neuter"; }
        int GetStaticPropData() const { return ESPropAnimateYes | ESPropGenderNeutral; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounMaleFemaleAnimate                                    \  /
    7-7                                                          \/
  ____________________________________________________________________
*/

    class CRMNounMaleFemaleAnimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_MALEFEMALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_MALEFEMALE_ANIMATE][1]; }
        String GetStaticProperties() const { return " animate=yes gender=malefemale"; }
        int GetStaticPropData() const { return ESPropAnimateYes | ESPropGenderMaleFemale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounMale                                                 \  /
    7-8                                                          \/
  ____________________________________________________________________
*/

    class CRMNounMale : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_MALE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_MALE][1]; }
        String GetStaticProperties() const { return " animate=any gender=male"; }
        int GetStaticPropData() const { return ESPropAnimateAny | ESPropGenderMale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounMaleOrFemaleInanimate                                \  /
    7-9                                                          \/
  ____________________________________________________________________
*/

    class CRMNounMaleOrFemaleInanimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_MALE_OR_FEMALE_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_MALE_OR_FEMALE_INANIMATE][1]; }
        String GetStaticProperties() const { return " animate=no gender=maleorfemale"; }
        int GetStaticPropData() const { return ESPropAnimateNo | ESPropGenderMaleOrFemale; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            NounHomonim homonim((Noun*)this);
            return ProcessFlex(FlexPos, "<noun" + GetStaticProperties() + GetDerivedProperties(), cNounMaleOrFemale, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounMaleOrFemaleAnimate                                  \  /
    7-10                                                         \/
  ____________________________________________________________________
*/

    class CRMNounMaleOrFemaleAnimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_MALE_OR_FEMALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_MALE_OR_FEMALE_ANIMATE][1]; }
        String GetStaticProperties() const { return " animate=yes gender=maleorfemale"; }
        int GetStaticPropData() const { return ESPropAnimateYes | ESPropGenderMaleOrFemale; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            NounHomonim homonim((Noun*)this);
            return ProcessFlex(FlexPos, "<noun" + GetStaticProperties() + GetDerivedProperties(), cNounMaleOrFemale, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounFemale                                               \  /
    7-11                                                         \/
  ____________________________________________________________________
*/

    class CRMNounFemale : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_FEMALE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_FEMALE][1]; }
        String GetStaticProperties() const { return " animate=any gender=female"; }
        int GetStaticPropData() const { return ESPropAnimateAny | ESPropGenderFemale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounNeuterOrFemaleInanimate                              \  /
    7-12                                                         \/
  ____________________________________________________________________
*/

    class CRMNounNeuterOrFemaleInanimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_NEUTER_OR_FEMALE_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_NEUTER_OR_FEMALE_INANIMATE][1]; }
        String GetStaticProperties() const { return " animate=no gender=neuterorfemale"; }
        int GetStaticPropData() const { return ESPropAnimateNo | ESPropGenderNeutralOrFemale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounNeuter                                               \  /
    7-13                                                         \/
  ____________________________________________________________________
*/

    class CRMNounNeuter : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_NEUTER; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_NEUTER][1]; }
        String GetStaticProperties() const { return " animate=any gender=neuter"; }
        int GetStaticPropData() const { return ESPropAnimateAny | ESPropGenderNeutral; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounNeuterOrMaleAnimate                                  \  /
    7-14                                                         \/
  ____________________________________________________________________
*/

    class CRMNounNeuterOrMaleAnimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_NEUTER_OR_MALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_NEUTER_OR_MALE_ANIMATE][1]; }
        String GetStaticProperties() const { return " animate=yes gender=neuterormale"; }
        int GetStaticPropData() const { return ESPropAnimateYes | ESPropGenderNeutralOrMale; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounNeuterConstInanimate                                 \  /
    7-15                                                         \/
  ____________________________________________________________________
*/

    class CRMNounNeuterConstInanimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_CONST_NEUTER_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_CONST_NEUTER_INANIMATE][1]; }
        String GetStaticProperties() const { return " animate=no gender=neuter"; }
        int GetStaticPropData() const { return ESPropAnimateNo | ESPropGenderNeutral; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNounNeuterOrMaleInimate                                  \  /
    7-16                                                         \/

  ____________________________________________________________________
*/

    class CRMNounNeuterOrMaleInimate : public Noun {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_NEUTER_OR_MALE_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_NEUTER_OR_MALE_INANIMATE][1]; }
        String GetStaticProperties() const { return " animate=no gender=neuterormale"; }
        int GetStaticPropData() const { return ESPropAnimateNo | ESPropGenderNeutralOrMale; }
    };

    /*____________________________________________________________________

																 /
CRMNounProperFemaleAnimate                                   \  /
7-17                                                          \/
____________________________________________________________________
*/

    class CRMNounProperFemaleAnimate : public CRMNounFemaleAnimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_PROPER_FEMALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_PROPER_FEMALE_ANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounFemaleAnimate::GetStaticProperties() + String(" proper=yes"); }
    };

    /*____________________________________________________________________

																 /
CRMNounHypocorFemaleAnimate                                  \  /
7-20                                                          \/
____________________________________________________________________
*/

    class CRMNounHypocorFemaleAnimate : public CRMNounProperFemaleAnimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_HYPOCOR_FEMALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_HYPOCOR_FEMALE_ANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounProperFemaleAnimate::GetStaticProperties() + String(" hypocor=yes"); }
    };

    /*____________________________________________________________________

																 /
CRMNounProperMaleAnimate									 \  /
7-18                                                          \/
____________________________________________________________________
*/

    class CRMNounProperMaleAnimate : public CRMNounMaleAnimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_PROPER_MALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_PROPER_MALE_ANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounMaleAnimate::GetStaticProperties() + String(" proper=yes"); }
    };

    /*____________________________________________________________________

																/
CRMNounHypocorMaleAnimate                                   \  /
7-21                                                         \/
____________________________________________________________________
*/

    class CRMNounHypocorMaleAnimate : public CRMNounProperMaleAnimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_HYPOCOR_MALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_HYPOCOR_MALE_ANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounProperMaleAnimate::GetStaticProperties() + String(" hypocor=yes"); }
    };

    /*____________________________________________________________________

																 /
CRMNounProperMaleFemaleAnimate								 \  /
7-19                                                          \/
____________________________________________________________________
*/

    class CRMNounProperMaleFemaleAnimate : public CRMNounMaleFemaleAnimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_PROPER_MALEFEMALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_PROPER_MALEFEMALE_ANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounMaleFemaleAnimate::GetStaticProperties() + String(" proper=yes"); }
    };

    /*____________________________________________________________________

																/
CRMNounHypocorMaleFemaleAnimate                             \  /
7-22                                                         \/
____________________________________________________________________
*/

    class CRMNounHypocorMaleFemaleAnimate : public CRMNounProperMaleFemaleAnimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_HYPOCOR_MALEFEMALE_ANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_HYPOCOR_MALEFEMALE_ANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounProperMaleFemaleAnimate::GetStaticProperties() + String(" hypocor=yes"); }
    };

    /*____________________________________________________________________

																 /
CRMNounProperFemaleInanimate                                 \  /
7-23                                                          \/
____________________________________________________________________
*/

    class CRMNounProperFemaleInanimate : public CRMNounFemaleInanimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_PROPER_FEMALE_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_PROPER_FEMALE_INANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounFemaleInanimate::GetStaticProperties() + String(" proper=yes"); }
    };

    /*____________________________________________________________________

																 /
CRMNounProperMaleInanimate									 \  /
7-24                                                          \/
____________________________________________________________________
*/

    class CRMNounProperMaleInanimate : public CRMNounMaleInanimate {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_PROPER_MALE_INANIMATE; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_PROPER_MALE_INANIMATE][1]; }
        String GetStaticProperties() const { return CRMNounMaleInanimate::GetStaticProperties() + String(" proper=yes"); }
    };

    /*____________________________________________________________________

																/
CRMNounProperSurname										\  /
7-25                                                         \/
____________________________________________________________________
*/

    class CRMNounProperSurname : public CRMLexem {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_NOUN_PROPER_SURNAME; }
        String GetSynClassName() const { return ERMClassNames[ERM_NOUN_PROPER_SURNAME][1]; }
        String GetStaticProperties() const { return String(" proper=yes animate=yes"); }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            CRMHomonim homonim;
            String props = "<noun";

            if (Paradigm->UID == cConstantParadigmID) props += " gender=malefemale";
            return ProcessFlex(FlexPos, props + GetStaticProperties() + GetDerivedProperties(), cProperSurname, &homonim, Paradigm, StressSchema);
        }

        static const String cConstantParadigmID;

        static Strings cProperSurname;
    };

    /*____________________________________________________________________


                                                                    /
    CRMAdjective                                                \  /
    8-1                                                          \/
  ____________________________________________________________________
*/

    class CRMAdjective : public CRMLexem {
    public:
        class AdjectiveHomonim : public CRMHomonim {
        public:
            void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm) {
                // GIG : 30.10.2012 - Properties added to Data instead of "<adjective"
                String form = Properties;
                switch (FlexPosition) {
                    case 0:
                        Data.Add(form + " animate=no gender=male number=single case=accusative");
                        break;
                    case 1:
                        Data.Add(form + " animate=yes gender=male number=single case=accusative");
                        break;
                    case 5:
                        Data.Add(form + " animate=any gender=neuter number=single case=accusative");
                        break;
                    case 12:
                        Data.Add(form + " animate=any gender=female number=single case=prepositional");
                        break;
                    case 15:
                        Data.Add(form + " animate=no gender=any number=plural case=accusative");
                        break;
                    case 16:
                        Data.Add(form + " animate=yes  gender=any number=plural case=accusative");
                        break;
                }
            }
            int StressDelta() { return _HackOn ? 2 : 0; }
            static void WeakDegreeHack(bool On) {
                static String WeakDegree(" form=comparative degree=weak");
                static String StrongDegree(" form=comparative degree=strong");
                _HackOn = On;
                if (On)
                    cAdjective[CRMParadigm::COMPARATIVE_ADJ_FLEX_POSITION] = WeakDegree;
                else
                    cAdjective[CRMParadigm::COMPARATIVE_ADJ_FLEX_POSITION] = StrongDegree;
            }

        private:
            static bool _HackOn;
        };
        ERMMorphoClass GetSynClass() const { return ERM_ADJECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_ADJECTIVE][1]; }

        // GIG 07.06.2015 : static adjectives (like кечуа)
        virtual String GetProperties() const {
            String retval = CRMLexem::GetProperties();
            if (Paradigm->UID == "Cl_0")
                retval += " animate=any gender=any ";
            return retval;
        }

        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            AdjectiveHomonim homonim;
            return ProcessFlex(FlexPos, GetProperties(), cAdjective, &homonim, Paradigm, StressSchema);
        }
        static Strings cAdjective;
    };

    /*____________________________________________________________________

                                                                    /
    CRMPronounAdjective class definition                        \  /
    8-2                                                          \/
  ____________________________________________________________________
*/

    class CRMPronounAdjective : public CRMLexem {
    public:
        class PronounAdjectiveHomonim : public CRMHomonim {
            void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm) {
                String props;
                switch (FlexPosition) {
                    case 0:
                        props = " animate=no gender=male number=single case=accusative";
                        break;
                    case 1:
                        props = " animate=yes gender=male number=single case=accusative";
                        break;
                    case 5:
                        props = " animate=any gender=neuter number=single case=accusative";
                        break;
                    case 12:
                        props = " animate=any gender=female number=single case=prepositional";
                        break;
                    case 15:
                        props = " animate=no gender=any number=plural case=accusative";
                        break;
                    case 16:
                        props = " animate=yes gender=any number=plural case=accusative";
                }
                if (!props.IsEmpty()) {
                    Data.Add(Properties + props);
                }
            }
        };
        ERMMorphoClass GetSynClass() const { return ERM_PRONOUN_ADJECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_PRONOUN_ADJECTIVE][1]; }
        // GIG: 29.03.2013
        //String        GetStaticProperties () const { return " animate=any"; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            PronounAdjectiveHomonim homonim;
            // GIG: 29.03.2013
            // redesign of homonimes processing
            //return ProcessFlex ( FlexPos, "<" + GetSynClassName () + GetStaticProperties () + GetDerivedProperties (), Number::cPronounAdjective, &homonim, Paradigm, StressSchema );
            return ProcessFlex(FlexPos, GetProperties(), Number::cPronounAdjective, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMNumberOrdinal class definition                           \  /
    8-3                                                          \/
  ____________________________________________________________________
*/

    class CRMNumberOrdinal : public CRMLexem {
    public:
        class NumberOrdinalHomonim : public CRMHomonim {
            void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm) {
                String props;
                switch (FlexPosition) {
                    case 0:
                        props = " animate=no gender=male number=single case=accusative";
                        break;
                    case 1:
                        props = " animate=yes gender=male number=single case=accusative";
                        break;
                    case 5:
                        props = " animate=any gender=neuter number=single case=accusative";
                        break;
                    case 12:
                        props = " animate=any gender=female number=single case=prepositional";
                        break;
                    case 15:
                        props = " animate=no gender=any number=plural case=accusative";
                        break;
                    case 16:
                        props = " animate=yes gender=any number=plural case=accusative";
                }
                if (!props.IsEmpty())
                    Data.Add(Properties + props);
            }
        };
        ERMMorphoClass GetSynClass() const { return ERM_NUMBER_ORDINAL; }
        String GetSynClassName() const { return ERMClassNames[ERM_NUMBER_ORDINAL][1]; }
        // GIG: 29.03.2013
        //String        GetStaticProperties () const { return " animate=any"; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            NumberOrdinalHomonim homonim;
            //return ProcessFlex ( FlexPos, "<" + GetSynClassName () + GetStaticProperties () + GetDerivedProperties (), Number::cPronounAdjective, &homonim, Paradigm, StressSchema );
            return ProcessFlex(FlexPos, GetProperties(), Number::cPronounAdjective, &homonim, Paradigm, StressSchema);
        }
    };

    /*____________________________________________________________________

                                                                    /
    CRMPossesiveAdjective class definition                      \  /
    8-5                                                          \/
  ____________________________________________________________________
*/

    class CRMPossesiveAdjective : public CRMLexem {
    public:
        class PossesiveAdjectiveHomonim : public CRMHomonim {
            void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm) {
                String props;
                switch (FlexPosition) {
                    case 0:
                        props = " animate=no gender=male number=single case=accusative";
                        break;
                    case 1:
                        props = " animate=yes gender=male number=single case=accusative";
                        break;
                    case 5:
                        props = " animate=any gender=neuter number=single case=accusative";
                        break;
                    case 12:
                        props = " animate=any gender=female number=single case=prepositional";
                        break;
                    case 15:
                        props = " animate=no gender=any number=plural case=accusative";
                        break;
                    case 16:
                        props = " animate=yes gender=any number=plural case=accusative";
                }
                if (!props.IsEmpty())
                    Data.Add(Properties + props);
            }
        };
        ERMMorphoClass GetSynClass() const { return ERM_POSSESIVE_ADJECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_POSSESIVE_ADJECTIVE][1]; }
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            PossesiveAdjectiveHomonim homonim;
            // GIG: 29.03.2013
            return ProcessFlex(FlexPos, GetProperties(), cPossesiveAdjective, &homonim, Paradigm, StressSchema);
        }

        static Strings cPossesiveAdjective;
    };

    /*____________________________________________________________________

                                                                    /
    Verb                                                        \  /
    9                                                            \/
  ____________________________________________________________________
*/

    class Verb : public CRMLexem {
    public:
        class VerbHomonim : public CRMHomonim {
            void Identify(Strings& Data, unsigned long FlexPosition, const String& Properties, const CRMParadigm* const Paradigm) {
                // VIA:GIG: specific processing only for participles
                // Participle criterion: FlexPosition >= 512
                if (FlexPosition < 512) return;
                // process participles
                FlexPosition = (FlexPosition >> 9) - 1;
                switch (FlexPosition) {
                    case 0:
                        Data.Add(Data[0] + " animate=no gender=male number=single case=accusative");
                        break;
                    case 1:
                        Data.Add(Data[0] + " animate=yes gender=male number=single case=accusative");
                        break;
                    case 5:
                        Data.Add(Data[0] + " animate=any gender=neuter number=single case=accusative");
                        break;
                    case 12:
                        Data.Add(Data[0] + " animate=any gender=female number=single case=prepositional");
                        break;
                    case 15:
                        Data.Add(Data[0] + " animate=no gender=any number=plural case=accusative");
                        break;
                    case 16:
                        Data.Add(Data[0] + " animate=yes gender=any number=plural case=accusative");
                        break;
                }
                    // GIG:
#ifdef _DEBUG
                if (FlexPosition >= sizeof(ERMParticiple) / sizeof(ERMParticiple[0]))
                    throw new Xception(String("Illegal flex position:") + String::FromInt(FlexPosition),
                                       String("ProcessFlex"), String("CRMLexeme"), String("RMStock.cpp"));
#endif

                Data[0] += ERMParticiple[FlexPosition];
            }
        };
        virtual String GetStaticProperties() const = 0;
        virtual Strings GetFlexData() const = 0;
        Strings IdentifyFlex(CRMParadigm::FlexVar& FlexPos, const String& Form) const {
            VerbHomonim homonim;
            return ProcessFlex(FlexPos, GetStaticProperties() + GetDerivedProperties(), GetFlexData(), &homonim, Paradigm, StressSchema);
        }
        // 9-1
        static String cIrreflexiveIntransitiveImperfectiveSign;
        static Strings cIrreflexiveIntransitiveImperfective;
        // 9-2
        static String cIrreflexiveIntransitivePerfectiveSign;
        static Strings cIrreflexiveIntransitivePerfective;
        // 9-3
        static String cIrreflexiveTransitiveImperfectiveSign;
        static Strings cIrreflexiveTransitiveImperfective;
        // 9-4
        static String cIrreflexiveTransitivePerfectiveSign;
        static Strings cIrreflexiveTransitivePerfective;

        // 9-5
        static String cReflexiveImperfectiveSign;
        static Strings cReflexiveImperfective;
        // 9-6
        static String cReflexivePerfectiveSign;
        static Strings cReflexivePerfective;
        // 9-7
        static String cFrequentativeIrreflexiveIntransitiveImperfectiveSign;
        static Strings cFrequentativeIrreflexiveIntransitiveImperfective;
        // 9-8
        static String cFrequentativeIrreflexiveTransitiveImperfectiveSign;
        static Strings cFrequentativeIrreflexiveTransitiveImperfective;
        // 9-9
        static String cImpersonalIrreflexiveIntransitiveImperfectiveSign;
        static Strings cImpersonalIrreflexiveIntransitiveImperfective;
        // 9-10
        static String cImpersonalIrreflexiveIntransitivePerfectiveSign;
        static Strings cImpersonalIrreflexiveIntransitivePerfective;
        // 9-11
        static String cImpersonalIrreflexiveTransitiveImperfectiveSign;
        static Strings cImpersonalIrreflexiveTransitiveImperfective;
        // 9-12
        static String cImpersonalIrreflexiveTransitivePerfectiveSign;
        static Strings cImpersonalIrreflexiveTransitivePerfective;
        // 9-13
        static String cImpersonalReflexiveIntransitiveImperfectiveSign;
        static Strings cImpersonalReflexiveIntransitiveImperfective;
        // 9-14
        static String cImpersonalReflexiveIntransitivePerfectiveSign;
        static Strings cImpersonalReflexiveIntransitivePerfective;
        // 9-15
        static String cIrreflexiveIntransitiveBiformSign;
        static Strings cIrreflexiveIntransitiveBiform;
        // 9-16
        static String cIrreflexiveTransitiveBiformSign;
        static Strings cIrreflexiveTransitiveBiform;
        // 9-17
        static String cReflexiveIntransitiveBiformSign;
        static Strings cReflexiveIntransitiveBiform;
        // 9-18
        static String cReflexiveIntransitiveBiformExtSign;
        static Strings cReflexiveIntransitiveBiformExt;
        // 9-19
        static String cBeIrreflexiveIntransitiveSign;
        static Strings cBeIrreflexiveIntransitive;
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbIrreflexiveIntransitiveImperfective class definition \  /
    9-1                                                          \/
  ____________________________________________________________________
*/

    class CRMVerbIrreflexiveIntransitiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cIrreflexiveIntransitiveImperfectiveSign; };
        Strings GetFlexData() const { return cIrreflexiveIntransitiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbIrreflexiveIntransitivePerfective class definition   \  /
    9-2                                                          \/
  ____________________________________________________________________
*/

    class CRMVerbIrreflexiveIntransitivePerfective : public Verb {
    public:
        String GetStaticProperties() const { return cIrreflexiveIntransitivePerfectiveSign; };
        Strings GetFlexData() const { return cIrreflexiveIntransitivePerfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbIrreflexiveTransitiveImperfective class definition   \  /
    9-3                                                          \/
  ____________________________________________________________________
*/

    class CRMVerbIrreflexiveTransitiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cIrreflexiveTransitiveImperfectiveSign; };
        Strings GetFlexData() const { return cIrreflexiveTransitiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbIrreflexiveTransitivePerfective class definition     \  /
    9-4                                                          \/
  ____________________________________________________________________
*/

    class CRMVerbIrreflexiveTransitivePerfective : public Verb {
    public:
        String GetStaticProperties() const { return cIrreflexiveTransitivePerfectiveSign; };
        Strings GetFlexData() const { return cIrreflexiveTransitivePerfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IRREFLEXIVE_TRANSITIVE_PERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IRREFLEXIVE_TRANSITIVE_PERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbReflexiveImperfective class definition               \  /
    9-5                                                          \/
  ____________________________________________________________________
*/

    class CRMVerbReflexiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cReflexiveImperfectiveSign; };
        Strings GetFlexData() const { return cReflexiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_REFLEXIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_REFLEXIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbReflexivePerfective class definition                 \  /
    9-6                                                          \/
  ____________________________________________________________________
*/

    class CRMVerbReflexivePerfective : public Verb {
    public:
        String GetStaticProperties() const { return cReflexivePerfectiveSign; };
        Strings GetFlexData() const { return cReflexivePerfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_REFLEXIVE_PERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_REFLEXIVE_PERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbFrequentativeIrreflexiveIntransitiveImperfective     \  /
    class definition                                             \/
    9-7
  ____________________________________________________________________
*/

    class CRMVerbFrequentativeIrreflexiveIntransitiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cFrequentativeIrreflexiveIntransitiveImperfectiveSign; };
        Strings GetFlexData() const { return cFrequentativeIrreflexiveIntransitiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbFrequentativeIrreflexiveTransitiveImperfective       \  /
    class definition                                             \/
    9-8
  ____________________________________________________________________
*/

    class CRMVerbFrequentativeIrreflexiveTransitiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cFrequentativeIrreflexiveTransitiveImperfectiveSign; };
        Strings GetFlexData() const { return cFrequentativeIrreflexiveTransitiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbImpersonalIrreflexiveIntransitiveImperfective        \  /
    class definition                                             \/
    9-9 
  ____________________________________________________________________
*/

    class CRMVerbImpersonalIrreflexiveIntransitiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cImpersonalIrreflexiveIntransitiveImperfectiveSign; };
        Strings GetFlexData() const { return cImpersonalIrreflexiveIntransitiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbImpersonalIrreflexiveIntransitivePerfective          \  /
    class definition                                             \/
    9-10
  ____________________________________________________________________
*/

    class CRMVerbImpersonalIrreflexiveIntransitivePerfective : public Verb {
    public:
        String GetStaticProperties() const { return cImpersonalIrreflexiveIntransitivePerfectiveSign; };
        Strings GetFlexData() const { return cImpersonalIrreflexiveIntransitivePerfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbImpersonalIrreflexiveTransitiveImperfective          \  /
    class definition                                             \/
    9-11
  ____________________________________________________________________
*/

    class CRMVerbImpersonalIrreflexiveTransitiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cImpersonalIrreflexiveTransitiveImperfectiveSign; };
        Strings GetFlexData() const { return cImpersonalIrreflexiveTransitiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbImpersonalIrreflexiveTransitivePerfective            \  /
    class definition                                             \/
    9-12
  ____________________________________________________________________
*/

    class CRMVerbImpersonalIrreflexiveTransitivePerfective : public Verb {
    public:
        String GetStaticProperties() const { return cImpersonalIrreflexiveTransitivePerfectiveSign; };
        Strings GetFlexData() const { return cImpersonalIrreflexiveTransitivePerfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_PERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_PERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbImpersonalReflexiveIntransitiveImperfective          \  /
    class definition                                             \/
    9-13
  ____________________________________________________________________
*/

    class CRMVerbImpersonalReflexiveIntransitiveImperfective : public Verb {
    public:
        String GetStaticProperties() const { return cImpersonalReflexiveIntransitiveImperfectiveSign; };
        Strings GetFlexData() const { return cImpersonalReflexiveIntransitiveImperfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_IMPERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_IMPERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbImpersonalReflexiveIntransitivePerfective            \  /
    class definition                                             \/
    9-14
  ____________________________________________________________________
*/

    class CRMVerbImpersonalReflexiveIntransitivePerfective : public Verb {
    public:
        String GetStaticProperties() const { return cImpersonalReflexiveIntransitivePerfectiveSign; };
        Strings GetFlexData() const { return cImpersonalReflexiveIntransitivePerfective; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_PERFECTIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_PERFECTIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbIrreflexiveIntransitiveBiform                        \  /
    class definition                                             \/
    9-15
  ____________________________________________________________________
*/

    class CRMVerbIrreflexiveIntransitiveBiform : public Verb {
    public:
        String GetStaticProperties() const { return cIrreflexiveIntransitiveBiformSign; };
        Strings GetFlexData() const { return cIrreflexiveIntransitiveBiform; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IRREFLEXIVE_INTRANSITIVE_BIFORM; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IRREFLEXIVE_INTRANSITIVE_BIFORM][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbIrreflexiveTransitiveBiform                          \  /
    class definition                                             \/
    9-16
  ____________________________________________________________________
*/

    class CRMVerbIrreflexiveTransitiveBiform : public Verb {
    public:
        String GetStaticProperties() const { return cIrreflexiveTransitiveBiformSign; };
        Strings GetFlexData() const { return cIrreflexiveTransitiveBiform; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_IRREFLEXIVE_TRANSITIVE_BIFORM; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_IRREFLEXIVE_TRANSITIVE_BIFORM][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbReflexiveIntransitiveBiform                          \  /
    class definition                                             \/
    9-17
  ____________________________________________________________________
*/

    class CRMVerbReflexiveIntransitiveBiform : public Verb {
    public:
        String GetStaticProperties() const { return cReflexiveIntransitiveBiformSign; };
        Strings GetFlexData() const { return cReflexiveIntransitiveBiform; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbReflexiveIntransitiveBiformExt                       \  /
    class definition                                             \/
    9-18
  ____________________________________________________________________
*/

    class CRMVerbReflexiveIntransitiveBiformExt : public Verb {
    public:
        String GetStaticProperties() const { return cReflexiveIntransitiveBiformExtSign; };
        Strings GetFlexData() const { return cReflexiveIntransitiveBiformExt; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM_EXT; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM_EXT][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMVerbBeIrreflexiveIntransitive                            \  /
    class definition                                             \/
    9-19
  ____________________________________________________________________
*/

    class CRMVerbBeIrreflexiveIntransitive : public Verb {
    public:
        String GetStaticProperties() const { return cBeIrreflexiveIntransitiveSign; };
        Strings GetFlexData() const { return cBeIrreflexiveIntransitive; };
        ERMMorphoClass GetSynClass() const { return ERM_VERB_BE_IRREFLEXIVE_INTRANSITIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_VERB_BE_IRREFLEXIVE_INTRANSITIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMPreposition class definition                             \  /
    10                                                           \/
  ____________________________________________________________________
*/

    class CRMPreposition : public Static {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_PREPOSITION; }
        String GetSynClassName() const { return ERMClassNames[ERM_PREPOSITION][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMConjuction class definition                              \  /
    11                                                           \/
  ____________________________________________________________________
*/

    class CRMConjunction : public Static {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_CONJUNCTION; }
        String GetSynClassName() const { return ERMClassNames[ERM_CONJUNCTION][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMAdverb class definition                                  \  /
    12                                                           \/
  ____________________________________________________________________
*/

    class CRMAdverb : public Static {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_ADVERB; }
        String GetSynClassName() const { return ERMClassNames[ERM_ADVERB][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMPredicative class definition                             \  /
    13                                                           \/
  ____________________________________________________________________
*/

    class CRMPredicative : public Static {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_PREDICATIVE; }
        String GetSynClassName() const { return ERMClassNames[ERM_PREDICATIVE][1]; }
    };

    /*____________________________________________________________________

                                                                    /
    CRMParenthesis class definition                             \  /
    14                                                           \/
  ____________________________________________________________________
*/

    class CRMParenthesis : public Static {
    public:
        ERMMorphoClass GetSynClass() const { return ERM_PARENTHESIS; }
        String GetSynClassName() const { return ERMClassNames[ERM_PARENTHESIS][1]; }
    };
};

class CRMLexems : public CSCAbstractPtrArray<CRMLexem> {};
class CRMPClasses : public CSCPtrArray<CRMParadigm> {};

#endif
