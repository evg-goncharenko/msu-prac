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
  Comments  : Russian morphology model is copyrighted by Volkova Irina
  ____________________________________________________________________
*/

#ifndef RMSTOCK_DATA_H_2001
#define RMSTOCK_DATA_H_2001

// Morpho-syntactic classes
enum ERMMorphoClass
{
    ERM_PARTICLE,                                                   // частица                                                                (1)
    ERM_INTERJECTION,                                               // междометие                                                             (2)
    ERM_PERSONAL_PRONOUN,                                           // личное местоимение                                                     (3-1)
    ERM_REFLEXIVE_PRONOUN,                                          // возвратное местоимение                                                 (3-2)
    ERM_PRONOUN,                                                    // местоимение                                                            (3-3)
    ERM_NUMBER_ONE,                                                 // числительное один                                                      (4-1)
    ERM_NUMBER_TWO,                                                 // числительное два, оба, полтора                                         (4-2)
    ERM_NUMBER_THREE,                                               // числительное три, четыре сколько, несколько, столько, много, немного и собирательные числительные
    ERM_NUMBER,                                                     // количественное числительное                                            (4-4)
    ERM_NUMBER_BIFORM,                                              // числительное с двумя формами                                           (4-5)
    ERM_NOUN_MALE_INANIMATE,                                        // существительное неодушевленное мужского рода                           (7-1)
    ERM_NOUN_MALE_ANIMATE,                                          // существительное одушевленное мужского рода                             (7-2)
    ERM_NOUN_FEMALE_INANIMATE,                                      // существительное неодушевленное женского рода                           (7-3)
    ERM_NOUN_FEMALE_ANIMATE,                                        // существительное одушевленное женского рода                             (7-4)
    ERM_NOUN_NEUTER_INANIMATE,                                      // существительное неодушевленное среднего рода                           (7-5)
    ERM_NOUN_NEUTER_ANIMATE,                                        // существительное одушевленное среднего рода                             (7-6)
    ERM_NOUN_MALEFEMALE_ANIMATE,                                    // существительное одушевленное обобщённого рода (мо/жо)                  (7-7)
    ERM_NOUN_MALE,                                                  // существительное мужского рода (од/неод)                                (7-8)
    ERM_NOUN_MALE_OR_FEMALE_INANIMATE,                              // существительное неодушевленное мужского/женского рода                  (7-9) 
    ERM_NOUN_MALE_OR_FEMALE_ANIMATE,                                // существительное одушевленное мужского/женского рода                    (7-10)
    ERM_NOUN_FEMALE,                                                // существительное мужского рода (од/неод)                                (7-11)
    ERM_NOUN_NEUTER_OR_FEMALE_INANIMATE,                            // существительное неодушевленное среднего/женского рода                  (7-12) 
    ERM_NOUN_NEUTER,                                                // существительное среднего рода (од/неод)                                (7-13)
    ERM_NOUN_NEUTER_OR_MALE_ANIMATE,                                // существительное одушевленное среднего/мужского рода                    (7-14)
    ERM_NOUN_CONST_NEUTER_INANIMATE,                                // существительное неизменяемое неодушевленное среднего рода              (7-15) 
    ERM_NOUN_NEUTER_OR_MALE_INANIMATE,                              // существительное неодушевленное среднего/мужского рода                  (7-16)
	// added proper nouns - GIG - 22/06/2017
	ERM_NOUN_PROPER_FEMALE_ANIMATE,                                 // существительное собственное одушевленное женского рода                 (7-17)
	ERM_NOUN_PROPER_MALE_ANIMATE,                                   // существительное собственное одушевленное мужского рода                 (7-18)
	ERM_NOUN_PROPER_MALEFEMALE_ANIMATE,                             // существительное собственное одушевленное обобщённого рода (мо/жо)      (7-19)
	ERM_NOUN_HYPOCOR_FEMALE_ANIMATE,                                // существительное гипокористич. одушевленное женского рода               (7-20)
	ERM_NOUN_HYPOCOR_MALE_ANIMATE,                                  // существительное гипокористич. одушевленное мужского рода               (7-21)
	ERM_NOUN_HYPOCOR_MALEFEMALE_ANIMATE,                            // существительное гипокористич. одушевленное обобщённого рода (мо/жо)    (7-22)
	ERM_NOUN_PROPER_FEMALE_INANIMATE,                               // существительное собственное неодушевленное женского рода               (7-23)
	ERM_NOUN_PROPER_MALE_INANIMATE,                                 // существительное собственное неодушевленное мужского рода               (7-24)
	ERM_NOUN_PROPER_SURNAME,										// существительное-фамилия (собственное одушевленное м/ж рода)			  (7-25)
	// end - GIG - 22/06/2017
	ERM_ADJECTIVE,                                                  // прилагательное                                                         (8-1)
    ERM_PRONOUN_ADJECTIVE,                                          // местоименное прилагательное                                            (8-2)
    ERM_NUMBER_ORDINAL,                                             // порядковое числительное                                                (8-3)
    ERM_PARTICIPLE,                                                 // причастие                                                              (8-4)
    ERM_POSSESIVE_ADJECTIVE,                                        // притяжательное прилагательное                                          (8-5)
    ERM_VERB_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE,                 // глагол невозвратный непереходный несовершенного вида (не многократный и не безличный)  (9-1)
    ERM_VERB_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE,                   // глагол невозвратный непереходный совершенного вида (не многократный и не безличный)    (9-2)
    ERM_VERB_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE,                   // глагол невозвратный переходный несовершенного вида (не многократный и не безличный)    (9-3)
    ERM_VERB_IRREFLEXIVE_TRANSITIVE_PERFECTIVE,                     // глагол невозвратный переходный совершенного вида (не многократный и не безличный)      (9-4)
    ERM_VERB_REFLEXIVE_IMPERFECTIVE,                                // глагол возвратный непереходный несовершенного вида (не многократный и не безличный)    (9-5)
    ERM_VERB_REFLEXIVE_PERFECTIVE,                                  // глагол возвратный непереходный совершенного вида (не многократный и не безличный)      (9-6)
    ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE,   // глагол многократный невозвратный непереходный несовершенного вида      (9-7)
    ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE,     // глагол многократный невозвратный переходный несовершенного вида        (9-8)
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE,      // глагол безличный невозвратный непереходный несовершенного вида         (9-9)
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE,        // глагол безличный невозвратный непереходный совершенного вида           (9-10)
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE,        // глагол безличный невозвратный переходный несовершенного вида
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_PERFECTIVE,          // глагол безличный невозвратный переходный совершенного вида
    ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_IMPERFECTIVE,        // глагол безличный возвратный непереходный несовершенного вида
    ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_PERFECTIVE,          // глагол безличный возвратный непереходный совершенного вида    
    ERM_VERB_IRREFLEXIVE_INTRANSITIVE_BIFORM,                       // глагол невозвратный непереходный двувидовый                            (9-15)
    ERM_VERB_IRREFLEXIVE_TRANSITIVE_BIFORM,                         // глагол невозвратный переходный двувидовый                              (9-16)
    ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM,                         // глагол возвратный непереходный двувидовый                              (9-17)
    ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM_EXT,                     // глагол возвратный непереходный двувидовый (только в наст/буд)          (9-18)
    ERM_VERB_BE_IRREFLEXIVE_INTRANSITIVE,                           // глагол "быть" невозвратный непереходный                                (9-19)
    ERM_PREPOSITION,                                                // предлог                                                                (10)
    ERM_CONJUNCTION,                                                // союз                                                                   (11)
    ERM_ADVERB,                                                     // наречие                                                                (12)
    ERM_PREDICATIVE,                                                // предикатив                                                             (13)
    ERM_PARENTHESIS,                                                // вводное слово                                                          (14)
    //ERM_ACRONYM,                                                    // сокращение
	// add new morpho-syntactical class constants here
	ERM_LAST_CLASS_NUMBER											// последний номер класса (не используется)
	// do not add any number here!!!
};

