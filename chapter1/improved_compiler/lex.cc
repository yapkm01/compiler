#include "include/lex.h"
#include <iostream>
#include <cctype>
#include <string>
using namespace std;

string inputBuffer;
string::iterator currentItr = inputBuffer.begin();
string::iterator yyTextItr = inputBuffer.begin();

int yyleng = 0;                                /* Lexeme Length        */
int yylineno = 0;                              /* Input line number    */

int lex() {

	currentItr = yyTextItr + yyleng;       /* Skip current lexeme  */
	while (1) {
		while (currentItr == inputBuffer.end()) {
			currentItr = inputBuffer.begin();
			if (!getline(cin, inputBuffer)) {
				inputBuffer.clear();
				return EOI;
			}
			++yylineno;
			while (isspace(*currentItr))
				++currentItr;
		}

		for (; *currentItr; ++currentItr) {
			yyTextItr = currentItr;
			yyleng = 1;

			switch(*currentItr) {
				case EOF: return EOI;
				case ';': return SEMI;
				case '+': return PLUS;
				case '*': return TIMES;
				case '(': return LP;
				case ')': return LP;
				case '\n':
				case '\t':
				case ' ': break;
				default:
					  if (!isalnum(*currentItr))
						  cout << "Ignoring illegal input " << *currentItr << endl;
					  else {
						  while (currentItr != inputBuffer.end() && isalnum(*currentItr))
							  ++currentItr;
						  yyleng = currentItr - yyTextItr;
						  return NUM_OR_ID;
					  }
					  break;
			}
		}
	}
}

static int lookahead = -1;

int match(int token) {
	/* Return true if "token" matches the current lookahead symbol. */
	if (lookahead == -1)
		lookahead = lex();

	return token == lookahead;
}

void advance() {
	/* Advance the lookahead to the next input symbol */
	lookahead = lex();
}

