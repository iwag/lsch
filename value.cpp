#include "lesh.hpp"

#include <stdlib.h>
#include <stdio.h>

ostream& operator<<(ostream &os, const Value &v){
	Vlist::iterator	i;
	
	switch( v.type ){
	case NUM:
                 os<<v.num;
		 return os;
	case SYMBOL:
		//return string("<#SYM ")+sym+">";
		os<<v.sym;
		 return os;
	case LIST:
		os << "[ ";
		/*
		for (i=v.vlist.begin(); i!=v.vlist.end(); i++){
			os << " " << *i;
		} 
		*/
		os << " ]";
		 return os;
	default:
		os << "**UNKNOWN-TYPE**";
		 return os;

	}
}

string Value::dump(){
	string	s;
	
		switch( type ){
		case NUM:
			  char    buf[128];
       	 sprintf(buf,"%d",num);
       	  return string(buf);
		case SYMBOL:
			//return string("<#SYM ")+sym+">";
			return sym;
		case LIST:
			s="[";
			for (Vlist::iterator i=vlist.begin(); i!=vlist.end(); i++){
				s = s+ " " + i->dump(); 
			} 
			return s+" ]";
			break;
		default:
			return string("**UNKNOWN-TYPE**");
		}

		return string("");
}

bool Value::is_self_evaluating(){
	return ( type==NUM );
}

bool Value::is_variable(){
	return ( type==SYMBOL );
}

bool Value::is_application(){
	return ( type==LIST ); 
}

bool Value::is_primitive_procedure(){
	return ( tag==PRIMITIVE );
}

bool Value::is_compound_procedure(){
	return  ( tag==PROCEDURE );
}

bool Value::is_lambda(){
	return  ( type==LIST && vlist.begin()->sym=="lambda" );
}

bool Value::is_definition(){
	return  ( type==LIST && vlist.begin()->sym=="define" );
}

bool Value::is_if(){
	return  ( type==LIST && vlist.begin()->sym=="if" );
}

Vlist Value::cond_clauses(){
	/* cdr */
	return Vlist(vlist.begin()+1, vlist.end());
}

/*
 * (if predicate consequent alternative)
 */
Value Value::if_predicate(){
	return vlist.at(1);
}

Value Value::if_consequent(){
	return vlist.at(2);
}

Value Value::if_alternative(){
	if ( vlist.size() == 3 ) // no-else
		return _V("false");
	return vlist.at(3);
}

Value Value::operatorr(){
	return vlist.at(0);
}

Vlist Value::operands(){
	Vlist::iterator start=vlist.begin();
	if ( start != vlist.end() ) start++;
	return Vlist(start,vlist.end());
}

Vlist Value::procedure_parameters(){
	//assert( type==LIST )
	//assert( tag==PROCEDURE )
	return vlist.begin()->vlist;
}

Vlist Value::procedure_body(){
	//assert( type==LIST )
	//assert( tag==PROCEDURE )
	return Vlist(vlist.begin()+1, vlist.end());
}

	/*
	Env procedure_environment(){
		//assert( type==LIST )
		//assert( tag==PROCEDURE )
		return env;
	}
	*/

Vlist Value::lambda_parameters(){
	return vlist.at(1).vlist;
}

Vlist Value::lambda_body(){
	return Vlist(vlist.begin()+2,vlist.end());
}

Value Value::definition_variable(){
	const Vlist::iterator second = vlist.begin()+1;
	if ( second->type == SYMBOL )
		return *second;
	else if ( second->type == LIST )
		return *(second->vlist.begin());
	else
		return *second;
}

Value Value::definition_value(){
	const Vlist::iterator	second=vlist.begin()+1;
	if ( second->type == SYMBOL ) {
		return *(second+1);
	} else if ( second->type == LIST ) {
		const Vlist::iterator	second_second = second->vlist.begin()+1;
		Vlist	params = Vlist(second_second,second->vlist.end());
		Vlist	body = Vlist(second+1,vlist.end());
		return make_lambda( params, body );
	} else {
		return *second;
	}
}

Env Value::procedure_environment(){
	return *env;
}

Value	Value::car(){
	return *vlist.begin();
}

Vlist	Value::cdr(){
	return Vlist(vlist.begin()+1,vlist.end());
}

