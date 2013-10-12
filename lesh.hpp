
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

	string dump();
	/**
	 * is self evaluating ? (number)
	 */
	bool is_self_evaluating();

	/**
	 * is variable ?
	 */
	bool is_variable();

	/**
	 * is things to apply ? (eg. compound of operator and operands)
	 */
	bool is_application();

	/**
	 * is  primitive procedure ? (+ - ...)
	 */
	bool is_primitive_procedure();

	/**
	 * is  compound procedure ? (lambda...)
	 */
	bool is_compound_procedure();

	/**
	 * is lambda ? (starts with "lambda")
	 */
	bool is_lambda();

	/**
	 * is definition  ? (starts with "define")
	 */ 
	bool is_definition();

	/**
	 * is if statement ?
	 */
	bool is_if();

	Value operatorr();

	Vlist operands();
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
        Value lookup_variable_value(Value var);
        void extend_environment( Vlist params, Vlist  vals );
        Value define_variable( Symbol var, Value val );
        //int set_variable_value( string var, Value val );
};


 Value eval( Value exp, Env& env );
 Value eval_sequence( Vlist exps, Env& env );
 Value apply( Value procedure ,Vlist argument );
 Vlist list_of_values( Vlist,Env  );
 Value apply_primitive_procedure( Value proc, Vlist arguments );
 Procedure make_procedure( Vlist params, Vlist body, Env *env );
 Value make_lambda( Vlist params,Vlist body );
 Frame make_frame( Vlist syms, Vlist vals );
 Value eval_definition( Value exp, Env& env );
 Value eval_if( Value exp, Env& env );
 Value cond_if( Value exp );


#endif
