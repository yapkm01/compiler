#include <stdarg.h>
#include <iostream>
#include "include/lex.h"
using namespace std;

#define MAXFIRST 16
#define SYNCH SEMI

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