const char ERMClassNames[ERM_LAST_CLASS_NUMBER][2][24] = 
{
    { "1",    "particle" },
    { "2",    "interjection" },
    { "3_1",  "personalpronoun" },
    { "3_2",  "reflexivepronoun" },
    { "3_3",  "pronoun" },
    { "4_1",  "numberone" },
    { "4_2",  "numbertwo" },
    { "4_3",  "numberthree" },
    { "4_4",  "number" },
    { "4_5",  "numberbiform" },
    { "7_1",  "noun_m_i" },
    { "7_2",  "noun_m_a" },
    { "7_3",  "noun_f_i" },
    { "7_4",  "noun_f_a" },
    { "7_5",  "noun_n_i" },
    { "7_6",  "noun_n_a" },
    { "7_7",  "noun_mf_a" },
    { "7_8",  "noun_m" },
    { "7_9",  "noun_m_f_i" },
    { "7_10", "noun_m_f_a" },
    { "7_11", "noun_f" },
    { "7_12", "noun_n_f_i" },
    { "7_13", "noun_n" },
    { "7_14", "noun_n_m_a" },
    { "7_15", "noun_n_c_i" },
    { "7_16", "noun_n_m_i" },
	// added for proper nouns - GIG - 22/06/2017
	{ "7_17", "noun_p_f_a"}, 
	{ "7_18", "noun_p_m_a" },
	{ "7_19", "noun_p_mf_a" },
	{ "7_20", "noun_ph_f_a" },
	{ "7_21", "noun_ph_m_a" },
	{ "7_22", "noun_ph_mf_a" },
	{ "7_23", "noun_p_f_i" },
	{ "7_24", "noun_p_m_i" },
	{ "7_25", "noun_sur" },
	// end - GIG - 22/06/2017
    { "8_1",  "adjective" },
    { "8_2",  "pronounadjective" },
    { "8_3",  "numberordinal" },
    { "8_4",  "participle" },
    { "8_5",  "possesiveadjective" },
    { "9_1",  "v_i_i_i" },
    { "9_2",  "v_i_i_p" },
    { "9_3",  "v_i_t_i" },
    { "9_4",  "v_i_t_p" },
    { "9_5",  "v_r_i" },
    { "9_6",  "v_r_p" },
    { "9_7",  "v_f_i_i_i" },
    { "9_8",  "v_f_i_t_i" },
    { "9_9",  "v_i_i_i_i" },
    { "9_10", "v_i_i_i_p" },
    { "9_11", "v_i_i_t_i" },
    { "9_12", "v_i_i_t_p" },
    { "9_13", "v_i_r_i_i" },
    { "9_14", "v_i_r_i_p" },
    { "9_15", "v_i_i_b" },
    { "9_16", "v_i_t_b" },
    { "9_17", "v_r_i_b" },
    { "9_18", "v_r_i_b_e" },
    { "9_19", "v_b_i_i" },
    { "10",   "preposition" },
    { "11",   "conjunction" },
    { "12",   "adverb" },
    { "13",   "predicative" },
    { "14",   "parenthesis" },
};

