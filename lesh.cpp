
#include "lesh.hpp"

Value parser(istream& in);
//Value parser(string::iterator s);

int main(void){
	Env	env = Env();

	Primitive::define_variables(env);
	
	// define primitive values and functions

	while (1) {
		Value	val =parser(cin);
		if ( !cin.good() ||  cin.eof() ) break;
		cout << val.dump() << endl;
		// eval val in current environment
		cout << eval(val,env).dump() << endl;
	}

	return 0;
}


Value eval( Value exp, Env& env ){
	Value	ret=Value("ooERRoo");
	if ( exp.is_self_evaluating() )
		// return *exp*
		ret = exp;
	else if ( exp.is_variable() )
		// return lookup variable in environment
		ret = env.lookup_variable_value( exp );
	else if ( exp.is_if() )
		ret = eval_if( exp,env );
	else if ( exp.is_definition() )
		ret = eval_definition( exp, env );
	else if ( exp.is_lambda() )
		ret = make_procedure( exp.lambda_parameters(), exp.lambda_body(), &env );
	else if ( exp.is_application() )
		// apply exp
		ret = apply( eval( exp.operatorr(),env ), 
			list_of_values( exp.operands(), env ) );
	return ret;
}

Value apply( Value procedure ,Vlist argument ){
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

Vlist list_of_values( Vlist exps,Env env  ){
	Vlist	var_list;
	for ( auto itr=exps.begin(); itr!=exps.end(); itr++ ){
		var_list.push_back( eval(*itr,env) );
	}
	return var_list;
}


Value apply_primitive_procedure( Value proc, Vlist arguments ){
 	int	ret=-62370895;
	Vlist::iterator	itr,j;

 	if ( proc.sym == "+" ){
		ret=0;
		for( auto itr=arguments.begin(); itr!=arguments.end(); itr++ ){
			ret += itr->num;
		}
	} else if ( proc.sym == "-" ){
		*(itr=arguments.begin());
		ret = itr->num;
		for( itr++; itr!=arguments.end(); itr++ ){
			ret -= itr->num;
		}
	} else if ( proc.sym == "*" ){
		ret=1;
		for( itr=arguments.begin(); itr!=arguments.end(); itr++ ){
			ret *= itr->num;
		}
	} else if ( proc.sym == "=" ){
		itr=j=arguments.begin();
		++itr;
		//assert( j->type == NUM );
		if ( itr->num == j->num ) 
			return  "true";
		else
			return  "false";
	} else if ( proc.sym == "null?" ){
		Vlist::iterator first=arguments.begin();
		if ( first->type == LIST && first->vlist.size()==0 )
			return  "true";
		else
			return  "false";
	} else if ( proc.sym == "list" ){
		Vlist	vl;
		vl=Vlist(arguments.begin(),arguments.end());
		return vl;
	} else if ( proc.sym == "car" ) {
		if ( arguments.size() != 1 ) cerr << "err-argument" << endl;
		Vlist::iterator first=arguments.begin();
		return *(first->vlist.begin());
	} else if ( proc.sym == "cdr" ) {
		if ( arguments.size() != 1 ) cerr << "err-argument" << endl;
		Vlist::iterator first=arguments.begin();
		return Vlist(first->vlist.begin()+1,first->vlist.end());
	} 


	return Value(ret);
 }

 Procedure make_procedure( Vlist params, Vlist body, Env *e ){
	return Value( params,body,e );
 }

 Value make_lambda( Vlist params,Vlist body ){
 	Vlist	vl;
	vl.push_back( "lambda" ); 
	vl.push_back( params ); 
	vl.insert( vl.end(), body.begin(), body.end() );
	return Value( vl );
 }

Frame make_frame( Vlist syms, Vlist vals ){
	Frame	fr;
	Vlist::iterator	si,vi;

	assert( syms.size() == vals.size() );

	for ( si=syms.begin(), vi=vals.begin(); si!=syms.end(); si++,vi++ ){
		fr.insert( make_pair(si->sym,*vi) );
	}

	return fr;
 }

/*
 * (define (eval-definition exp env)
 * 	(define-variable (definition-variable exp)
 * 		(eval (definition-value exp) env) ))
 */
 Value eval_definition( Value exp, Env& env ){
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
 Value eval_if( Value exp, Env& env ){
 	if ( _V("true").sym==eval(exp.if_predicate(),env).sym ){
		return eval( exp.if_consequent(),env);
	} else {
		return eval( exp.if_alternative(),env);
	}
 }

 Value eval_sequence( Vlist exps, Env& env ){
 	Vlist::iterator	it;
	Value	ret;
	for ( it=exps.begin(); it!=exps.end(); it++ ){
		ret = eval( *it, env );
	}
	return ret;
 }

