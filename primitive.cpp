#include "lesh.hpp"	

namespace Primitive {
Value apply_plus(const Vlist& arguments);
Value apply_minus(const Vlist& arguments);
Value apply_multiply(const Vlist& arguments);
Value apply_equal(const Vlist& arguments);
Value apply_null(const Vlist& arguments);
Value apply_list(const Vlist& arguments);
Value apply_car(const Vlist& arguments);
Value apply_cdr(const Vlist& arguments);

const Symbol END_OF_FUNC_LIST = "__END_OF_FUNC_LIST__";

typedef Value (*PrimitiveFuncp)(const Vlist&);

/**
 * symbol and function table
 */
const pair<Symbol, PrimitiveFuncp> func_table[] = {
	make_pair(SYMBOL_PLUS, &apply_plus),
	make_pair(SYMBOL_MINUS, &apply_minus),
	make_pair(SYMBOL_MULTIPLY, &apply_multiply),
	make_pair(SYMBOL_EQUAL, &apply_equal),
	make_pair(SYMBOL_NULL, &apply_null),
	make_pair(SYMBOL_LIST, &apply_list),
	make_pair(SYMBOL_CAR, &apply_car),
	make_pair(SYMBOL_CDR, &apply_cdr),
	make_pair(END_OF_FUNC_LIST, &apply_plus),
};


void define_variables(Env &env) {
	env.define_variable( SYMBOL_TRUE, Value(SYMBOL_TRUE));
	env.define_variable( SYMBOL_FALSE, Value(SYMBOL_FALSE));
	for ( auto i=0; func_table[i].first!=END_OF_FUNC_LIST; i++ ) {
		env.define_variable( func_table[i].first, Value(func_table[i].first, PRIMITIVE));
	}
}

Value apply_procedure(const Symbol& sym, const Vlist& arguments) {
	for ( auto i=0; func_table[i].first!=END_OF_FUNC_LIST; i++ ) {
		if ( func_table[i].first == sym ) {
			return func_table[i].second(arguments);
		}
	}
	return Value(SYMBOL_ERROR);
}

Value apply_plus(const Vlist& arguments) {
		auto ret=0;
		for( auto itr=arguments.begin(); itr!=arguments.end(); itr++ ){
			ret += itr->num;
		}
		return Value(ret);
}

Value apply_minus(const Vlist& arguments) {
		auto itr=arguments.begin();
		auto ret = itr->num;
		for( itr++; itr!=arguments.end(); itr++ ){
			ret -= itr->num;
		}
		return Value(ret);
} 

Value apply_multiply(const Vlist& arguments) {
		auto ret=1;
		for( auto itr=arguments.begin(); itr!=arguments.end(); itr++ ){
			ret *= itr->num;
		}
		return Value(ret);
} 

Value apply_equal(const Vlist& arguments) {
		auto itr=arguments.cbegin();
		auto j = itr;
		++itr;
		//assert( j->type == NUM );
		if ( itr->num == j->num ) 
			return  Value(SYMBOL_TRUE);
		else
			return  Value(SYMBOL_FALSE);
}

Value apply_null(const Vlist& arguments) {
		auto first=arguments.cbegin();
		if ( first->type == LIST && first->vlist.size()==0 )
			return  Value(SYMBOL_TRUE);
		else
			return  Value(SYMBOL_FALSE);
}

Value apply_list(const Vlist& arguments) {
		return Vlist(arguments.begin(),arguments.end());
}

Value apply_car(const Vlist& arguments) {
		if ( arguments.size() != 1 ) cerr << "err-argument" << endl;
		auto first=arguments.cbegin();
		return *(first->vlist.begin());
}

Value apply_cdr(const Vlist& arguments) {
		if ( arguments.size() != 1 ) cerr << "err-argument" << endl;
		auto first=arguments.begin();
		return Vlist(first->vlist.begin()+1,first->vlist.end());
}


}