// Cases
enum ERMCasesNumbers
{
    ERMNominativeCase,
    ERMGenitiveCase,
    ERMDativeCase,
    ERMAccusativeCase,
    ERMInstrumentalCase,
    ERMPrepositionalCase
};

// Capitalization
enum ERMCapitalization
{
    ERM_Normal,
    ERM_First,
    ERM_All
};
const char ERMCaps[3][8] =
{
    "normal",
    "first",
    "all"
};

// Animation
const char ERMAnimates[3][4] =
{
    "yes",
    "no",
    "any"
};

// Gender
const char ERMGenders[4][8] =
{
    "male",
    "female",
    "neuter",
    "any"
};

// Number
const char ERMNumbers[3][8] =
{
    "single",
    "plural",
    "any"
};

// Person
const char ERMPersons[4][4] =
{
    "1",
    "2",
    "3",
    "any"
};

// Paradigm item types
const int ERMPFlex      = 0;
const int ERMPVariation = 1;
const int ERMPFlexes    = 2;

const char ERMNumberBiFormCase[2][14] = { "nominative", "accusative" };

const char ERMNumberCase[6][14] =
{
    "nominative", "genitive", "dative", "accusative", "instrumental", "prepositional" 
};

const char ERMParticiple[24][128] =
{
    " animate=\"any\" gender=\"male\" case=\"nominative\" number=\"single\"",
    " animate=\"any\" gender=\"male\" case=\"genitive\" number=\"single\"",
    " animate=\"any\" gender=\"male\" case=\"dative\" number=\"single\"",
    " animate=\"any\" gender=\"male\" case=\"instrumental\" number=\"single\"",
    " animate=\"any\" gender=\"male\" case=\"prepositional\" number=\"single\"",
    " animate=\"any\" gender=\"neuter\" case=\"nominative\" number=\"single\"",
    " animate=\"any\" gender=\"neuter\" case=\"genitive\" number=\"single\"",
    " animate=\"any\" gender=\"neuter\" case=\"dative\" number=\"single\"",
    " animate=\"any\" gender=\"neuter\" case=\"instrumental\" number=\"single\"",
    " animate=\"any\" gender=\"neuter\" case=\"prepositional\" number=\"single\"",
    " animate=\"any\" gender=\"female\" case=\"nominative\" number=\"single\"",
    " animate=\"any\" gender=\"female\" case=\"genitive\" number=\"single\"",
    " animate=\"any\" gender=\"female\" case=\"dative\" number=\"single\"",
    " animate=\"any\" gender=\"female\" case=\"accusative\" number=\"single\"",
    " animate=\"any\" gender=\"female\" case=\"instrumental\" number=\"single\"",
    " animate=\"any\" gender=\"any\" case=\"nominative\" number=\"plural\"",
    " animate=\"any\" gender=\"any\" case=\"genitive\" number=\"plural\"",
    " animate=\"any\" gender=\"any\" case=\"dative\" number=\"plural\"",
    " animate=\"any\" gender=\"any\" case=\"instrumental\" number=\"plural\"",
    " animate=\"any\" gender=\"any\" case=\"prepositional\" number=\"plural\"",
    " form=\"short\" gender=\"male\" number=\"single\"",
    " form=\"short\" gender=\"female\" number=\"single\"",
    " form=\"short\" gender=\"neuter\" number=\"single\"",
    " form=\"short\" gender=\"any\" number=\"plural\""
};

#endif