About this project:
This project is a lexical analyzer generator written in C. Lexical Analysis is the first phase of the compiler also known as a scanner. It converts the High level input program into a sequence of Tokens.
Tokens:
A lexical token is a sequence of characters that can be treated as a unit in the grammar of the programming languages.

Example of tokens:

Type token (id, number, real, . . . )
Punctuation tokens (IF, void, return, . . . )
Alphabetic tokens (keywords)
How Lexical Analyzer functions
Tokenization i.e. Dividing the program into valid tokens.
Remove white space characters.
Remove comments.
How to Run this Project:
Note: This is a Lexical Analyzer for a particular subset of C language explained under Assumptions: . It may not be able to parse the all token of C++ language.
ASSUMPTIONS:
While designing the LexicalAnalyzer, I have assumed the following assumptions for my Language:
 Special Symbol: ; { } ( ) , #
 Keyword: int, char, float, bool, cin, cout, main
 Pre-processor Directives: include, define
 Library: iostream, studio, string
 Operators: *, +, >>, <<,>,<
 Numbers/Integers: 0 to 9.
 Identifies/ Variables: All alphabetic strings except the keywords, numbers, Pre-processor directive and library strings.
Sample Source Code:
MySourceCode.txt

#include <iostream>
#define LIMIT 5
using namespace std ;
int main(){
    // this comment is written by akshit mangotra for lexical analyzer to avoid reading the comments
    int A , B ;
    cin >> A >> B;
    cout << A * B ; 
}
