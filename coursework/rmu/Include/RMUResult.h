#ifndef __RMURESULT_H__
#define __RMURESULT_H__

#include <string>
#include <vector>

//#include "RMUDLL.h"

#ifndef WITH_PHRASES
#define WITH_PHRASES
#endif

struct RMUResult;

#ifdef WITH_PHRASES
extern const char* S_clStr[];
extern const char* AnimateStr[];
extern const char* GenderStr[];
extern const char* NumberStr[];
extern const char* CaseStr[];
extern const char* ReflectionStr[];
extern const char* PerfectiveStr[];
extern const char* TransitiveStr[];
extern const char* PersonStr[];
extern const char* TenseStr[];
extern const char* VoiceStr[];
extern const char* DegreeStr[];
#endif

void RMUResultGet(const std::string& answer, std::vector<RMUResult>& result);  // throw(RMUError)

//	Return character representation (read-only) of the morphosyntax class of the result
const char* RMUGetSyntClassString(const RMUResult* pResult);

////////////////////////////////////////////////////////////////////////////////////////////
//
//		RMUFindSyntClassId
//	Helper function for grammar table parser
//	Return value - number of syntax class with given name (in Win-1251 encoding)
//	Currently English and Russian nclass names are supported
int RMUFindSyntClassId(const char* className);

const int Undefined = 0;

enum Es_cl { noun = 1,
             personalpronoun,
             reflexivepronoun,
             pronoun,
             propername,
             adjective,
             possesiveadjective,
             pronounadjective,
             numberordinal,
             participle,
             shortadjective,
             shortparticiple,
             comparative,
             verb,
             unpersonalverb,
             frequentativeverb,
             gerund,
             numberone,
             numbertwo,
             numberthree,
             number,
             numberbiform,
             adverb,
             preposition,
             conjunction,
             predicative,
             particle,
             interjection,
             parenthesis,
             acronym,
             comma,
             quotation,
             dash,
             point,
             S_clAny
};

enum Eanimate { animate = 1,
                unanimate,
                animateAny };

enum Egender { male = 1,
               female,
               neuter,
               malefemale,
               maleorfemale,
               genderAny };

enum Enumber { single = 1,
               plural,
               numberAny };

enum Ecase { nominative = 1,
             genitive,
             dative,
             accusative,
             instrumental,
             prepositional,
             caseAny };

enum Ereflection { reflexive = 1,
                   unreflexive,
                   reflexiveForm,
                   reflectionAny };

enum Eperfective { perfective = 1,
                   unperfective,
                   perfectiveAny };

enum Etransitive { transitive = 1,
                   untransitive,
                   transitiveAny };

enum Eperson { personAny = 4 };

enum Etense { infinitive = 1,
              present,
              past,
              future,
              imperative,
              tenseAny };

enum Evoice { active = 1,
              passive,
              voiceAny };

enum Edegree { strong = 1,
               weak,
               degreeAny };

struct RMUResult {
    std::string lexem;
    std::string P_cl;
    std::string schema;
    std::string uid;

    int S_cl;
    int Animate;
    int Gender;
    int Number;
    int Case;
    int Reflection;
    int Perfective;
    int Transitive;
    int Person;
    int Tense;
    int Voice;
    int Degree;
    bool Static;
    std::string Stress;
    bool ProperName;

    // phrase-specific
#ifdef WITH_PHRASES
    RMUResult* nextWord;
    bool mainWord;
    int Phrase_cl;
    int innerPhrase_cl;
#endif
    RMUResult() {
        S_cl = 0;
        Animate = 0;
        Gender = 0;
        Number = 0;
        Case = 0;
        Reflection = 0;
        Perfective = 0;
        Transitive = 0;
        Person = 0;
        Tense = 0;
        Voice = 0;
        Degree = 0;
        Static = false;
        // Stress = 0;
        ProperName = false;

        // phrase-specific
#ifdef WITH_PHRASES
        mainWord = false;
        nextWord = NULL;
        Phrase_cl = 0;
        innerPhrase_cl = 0;
#endif
    }

    std::string GetTranslatedAnswer(bool shortForm = false) const;
};

#endif