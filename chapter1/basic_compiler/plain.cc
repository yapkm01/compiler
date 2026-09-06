
/* Basic parser, shows the structure but there's no code generation */

#include <iostream>
#include "include/lex.h"
#include "include/plain.h"
using namespace std;

static void expression();
static void expr_prime();
static void term();
static void term_prime();
static void factor();

void statements() {
  	/* statements -> expression SEMI
   	 *	       | expression SEMI statements
   	 */

   	expression();

   	if (match(SEMI))
   		advance();
   	else cout << yylineno << ": Inserting missing semicolon\n";
   		//
   	if (!match(EOI))
   		statements();
}

static void expression() {
  	/* expression -> term expression' */

   	term();
   	expr_prime();
}

static void expr_prime() {
  	/* expression' -> PLUS term expression' 
  	 *	        | epsilon
  	 */

	if (match(PLUS)) {
		advance();
 	  	term();
   		expr_prime();
	}
}

static void term() {
  	/* term -> factor term' */

  	factor();
   	term_prime();
}

static void term_prime() {
  	/* term' -> TIMES factor term'
  	 *	  | epsilon
  	 */

	if (match(TIMES)) {
		advance();
 	  	term();
   		term_prime();
   	}
}

static void factor() {
  	/* factor -> NUM_OR_ID
  	 *	   | LP expression RP 
  	 */

	if (match(NUM_OR_ID)) {
		advance();
	} else if (match(LP)) {
		advance();
		expression();
		if (match(RP))
			advance();
		else cout << yylineno << ": Mismatched parenthesis" << endl;
	} else cout << yylineno << ": Number of identifier expected" << endl;
}
