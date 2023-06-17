#ifndef TEXT_DATA_HPP
#define TEXT_DATA_HPP

// Macros
// IOP TEXTS
#ifdef ECLI_HPP
#define HELP_ERROR_COMMAND_NFOUND "The command you have entered could not be identified. \
If you don't know what to do, you can consider to use 'help' command or 'exit' simply.\n"

#define HELP_PRONOUN_IDS "-p1: I\n-p2: You\n-p3: We\n-p4: \
They\n-p5: He\n-p6: She\n-p7: It\n"

#define HELP_MAIN "\t\x1b[38;5;45mclear\x1b[0m : Use this command to clear console.\n\
\t\x1b[38;5;45mdelete\x1b[0m : Use this command to delete data.\n\
\t\x1b[38;5;45mexit\x1b[0m : Use this command to exit the program.\n\
\t\x1b[38;5;45mhelp\x1b[0m : Use this command to get information about commands.\n\
\t\x1b[38;5;45mparse\x1b[0m : Use this command to parse verbs, nouns, etc.\n\
\t\x1b[38;5;45ms_parse\x1b[0m : Use this command to parse a sentence.\n\
\t\x1b[38;5;45mprint\x1b[0m : Use this command to print some data.\n\
\t\x1b[38;5;45mq_generate\x1b[0m : Use this command to create simple question.\n\
\t\x1b[38;5;45mread\x1b[0m : Use this command to read data from disk.\n\
\t\x1b[38;5;45mtester\x1b[0m : Use this command to run morphological tester.\n\
\t\x1b[38;5;45mtokenize\x1b[0m : Use this command to tokenize a string.\n\
\t\x1b[38;5;45m-command\x1b[0m : Use this parameter to see parsed command text.\n\
For the more information about spesific command or usage type \"help read\". i.e\n"

#define HELP_PARAM_CLEAR "\x1b[38;5;45mclear\x1b[0m : This command clears the chatbot console and has no parameter to type.\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m clear\x1b[49m"

#define HELP_PARAM_DELETE "\x1b[38;5;45mdelete\x1b[0m : This command deletes data from ram and can have these parameters;\n\
\t -tokens : This parameter deletes existing tokens.\n\
\t -nouns  : This parameter deletes all nouns dictionary.\n\
\t -verbs  : This parameter deletes all verbs dictionary.\n\
\t -pronouns  : This parameter deletes all pronouns dictionary.\n\
\t -adverbs  : This parameter deletes all adverbs dictionary.\n\
\t -adjectives  : This parameter deletes all adjectives dictionary.\n\
\t -prepositions : This parameter deletes all prepositions dictionary.\n\
\t -conjunctions  : This parameter deletes all conjunctions dictionary.\n\
\t -interjections : This parameter deletes all interjections dictionary.\n\
\t -aux_verbs : This parameter deletes all auxiliary verbs dictionary.\n\
\t -determiners : This parameter deletes all determiners dictionary.\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m delete -verbs\x1b[49m"

#define HELP_PARAM_EXIT "\x1b[38;5;45mexit\x1b[0m : This command closes the application and has no parameter to type.\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m exit\x1b[49m"

#define HELP_PARAM_PARSE "\x1b[38;5;45mparse\x1b[0m : This command parses text from console and can have these parameters;\n\
\t \"\"    : (With no parameter) You can parse an input by looking all types.\n\
\t -noun : This parameter parse the input by just looking to nouns.\n\
\t -verb : This parameter parse the input by just looking to verbs.\n\
\t -pronoun : This parameter parse the input by just looking to pronouns.\n\
\t -adverb  : This parameter parse the input by just looking to adverbs.\n\
\t -adjective : This parameter parse the input by just looking to adjectives.\n\
\t -preposition : This parameter parse the input by just looking to prepositions.\n\
\t -conjunction  : This parameter parse the input by just looking to conjunctions.\n\
\t -interjection : This parameter parse the input by just looking to interjections.\n\
\t -aux_verb : This parameter parse the input by just looking to auxiliary verb.\n\
\t -determiner : This parameter parse the input by just looking to determiner.\n\
Example usages;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m parse \"speaking\"\x1b[49m\n\
\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m parse -adverb \"speaking\"\x1b[49m"

#define HELP_PARAM_PRINT "\x1b[38;5;45mprint\x1b[0m : This command prints the existing data in ram and can have these parameters;\n\
\t -irregulars: This parameter prints all irregular verbs.\n\
\t -tokens: This parameter prints lastly tokenized sentence.\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m print -irregulars\x1b[49m"

#define HELP_Q_GENERATE_PRINT "\x1b[38;5;45mprint\x1b[0m : This command generates question based on the sentence type\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m q_generate \"He lives in Turkey.\"\x1b[49m"

#define HELP_PARAM_READ "\x1b[38;5;45mread\x1b[0m : This command reads data from disk and and can have these parameters;\n\
\t all : This parameter reads all dictionaries in the default folder.\n\
\t -dir : This parameter indicates the directory of the file.\n\
\t -noun : This parameter specifies the file as noun.\n\
\t -verb : This parameter specifies the file as verb.\n\
\t -adverb : This parameter specifies the file as adverb.\n\
\t -pronoun : This parameter specifies the file as pronoun.\n\
\t -adjective : This parameter specifies the file as adjective.\n\
\t -preposition : This parameter specifies the file as preposition.\n\
\t -conjunction : This parameter specifies the file as conjunction.\n\
\t -interjection : This parameter specifies the file as interjection.\n\
\t -aux_verb : This parameter specifies the file as auxiliary verb.\n\
\t -determiner : This parameter specifies the file as determiner.\n\
Example usages;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m read -dir \"dict/nouns.txt\" -noun\x1b[49m\n\
\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m read -dir \"dict/verbs.txt\" -verb\x1b[49m"

#define HELP_PARAM_S_PARSE "\x1b[38;5;45ms_parse\x1b[0m : This command parse a sentence and doesn't take any parameter.\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m s_parse \"This is a simple sentence.\"\x1b[49m"

#define HELP_PARAM_TESTER "\x1b[38;5;45mtester\x1b[0m : This command runs the morphological tester.\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m tester -dir \"test.txt\"\x1b[49m"

#define HELP_PARAM_TOKENIZE "\x1b[38;5;45mtokenize\x1b[0m : This command tokenize an input and doesn't take any parameter.\n\
Example usage;\n\t\x1b[48;5;237m\x1b[36mECLI $\x1b[39m tokenize \"This is a simple sentence.\"\x1b[49m"

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
