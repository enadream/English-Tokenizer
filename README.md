 # English Tokenizer & Morphological Analyzer (C++)
 
 
 
 A custom-built English tokenizer and morphological analyzer developed entirely in C++ with zero external dependencies. This project showcases a deep dive into natural language processing fundamentals, parsing English sentences both syntactically and morphologically.
 
 
 
 ### Project Demo
 
 
 
To see a demonstration of BioSphere in action, click the image below:

 [![Watch the video](https://i3.ytimg.com/vi/_oUgIxFwHGc/maxresdefault.jpg)](https://youtu.be/_oUgIxFwHGc)
 
 
 ## 📜 General Summary
 
 
 
 This application was developed as a comprehensive full-stack project to demonstrate a robust understanding of C++ programming, English language rules, and core NLP concepts. Built from the ground up using only the C++ Standard Library, it tokenizes input text, analyzes the morphological structure of each word, and classifies them into one of ten grammatical categories.
 
 
 
 The system's logic is designed to handle various linguistic nuances, including suffixed words, irregular verb conjugations, and singular/plural noun forms, making it a powerful tool for syntactic analysis.
 
 
 
 For a complete and detailed explanation of the project's architecture, data structures, and algorithms, please refer to the [**Report.pdf**](https://github.com/enadream/English-Tokenizer/blob/main/Report.pdf).
 
 
 
 ## ✨ Key Features
 
 
 
 * **Zero Dependencies:** Built entirely in C++ using only the Standard Library.
 
 
 
 * **Syntactic Tokenization:** Breaks down raw English sentences into a clean stream of tokens (words and punctuation).
 
 
 
 * **Morphological Analysis:** Intelligently identifies the root form of words, accounting for:
 
 
 
   * **Suffixes** (e.g., `-ing`, `-ed`, `-s`).
 
 
 
   * **Irregular Verbs** (e.g., `went` -> `go`).
 
 
 
   * **Pluralization** (e.g., `cars` -> `car`, `geese` -> `goose`).
 
 
 
 * **Grammatical Classification:** Categorizes each word into one of 10 fundamental parts of speech:
 
 
 
   * Adjectives
 
 
 
   * Adverbs
 
 
 
   * Conjunctions
 
 
 
   * Determiners
 
 
 
   * Interjections
 
 
 
   * Nouns
 
 
 
   * Prepositions
 
 
 
   * Pronouns
 
 
 
   * Verbs (including auxiliary verbs)
 
 
 
 * **Command-Line Interface:** An interactive CLI for easy input and clear, formatted output.
 
 
 
 ## 🛠️ How It Works
 
 
 
 The application processes text through a sequential pipeline:
 
 
 
 1. **Input:** The user provides an English sentence via the command line.
 
 
 
 2. **Tokenization:** The `Tokenizer` module splits the input string into individual tokens.
 
 
 
 3. **Lexical Analysis:** Each token is looked up in the dictionary files located in the `/dict` directory.
 
 
 
 4. **Morphological Analysis:** If a direct match isn't found, the system applies a set of rules to check for suffixes, irregular forms, and other variations to find the word's root and type.
 
 
 
 5. **Classification & Output:** The system classifies the word based on the analysis and presents a detailed breakdown of the sentence's grammatical structure to the user.
 
 
 
 ## 🚀 Getting Started
 
 
 
 ### Prerequisites
 
 
 
 * A C++ compiler that supports C++17 (e.g., GCC, Clang, MSVC).
 
 
 
 ### Compilation
 
 
 
 1. Clone the repository:
 
 
 
    ```
    git clone https://github.com/enadream/English-Tokenizer.git
    cd English-Tokenizer
 
    ```
 
 
 
 2. Compile the source files. A simple way using `g++` is:
 
 
 
    ```
    g++ -std=c++17 src/*.cpp src/misc/*.cpp -o tokenizer
 
    ```
 
 
 
    *Note:* For a more robust build, you could use a `Makefile` or a *build system like `CMake`.*
 
 
 
 ### Running the Application
 
 
 
 Execute the compiled binary from the terminal:
 
 
 
 ```
 ./tokenizer
 
 ```
 
 
 
 The application will then prompt you for input.
 
 
 
 ## 📂 Project Structure
 
 
 
 ```
 .
 ├── dict/              # Dictionary files for each word type
 ├── src/               # All C++ source (.cpp) and header (.hpp) files
 │   ├── misc/          # Utility helper functions and data structures
 │   ├── main.cpp       # Main entry point of the application
 │   ├── tokenizer.hpp  # Header for the Tokenizer class
 │   └── ...            # Other class files (verb, noun, etc.)
 ├── Report.pdf         # In-depth project documentation
 └── README.md          # This file
 
 ```
 
 

