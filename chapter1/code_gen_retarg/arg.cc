/* Basic parser, shows the structure but there's no code generation */
#include <iostream>
#include <stdarg.h>
#include "include/lex.h"
#include "include/arg.h"
#include "include/name.h"
using namespace std;

#define MAXFIRST 16
#define SYNCH SEMI

static void term(const char** tempvar);
static void factor(const char** tempvar);
static void expression(const char** tempvar);

int legal_lookahead(int first_arg, ...) {
/*
 * Simple error detection and recovery. Arguments are a a-terminated list of
 * those tokens that can legitimately come next in the input. If the list is
 * empty, the end of file must come next. Print an error message if
 * necessary. Error recovery is performed by discarding all input symbols
 * until one that's in the input list is found
 *
 * Return true if there's no error or if we recovered from the error,
 * false if we can't recover.
 */
	va_list args;
	int tok;
	int lookaheads[MAXFIRST], * p=lookaheads, * current;
	int error_printed = 0;
	int rval = 0;

	va_start(args, first_arg );

	if (!first_arg) {
		if (match(EOI))
			rval = 1;
	} else {
		*p++ = first_arg;
		while ((tok = va_arg(args, int)) && p < &lookaheads[MAXFIRST])
			*++p = tok;
		while (!match(SYNCH)) {
			for (current = lookaheads; current < p; ++current)
				if (match(*current)) {
					rval = 1;
					goto exit;
				}
			if (!error_printed) {
				cout << "Line " << yylineno << "Syntax error\n";
				error_printed = 1;
			}
			advance();
		}
	}
exit:
	va_end(args);
	return rval;
}

void statements() {
	/* statements -> expression SEMI
	 * 	       | expression SEMI statements
	 */

	const char* tempvar = nullptr;
	while (!match(EOI)) {
		expression(&tempvar);
		if (match(SEMI))
			advance();
		else cout << yylineno << ": Inserting missing semicolon\n";
		freename(tempvar);
	}
}

static void expression(const char** tempvar) {
	/* expression  -> term expression'
	 * expression' -> PLUS term expression' 
	 *              | epsilon
	 */

	const char* tempvar2 = nullptr;

	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;

	term(&(*tempvar));
	while (match(PLUS)) {
		advance();
		term(&tempvar2);
		cout << *tempvar << "+=" << tempvar2 << endl;
		freename(tempvar2);
	}
}

static void term(const char** tempvar) {
	/* term  -> factor term'
	 * term' -> TIMES factor term' 
	 *        | epsilon
	 */

	const char* tempvar2 = nullptr;

	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;

	factor(&(*tempvar));
	while (match(TIMES)) {
		advance();
		factor(&tempvar2);
		cout << *tempvar << "*=" << tempvar2 << endl;
		freename(tempvar2);
	}
}

static void factor(const char** tempvar) {
	/* factor  -> NUM_OR_ID
	 *          | LP expression RP
	 */

	*tempvar = nullptr;

	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;

	if (match(NUM_OR_ID)) {
		*tempvar = newname();
		cout << *tempvar << "=" << *yyTextItr << endl;
		advance();
	} else if (match(LP)) {
		advance();
		expression(tempvar);
		if (match(RP))
			advance();
		else cout << yylineno << ": Mismatched parenthesis" << endl;
	} else cout << yylineno << ": Number of identifier expected" << endl;
}
