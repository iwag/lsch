
#ifndef LESH_DEFINED
#define LESH_DEFINED

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <assert.h>

using namespace std;

typedef enum { NUM, SYMBOL, LIST } kSYMTYPE;
typedef enum { NONE,PRIMITIVE, PROCEDURE,LAMBDA,DEFINE } kTAG;

// prototype class
class Env;
class Value;

/** Symbol */
typedef string	Symbol;
/** Value list*/
typedef vector<Value> Vlist;
typedef Value _V;
typedef Value Procedure;
typedef map<string,Value> Frame;



/**
 * Value
 *
 */ 
class Value{
	kTAG tag;
	Env*	env;

public:
	kSYMTYPE type;
	int	num;
	Symbol	sym;
	Vlist	vlist;

	/** 
	 * constructor for LIST
	 */
	Value() : tag(NONE), type(LIST), num(0), sym(""), vlist() {
	}

	/** 
	 * constructor for number
	 */
	Value(int number) : tag(NONE), type(NUM), num(number), sym(""), vlist() {
	}

	/** 
	 * constructor for symbol 
	 */
	Value(string symbol_name) : tag(NONE), type(SYMBOL), 
		num(0), sym(Symbol(symbol_name)), vlist() {
	}

	/** 
	 * constructor for symbol (c-string)
	 */
	Value(const char *s) : tag(NONE), type(SYMBOL), num(0), sym(Symbol(s)), vlist() {
	}
	

	/** 
	 * constructor for tagged symbol 
	 */
	Value(string s, kTAG arg_tag) : tag(arg_tag), type(SYMBOL), num(0), sym(Symbol(s)), vlist() {
	}

	/**
	 * constructor for value list
	 */
	Value(Vlist vl) : tag(NONE), type(LIST), num(0), sym(""), vlist(vl) {
	}


	/**
	 * constructor for tagged value list 
	 */
	Value( Vlist vl,kTAG arg_tag ) : tag(NONE), type(LIST), num(0), sym(""), vlist(vl) {
	}

	/**
	 * constructor for two values 
	 */
	Value( Value v1, Value v2 ) : tag(NONE), type(LIST), num(0), sym("") {
		vlist.push_back(v1);
		vlist.push_back(v2);
	}

	/**
	 * constructor for three values 
	 */
	Value( Value v1, Value v2, Value v3 ) : tag(NONE), type(LIST), num(0), sym("") {
		vlist.push_back(v1);
		vlist.push_back(v2);
		vlist.push_back(v3);
	}

	/**
	 * constructor for proceduere
	 */
	Value( Vlist params, Vlist body, Env* arg_env) : tag(PROCEDURE), type(LIST), num(0), sym("") {
		vlist.push_back(Value(params));
		vlist.insert(vlist.end(), body.begin(), body.end() );
		env = arg_env;
	}

	string dump(void);
	/**
	 * is self evaluating ? (number)
	 */
	bool is_self_evaluating(void) const;

	/**
	 * is variable ?
	 */
	bool is_variable(void) const;

	/**
	 * is things to apply ? (eg. compound of operator and operands)
	 */
	bool is_application(void) const;

	/**
	 * is  primitive procedure ? (+ - ...)
	 */
	bool is_primitive_procedure(void) const;

	/**
	 * is  compound procedure ? (lambda...)
	 */
	bool is_compound_procedure(void) const;

	/**
	 * is lambda ? (starts with "lambda")
	 */
	bool is_lambda(void) const;

	/**
	 * is definition  ? (starts with "define")
	 */ 
	bool is_definition(void) const;

	/**
	 * is if statement ?
	 */
	bool is_if(void) const;

	Value operatorr(void) const;

	Vlist operands(void);
	Vlist procedure_body();
	Env procedure_environment();
	Vlist procedure_parameters();
	Vlist lambda_parameters();
	vector<Value> lambda_body();
	Value definition_variable();
	Value definition_value();
	Value if_predicate();
	Value if_consequent();
	Value if_alternative();
	Vlist begin_action();
	Vlist cond_clauses();
	Value	car();
	Vlist	cdr();
	friend ostream& operator<<(ostream &os, const Value &v);

private:
};


/**
 * environment
 *
 * member is Map of symbol and value (frames)
 */ 
class Env{
public:
    list<Frame>     frames;
	Env(){
		frames.push_back( Frame() );
	}
    Env(Frame fr){
        frames.push_back(fr);
    }

	Value lookup_variable_value(const Symbol& sym);
	Value define_variable(const Symbol& sym, const Value& val);
	void extend_environment( const Vlist& syms, const Vlist& vals);
};

namespace Primitive {
	const Symbol SYMBOL_PLUS = "+";
	const Symbol SYMBOL_MINUS = "-";
	const Symbol SYMBOL_MULTIPLY = "*";
	const Symbol SYMBOL_EQUAL = "=";
	const Symbol SYMBOL_NULL = "null?";
	const Symbol SYMBOL_CAR = "car";
	const Symbol SYMBOL_CDR = "cdr";
	const Symbol SYMBOL_LIST = "list";
	const Symbol SYMBOL_FALSE = "false";
	const Symbol SYMBOL_TRUE = "true";
	const Symbol SYMBOL_ERROR = "error";
	const Symbol SYMBOL_LAMBDA = "lambda";
	const Symbol SYMBOL_DEFINE = "define";
	const Symbol SYMBOL_IF = "if";

	void define_variables(Env &env);
}



#endif
