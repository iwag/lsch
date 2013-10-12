#include "lesh.hpp"	

#include <stdlib.h>

Value parser(istream& in){
	char	c;
	in>>c;
	while ( c == ' ' || c == '\n' ) in>>c;

	if ( c=='(' ){
		Vlist	vl;
		string	s;

		do {
			in>>c;
			if ( c=='\n' || c==')')
				break;
			in.unget();
			vl.push_back(parser(cin));
		} while ( 1 );
		return vl;
	} else {
		string	s;
		in.unget();
		in >> s;
		if ( (s.c_str()[s.size()-1]) == ')' ) {
			s.erase( s.size()-1, 1 );
			in.unget();
		}
		if ( isdigit(s.c_str()[0]) ) // or [0]=='-'
			return atoi(s.c_str());
		else
			return s;
	}
}

