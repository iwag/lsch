#include "lesh.hpp"

#include <stdlib.h>
#include <stdio.h>

static Value make_lambda( const Vlist& params, const Vlist& body );

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

string Value::dump(void) const {
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
			for (auto i=vlist.begin(); i!=vlist.end(); i++){
				s = s+ " " + i->dump(); 
			} 
			return s+" ]";
			break;
		default:
			return string("**UNKNOWN-TYPE**");
		}

		return string("");
}

#define GEN_IS_FUNC_TYPE(_f,_t) \
bool Value::_f(void) const { return (type==_t); }  

GEN_IS_FUNC_TYPE(is_self_evaluating, NUM)
GEN_IS_FUNC_TYPE(is_variable, SYMBOL)
GEN_IS_FUNC_TYPE(is_application, LIST)

#define GEN_IS_FUNC_TAG(_f,_t) \
bool Value::_f(void) const { return (tag==_t); }  

GEN_IS_FUNC_TAG(is_primitive_procedure, PRIMITIVE)
GEN_IS_FUNC_TAG(is_compound_procedure, PROCEDURE)

#define GEN_IS_FUNC_LIST_SYM(_f,_s) \
bool Value::_f(void) const {return  ( type==LIST && vlist.begin()->sym==_s );} 

GEN_IS_FUNC_LIST_SYM(is_lambda, Primitive::SYMBOL_LAMBDA)
GEN_IS_FUNC_LIST_SYM(is_definition, Primitive::SYMBOL_DEFINE)
GEN_IS_FUNC_LIST_SYM(is_if, Primitive::SYMBOL_IF)


Vlist Value::cond_clauses() const {
	/* cdr */
	return Vlist(vlist.begin()+1, vlist.end());
}

/*
 * (if predicate consequent alternative)
 */
Value Value::if_predicate() const {
	return vlist.at(1);
}

Value Value::if_consequent() const {
	return vlist.at(2);
}

Value Value::if_alternative() const {
	if ( vlist.size() == 3 ) // no-else
		return Value(Primitive::SYMBOL_FALSE);
	return vlist.at(3);
}

Value Value::operatorr(void) const{
	return vlist.at(0);
}

Vlist Value::operands() const {
	auto start=vlist.begin();
	if ( start != vlist.end() ) start++;
	return Vlist(start,vlist.end());
}

Vlist Value::procedure_parameters() const {
	//assert( type==LIST )
	//assert( tag==PROCEDURE )
	return vlist.begin()->vlist;
}

Vlist Value::procedure_body()const{
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

Vlist Value::lambda_parameters(void) const{
	return vlist.at(1).vlist;
}

Vlist Value::lambda_body() const{
	return Vlist(vlist.begin()+2,vlist.end());
}

Value Value::definition_variable() const{
	const auto second = vlist.begin()+1;
	if ( second->type == SYMBOL )
		return *second;
	else if ( second->type == LIST )
		return *(second->vlist.begin());
	else
		return *second;
}

Value Value::definition_value(void) const{
	const auto	second=vlist.begin()+1;
	if ( second->type == SYMBOL ) {
		return *(second+1);
	} else if ( second->type == LIST ) {
		const auto	second_second = second->vlist.begin()+1;
		Vlist	params = Vlist(second_second,second->vlist.end());
		Vlist	body = Vlist(second+1,vlist.end());
		return make_lambda( params, body );
	} else {
		return *second;
	}
}

Env Value::procedure_environment(void) const{
	return *env;
}

Value Value::car(void) const{
	return *vlist.begin();
}

Vlist	Value::cdr(void) const{
	return Vlist(vlist.begin()+1,vlist.end());
}

static Value make_lambda( const Vlist& params, const Vlist& body ){
 	Vlist	vl;
	vl.push_back( Primitive::SYMBOL_LAMBDA ); 
	vl.push_back( params ); 
	vl.insert( vl.end(), body.begin(), body.end() );
	return Value( vl );
}



