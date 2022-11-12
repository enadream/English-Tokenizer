# ChatBot Application by Enadream

Welcome to the Enadream Chatbot command line interface(ECLI). When the program has started, the enadream chatbot cli will welcome you. I shortly abbreviated the interface as Ecli.


## Ecli Commands
- /help : Use this command to get information about commands.
- /exit : Use this command to terminate program.
- /clear : Use this command to clear console.
- /read : Use this command to read data from disk.
- /parse : Use this command to parse verbs, nouns, etc.
- /find : Use this command to find a verb, a noun etc.
- /print : Use this command to print some data.
- /delete : Use this command to delete a spesific dictionary.

### '/read' Usage and Other Parameters
- Structure of the command has to be like this: <br>
	`/read -dir "directory of the dictionary file relative to the .exe file" [word type]`
- Example Usage: <br>
	`/read -dir "dict/verbs.txt" -verb`

#### Available Parameters for '/read' command
- [-dir][""] : Defines the directory of the file
- [-typename] : Defines the type of dictionary
	- [-verb] : The dictionary contains verbs.
	- [-noun] : The dictionary contains nouns.
	- [-pronoun] : The dictionary contains pronouns.
	- [-adjective] : The dictionary contains adjectives.
	- [-adverb] : The dictionary contains adverbs.
