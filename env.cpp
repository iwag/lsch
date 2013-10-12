#include "lesh.hpp"

static Frame make_frame( const Vlist& syms, const Vlist& vals ){
	Frame	fr;

	assert( syms.size() == vals.size() );

	for ( auto si=syms.begin(), vi=vals.begin(); si!=syms.end(); si++,vi++ ){
		fr.insert( make_pair(si->sym,*vi) );
	}

	return fr;
}



Value Env::lookup_variable_value(const Symbol& sym){
	for( auto itr=frames.begin(); itr!=frames.end(); itr++ ){ // list
		Frame	f = *itr;
		auto	found = f.find(sym);//map
		if( found != f.end() ){
			return found->second;
		}
	}
	cout << "UNBIND ERROR" << endl;
	return Value("ooERRORoo");
}

Value Env::define_variable(const Symbol& sym, const Value& val){

	for ( auto itr=frames.begin(); itr!=frames.end(); itr++ ){ // list
		auto	found = itr->find(sym);//map
		if ( found != itr->end() ){ // found !!
			itr->erase( found ); // unbind if found
			break;
		}
	}

	frames.begin()->insert( make_pair(sym,val));
	return Value("ok");
}

void Env::extend_environment( const Vlist& syms, const Vlist& vals){
	frames.push_front( make_frame( syms, vals ) );
}


