
/* Basic parser, shows the structure but there's no code generation */
#include <iostream>
#include "include/lex.h"
#include "include/improved.h"
#include "include/name.h"
using namespace std;

static void term(const char* tempvar);
// void expr_prime();
static void factor(const char* tempvar);
// void term_prime();
static void expression(const char* tempvar);

//void statements() {
//  	/* statements -> expression SEMI
//   	 *	       | expression SEMI statements
//   	 */
//
//   	expression();
//
//   	if (match(SEMI))
//   		advance();
//   	else cout << yylineno << ": Inserting missing semicolon\n";
//   		//
//   	if (!match(EOI))
//   		statements();
//}

void statements() {
	/* statements -> expression SEMI
	 * 	       | expression SEMI statements
	 */

	const char* tempvar;
	while (!match(EOI)) {
		expression(tempvar = newname());
		freename(tempvar);

		if (match(SEMI))
			advance();
		else cout << yylineno << ": Inserting missing semicolon\n";
	}
}

//static void expression() {
//  	/* expression -> term expression */
//
//   	term();
//   	expr_prime();
//}

//static void expr_prime() {
//  	/* expression -> PLUS term expression */
//  	 *	       | epsilon
//  	 */
//
//	if (match(PLUS)) {
//		advance();
// 	  	term();
//   		expr_prime();
//	}
//}

static void expression(const char* tempvar) {
	/* expression  -> term expression'
	 * expression' -> PLUS term expression' | epsilon
	 */

	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;

	const char* tempvar2;
	term(tempvar);
	while (match(PLUS)) {
		advance();
		term(tempvar2 = newname());
		cout << tempvar << "+=" << tempvar2 << endl;
		freename(tempvar2);
	}
}

//static void term() {
//  	/* term -> factor term' */
//
//  	factor();
//   	term_prime();
//}

//static void term_prime() {
//  	/* term' -> TIMES factor term'
//  	 *	  | epsilon
//  	 */
//
//	if (match(TIMES)) {
//		advance();
// 	  	term();
//   		term_prime();
//   	}
//}

static void term(const char* tempvar) {
	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;

	const char* tempvar2;
	factor(tempvar);
	while (match(TIMES)) {
		advance();
		term(tempvar2 = newname());
		cout << tempvar << "*=" << tempvar2 << endl;
		freename(tempvar2);
	}
}

static void factor(const char* tempvar) {
	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;

	if (match(NUM_OR_ID)) {
		cout << tempvar << "=" << *yyTextItr << endl;
		advance();
	} else if (match(LP)) {
		advance();
		expression(tempvar);
		if (match(RP))
			advance();
		else cout << yylineno << ": Mismatched parenthesis" << endl;
	} else cout << yylineno << ": Number of identifier expected" << endl;
}
