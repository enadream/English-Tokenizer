#ifndef TEXT_DATA_HPP
#define TEXT_DATA_HPP

// Macros
// IOP TEXTS
#ifdef IO_OPERATION_HPP

#define HELP_ERROR_COMMAND_NFOUND "The command you have entered could not be identified. \
If you don't know what to do, you can consider to use 'help' command or 'exit' simply.\n"

#define HELP_PRONOUN_IDS "-p1: I\n-p2: You\n-p3: We\n-p4: \
They\n-p5: He\n-p6: She\n-p7: It\n"

#define HELP_MAIN "\texit : Use this command to terminate program.\n\
\tclear : Use this command to clear console.\n\
\tread : Use this command to read data from disk.\n\
\tparse : Use this command to parse verbs, nouns, etc.\n\
\tfind : Use this command to find a verb, a noun etc.\n\
\tprint : Use this command to print some data.\n\
\thelp : Use this command to get information about commands.\n\
For the more information about spesific command or usage type \"/help /read\" i.e\n"

#define HELP_TIME_TENSES "\t-t1: Past Simple Tense\n\
\t-t2: Past Continuous Tense\n\
\t-t3: Past Perfect Tense\n\
\t-t4: Past Perfect Continuous Tense\n\n\
\t-t5: Present Simple Tense\n\
\t-t6: Present Continuous Tense\n\
\t-t7: Present Perfect Tense\n\
\t-t8: Present Perfect Continuous Tense\n\n\
\t-t9:  Future Simple Tense\n\
\t-t10: Future Continuous Tense\n\
\t-t11: Future Perfect Tense\n\
\t-t12: Future Perfect Continuous Tense\n\n\
\t-t13: Conditional Simple Tense\n\
\t-t14: Conditional Continuous Tense\n\
\t-t15: Conditional Perfect Tense\n\
\t-t16: Conditional Perfect Continuous Tense\n"

#define HEADER_MESSAGE "************** Welcome to the ChatBot (V0.3 @enadream) **************\n\n"

#endif


// VERB TEXTS
#ifdef VERB_HPP
#define VERB_EX_NONE "None"
#define VERB_EX_IRREGULAR_V1 "Irregular Verb"
#define VERB_EX_IRREGULAR_V2 "Irregular Verb V2"
#define VERB_EX_IRREGULAR_V3 "Irregular Verb V3"
#define VERB_EX_ENDSWITH_E "Verb ends with 'e'"
#define VERB_EX_ENDSWITH_CY "Verb ends with Consonant + 'y'"
#define VERB_EX_ENDSWITH_Z "Verb ends with 'z'"
#define VERB_EX_ENDSWITH_S "Verb ends with 's'"
#define VERB_EX_ENDSWITH_X "Verb ends with 'x'"
#define VERB_EX_ENDSWITH_O "Verb ends with 'o'"
#define VERB_EX_ENDSWITH_SS "Verb ends with 'ss'"
#define VERB_EX_ENDSWITH_ZZ "Verb ends with 'zz'"
#define VERB_EX_ENDSWITH_CH "Verb ends with 'ch'"
#define VERB_EX_ENDSWITH_SH "Verb ends with 'sh'"
#define VERB_EX_ENDSWITH_WXY "Verb ends with 'w' or 'x' or 'y'"
#define VERB_EX_ENDSWITH_CE "Verb ends with Consonant + 'e'"
#define VERB_EX_ENDSWITH_IE "Verb ends with 'ie'"
#define VERB_EX_ENDSWITH_XVC "Verb has one syllable and ands with Vowel + Consonant"
#define VERB_EX_DOUBLE_LAST "Double last char"
#endif // VERB_HPP
#endif // !HELP_DATA_H
