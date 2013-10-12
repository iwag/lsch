
#include "lesh.hpp"

static Value eval( const Value& exp, Env& env );
static Value eval_sequence( const Vlist& exps, Env& env );
static Value apply( const Value& exp, Env& env ) ;
static Vlist list_of_values( const Vlist&,Env  );
static Value apply_primitive_procedure( const Value& proc, const Vlist& arguments );
static Procedure make_procedure( const Vlist& params, const Vlist& body, Env *env );
static Value eval_definition( const Value& exp, Env& env );
static Value eval_if( const Value& exp, Env& env );

extern Value parser(istream& in);


/**
 * main 
 */
int main(void){
	// create empty environment
	Env	env = Env();

	// define primitive values and functions
	Primitive::define_variables(env);
	

	/**
	 * eval loop
	 */
	while (1) {
		Value	val =parser(cin);
		// if istream is bad or eof then break
		if ( !cin.good() ||  cin.eof() ) break;
		cout << val.dump() << endl;
		// eval val in current environment
		cout << eval(val,env).dump() << endl;
	}

	return 0;
}


/**
 * eval
 */
static Value eval( const Value& exp, Env& env ){
	Value	ret=Value("ooERRoo");
	if ( exp.is_self_evaluating() )
		// return *exp*
		ret = exp;
	else if ( exp.is_variable() )
		// return lookup variable in environment
		ret = env.lookup_variable_value( exp.sym );
	else if ( exp.is_if() )
		ret = eval_if( exp,env );
	else if ( exp.is_definition() )
		ret = eval_definition( exp, env );
	else if ( exp.is_lambda() )
		ret = make_procedure( exp.lambda_parameters(), exp.lambda_body(), &env );
	else if ( exp.is_application() )
		// apply exp
		ret = apply( exp, env );
	return ret;
}

static Value apply( const Value& exp, Env& env ) {
		
	const Value& procedure = eval(exp.operatorr(), env); 
	const Vlist& argument = list_of_values( exp.operands(), env );

 	if ( procedure.is_primitive_procedure() ){
		return apply_primitive_procedure( procedure, argument );
	} else if ( procedure.is_compound_procedure() ){
		Env env = procedure.procedure_environment();
		env.extend_environment( 
			procedure.procedure_parameters(), argument);
		return eval_sequence(procedure.procedure_body(), env );
	}
	return Value("error-apply");
 }

static Vlist list_of_values( const Vlist& exps,Env env  ){
	Vlist	var_list;
	for ( auto itr=exps.begin(); itr!=exps.end(); itr++ ){
		var_list.push_back( eval(*itr,env) );
	}
	return var_list;
}


static Value apply_primitive_procedure( const Value& proc, const Vlist& arguments ){
	return Primitive::apply_procedure( proc.sym, arguments );
}

static Procedure make_procedure( const Vlist& params, const Vlist& body, Env *e ){
	return Value( params,body,e );
 }

/*
 * (define (eval-definition exp env)
 * 	(define-variable (definition-variable exp)
 * 		(eval (definition-value exp) env) ))
 */
static Value eval_definition( const Value& exp, Env& env ){
 	env.define_variable( exp.definition_variable().sym,
				eval( exp.definition_value(), env));
	return Value("ok");
 }

/*
 * (define (eval-if exp env) 
 * 	(if (true? (eval (if_predicate exp) env))
 * 		(eval (if-consequent exp) env)
 * 		(eval (if-alternative exp) env)))
 */
static Value eval_if( const Value& exp, Env& env ){
 	if ( Primitive::SYMBOL_TRUE ==eval(exp.if_predicate(),env).sym ){
		return eval( exp.if_consequent(),env);
	} else {
		return eval( exp.if_alternative(),env);
	}
 }

static Value eval_sequence( const Vlist& exps, Env& env ){
	Value	ret;
	for ( auto it=exps.begin(); it!=exps.end(); it++ ){
		ret = eval( *it, env );
	}
	return ret;
}

