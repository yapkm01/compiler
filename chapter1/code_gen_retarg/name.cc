#include <iostream>
#include "include/lex.h"
using namespace std;

const char* Names[] = {"t0","t1","t2","t3","t4","t5","t6","t7"}; 
const char** Namep = Names;

const char* newname() {
	if (Namep >= &Names[sizeof(Names)/sizeof(*Names)]) {
		cout << yylineno << ": Expression too complex\n" << endl;
		exit(1);
	}	
	return (*Namep++);
}

void freename(const char* s) {
	if (Namep > Names)
		*--Namep = s;
	else cout << yylineno << ": Internal error: Name stack underflow\n" << endl;
}
