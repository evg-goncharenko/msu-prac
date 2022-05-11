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
    ERM_PARTICLE,                                                   // �������                                                                (1)
    ERM_INTERJECTION,                                               // ����������                                                             (2)
    ERM_PERSONAL_PRONOUN,                                           // ������ �����������                                                     (3-1)
    ERM_REFLEXIVE_PRONOUN,                                          // ���������� �����������                                                 (3-2)
    ERM_PRONOUN,                                                    // �����������                                                            (3-3)
    ERM_NUMBER_ONE,                                                 // ������������ ����                                                      (4-1)
    ERM_NUMBER_TWO,                                                 // ������������ ���, ���, �������                                         (4-2)
    ERM_NUMBER_THREE,                                               // ������������ ���, ������ �������, ���������, �������, �����, ������� � ������������� ������������
    ERM_NUMBER,                                                     // �������������� ������������                                            (4-4)
    ERM_NUMBER_BIFORM,                                              // ������������ � ����� �������                                           (4-5)
    ERM_NOUN_MALE_INANIMATE,                                        // ��������������� �������������� �������� ����                           (7-1)
    ERM_NOUN_MALE_ANIMATE,                                          // ��������������� ������������ �������� ����                             (7-2)
    ERM_NOUN_FEMALE_INANIMATE,                                      // ��������������� �������������� �������� ����                           (7-3)
    ERM_NOUN_FEMALE_ANIMATE,                                        // ��������������� ������������ �������� ����                             (7-4)
    ERM_NOUN_NEUTER_INANIMATE,                                      // ��������������� �������������� �������� ����                           (7-5)
    ERM_NOUN_NEUTER_ANIMATE,                                        // ��������������� ������������ �������� ����                             (7-6)
    ERM_NOUN_MALEFEMALE_ANIMATE,                                    // ��������������� ������������ ����������� ���� (��/��)                  (7-7)
    ERM_NOUN_MALE,                                                  // ��������������� �������� ���� (��/����)                                (7-8)
    ERM_NOUN_MALE_OR_FEMALE_INANIMATE,                              // ��������������� �������������� ��������/�������� ����                  (7-9) 
    ERM_NOUN_MALE_OR_FEMALE_ANIMATE,                                // ��������������� ������������ ��������/�������� ����                    (7-10)
    ERM_NOUN_FEMALE,                                                // ��������������� �������� ���� (��/����)                                (7-11)
    ERM_NOUN_NEUTER_OR_FEMALE_INANIMATE,                            // ��������������� �������������� ��������/�������� ����                  (7-12) 
    ERM_NOUN_NEUTER,                                                // ��������������� �������� ���� (��/����)                                (7-13)
    ERM_NOUN_NEUTER_OR_MALE_ANIMATE,                                // ��������������� ������������ ��������/�������� ����                    (7-14)
    ERM_NOUN_CONST_NEUTER_INANIMATE,                                // ��������������� ������������ �������������� �������� ����              (7-15) 
    ERM_NOUN_NEUTER_OR_MALE_INANIMATE,                              // ��������������� �������������� ��������/�������� ����                  (7-16)
	// added proper nouns - GIG - 22/06/2017
	ERM_NOUN_PROPER_FEMALE_ANIMATE,                                 // ��������������� ����������� ������������ �������� ����                 (7-17)
	ERM_NOUN_PROPER_MALE_ANIMATE,                                   // ��������������� ����������� ������������ �������� ����                 (7-18)
	ERM_NOUN_PROPER_MALEFEMALE_ANIMATE,                             // ��������������� ����������� ������������ ����������� ���� (��/��)      (7-19)
	ERM_NOUN_HYPOCOR_FEMALE_ANIMATE,                                // ��������������� ������������. ������������ �������� ����               (7-20)
	ERM_NOUN_HYPOCOR_MALE_ANIMATE,                                  // ��������������� ������������. ������������ �������� ����               (7-21)
	ERM_NOUN_HYPOCOR_MALEFEMALE_ANIMATE,                            // ��������������� ������������. ������������ ����������� ���� (��/��)    (7-22)
	ERM_NOUN_PROPER_FEMALE_INANIMATE,                               // ��������������� ����������� �������������� �������� ����               (7-23)
	ERM_NOUN_PROPER_MALE_INANIMATE,                                 // ��������������� ����������� �������������� �������� ����               (7-24)
	ERM_NOUN_PROPER_SURNAME,										// ���������������-������� (����������� ������������ �/� ����)			  (7-25)
	// end - GIG - 22/06/2017
	ERM_ADJECTIVE,                                                  // ��������������                                                         (8-1)
    ERM_PRONOUN_ADJECTIVE,                                          // ������������ ��������������                                            (8-2)
    ERM_NUMBER_ORDINAL,                                             // ���������� ������������                                                (8-3)
    ERM_PARTICIPLE,                                                 // ���������                                                              (8-4)
    ERM_POSSESIVE_ADJECTIVE,                                        // �������������� ��������������                                          (8-5)
    ERM_VERB_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE,                 // ������ ������������ ������������ �������������� ���� (�� ������������ � �� ���������)  (9-1)
    ERM_VERB_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE,                   // ������ ������������ ������������ ������������ ���� (�� ������������ � �� ���������)    (9-2)
    ERM_VERB_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE,                   // ������ ������������ ���������� �������������� ���� (�� ������������ � �� ���������)    (9-3)
    ERM_VERB_IRREFLEXIVE_TRANSITIVE_PERFECTIVE,                     // ������ ������������ ���������� ������������ ���� (�� ������������ � �� ���������)      (9-4)
    ERM_VERB_REFLEXIVE_IMPERFECTIVE,                                // ������ ���������� ������������ �������������� ���� (�� ������������ � �� ���������)    (9-5)
    ERM_VERB_REFLEXIVE_PERFECTIVE,                                  // ������ ���������� ������������ ������������ ���� (�� ������������ � �� ���������)      (9-6)
    ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE,   // ������ ������������ ������������ ������������ �������������� ����      (9-7)
    ERM_VERB_FREQUENTATIVE_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE,     // ������ ������������ ������������ ���������� �������������� ����        (9-8)
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_IMPERFECTIVE,      // ������ ��������� ������������ ������������ �������������� ����         (9-9)
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_INTRANSITIVE_PERFECTIVE,        // ������ ��������� ������������ ������������ ������������ ����           (9-10)
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_IMPERFECTIVE,        // ������ ��������� ������������ ���������� �������������� ����
    ERM_VERB_IMPERSONAL_IRREFLEXIVE_TRANSITIVE_PERFECTIVE,          // ������ ��������� ������������ ���������� ������������ ����
    ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_IMPERFECTIVE,        // ������ ��������� ���������� ������������ �������������� ����
    ERM_VERB_IMPERSONAL_REFLEXIVE_INTRANSITIVE_PERFECTIVE,          // ������ ��������� ���������� ������������ ������������ ����    
    ERM_VERB_IRREFLEXIVE_INTRANSITIVE_BIFORM,                       // ������ ������������ ������������ ����������                            (9-15)
    ERM_VERB_IRREFLEXIVE_TRANSITIVE_BIFORM,                         // ������ ������������ ���������� ����������                              (9-16)
    ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM,                         // ������ ���������� ������������ ����������                              (9-17)
    ERM_VERB_REFLEXIVE_INTRANSITIVE_BIFORM_EXT,                     // ������ ���������� ������������ ���������� (������ � ����/���)          (9-18)
    ERM_VERB_BE_IRREFLEXIVE_INTRANSITIVE,                           // ������ "����" ������������ ������������                                (9-19)
    ERM_PREPOSITION,                                                // �������                                                                (10)
    ERM_CONJUNCTION,                                                // ����                                                                   (11)
    ERM_ADVERB,                                                     // �������                                                                (12)
    ERM_PREDICATIVE,                                                // ����������                                                             (13)
    ERM_PARENTHESIS,                                                // ������� �����                                                          (14)
    //ERM_ACRONYM,                                                    // ����������
	// add new morpho-syntactical class constants here
	ERM_LAST_CLASS_NUMBER											// ��������� ����� ������ (�� ������������)
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